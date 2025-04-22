#include <fstream>
#include <sstream>
#include <list>
#include <utility>
#include <map>
#include <vector>
#include "window.h"

using namespace std;

// Constants for category selection
const vector<string> CATEGORIES = {
    "Name", "Best_Resolution", "Memory", "Memory_Type", "Manufacturer"
};

void heapify(vector<list<string>>& data, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // If left child is larger than root
    if (left < n && data[left].front() > data[largest].front())
        largest = left;

    // If right child is larger than largest so far
    if (right < n && data[right].front() > data[largest].front())
        largest = right;

    // If largest is not root
    if (largest != i) {
        swap(data[i], data[largest]);
        heapify(data, n, largest);
    }
}

void heapSort(vector<list<string>>& data) {
    int n = data.size();

    // Build heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(data, n, i);

    // One by one extract elements from heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        swap(data[0], data[i]);
        // call max heapify on the reduced heap
        heapify(data, i, 0);
    }
}

int partition(vector<list<string>>& arr, int low, int high) {
    string pivot = arr[high].front();
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].front() <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<list<string>>& arr, int low, int high) {
    if (low < high) {
        // Find the partitioning index
        int part = partition(arr, low, high);

        // Separately sort elements before and after partition
        quickSort(arr, low, part - 1);
        quickSort(arr, part + 1, high);
    }
}

void quickSortGPUs(vector<list<string>>& columnToBeSorted) {
    if (columnToBeSorted.size() > 1) {
        quickSort(columnToBeSorted, 0, columnToBeSorted.size() - 1);
    }
}

vector<string> parseCSVLine(const string& line) {
    vector<string> result;
    string field;
    bool inQuotes = false;

    for (size_t i = 0; i < line.length(); i++) {
        char c = line[i];

        // Check for double quotes
        if (c == '"') {
            // Check if this is an escaped quote
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                field += '"';
                i++;
            }
            else {
                inQuotes = !inQuotes;  // Toggle quote state
                // Don't add the quote character to the field
            }
        }
        // Field separator
        else if (c == ',' && !inQuotes) {
            // End of field reached. Add to results
            result.push_back(field);
            field.clear();
        }
        // Regular character. Add to current field
        else {
            field += c;
        }
    }

    // Add the last field
    result.push_back(field);

    return result;
}


struct DataInfo {
    map<int, string> labels;
    vector<int> columnMapping;
    int sortedColumnIndex;
};

DataInfo loadData(vector<list<string>>& columnToBeSorted, int answer, string fileName, int sortType) {
    vector<vector<string>> data;
    ifstream file(fileName);
    map<string, int> categoryIDX;
    DataInfo dataInfo;

    if (!file.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        return dataInfo;
    }

    string line;
    if (!getline(file, line)) {
        cerr << "Error reading header from file" << endl;
        return dataInfo;
    }

    // Parse header row properly
    vector<string> headers = parseCSVLine(line);

    for (size_t i = 0; i < headers.size(); i++) {
        string& header = headers[i];

        // Trim whitespace
        size_t start = header.find_first_not_of(" \t\r\n\"");
        if (start != string::npos) {
            size_t end = header.find_last_not_of(" \t\r\n\"");

            if (end != string::npos) {
                header = header.substr(start, end - start + 1);
            }
            else {
                header = header.substr(start);
            }
        }
        else {
            header = "";
        }

        categoryIDX[header] = i;
        dataInfo.labels[i] = header;
    }

    // Categories for GPU information
    const vector<string> CATEGORY_NAMES = {
        "Name", "Best_Resolution", "Memory", "Memory_Type", "Manufacturer"
    };

    string mainNodeIDX = CATEGORY_NAMES[answer];

    if (categoryIDX.find(mainNodeIDX) == categoryIDX.end()) {
        cerr << "Error: Column '" << mainNodeIDX << "' not found in headers" << endl;
        return dataInfo;
    }

    int columnIndex = categoryIDX[mainNodeIDX];
    dataInfo.sortedColumnIndex = columnIndex;

    // Read data rows
    while (getline(file, line)) {
        vector<string> row = parseCSVLine(line);

        // Only add rows that have enough columns
        if (row.size() > columnIndex) {
            data.push_back(row);
        }
    }

    columnToBeSorted.clear();
    dataInfo.columnMapping.clear();

    dataInfo.columnMapping.push_back(columnIndex);

    for (int i = 0; i < headers.size(); i++) {
        if (i != columnIndex) {
            dataInfo.columnMapping.push_back(i);
        }
    }

    for (const auto& row : data) {
        list<string> augmentedRow;

        augmentedRow.push_back(row[columnIndex]);

        for (size_t i = 0; i < row.size(); i++) {
            if (i != columnIndex) {
                augmentedRow.push_back(row[i]);
            }
        }

        columnToBeSorted.push_back(augmentedRow);
    }

    // Sort the data
    switch (sortType) {
    case 0:
        heapSort(columnToBeSorted);
        break;
    case 1:
        quickSortGPUs(columnToBeSorted);
        break;
    default:
        cerr << "Invalid sort type specified" << endl;
        break;
    }

    return dataInfo;
}

int main() {
    string fileName = "All_GPUs.csv";
    vector<list<string>> columnToBeSorted;
    DataInfo dataInfo = loadData(columnToBeSorted, 0, fileName, 0);

    GUI gui;
    struct WindowCharacteristics wc = gui.getWindowChar();

    // Set up window title
    sf::Text windowTitle(wc.GUI_TITLE, wc.FONTS.at("roboto-bold"));
    windowTitle.setCharacterSize(wc.GUI_TITLE_SIZE);
    windowTitle.setFillColor(wc.GUI_TITLE_COLOR);
    windowTitle.setStyle(sf::Text::Underlined);

    // Center the title
    float guiTitleLength = windowTitle.getLocalBounds().width;
    float windowMid = (wc.WIDTH / 2) - (guiTitleLength / 2);
    windowTitle.setPosition(windowMid, wc.GUI_TITLE_Y);

    // Set up "Sort by" text
    sf::Text sortByText(wc.SORT_BY_TEXT, wc.FONTS.at("roboto-medium"));
    sortByText.setCharacterSize(wc.SORT_BY_SIZE);
    sortByText.setFillColor(wc.SORT_BY_COLOR);
    sortByText.setPosition(wc.SORT_BY_X, wc.SORT_BY_Y);

    // Set up "Sorting algorithms" text
    sf::Text sortAlgosText(wc.SORT_ALGOS_TEXT, wc.FONTS.at("roboto-medium"));
    sortAlgosText.setCharacterSize(wc.SORT_ALGOS_SIZE);
    sortAlgosText.setFillColor(wc.SORT_ALGOS_COLOR);
    sortAlgosText.setPosition(wc.SORT_ALGOS_X, wc.SORT_ALGOS_Y);

    int sortAlgo = 0;

    // Set up separator
    sf::RectangleShape separator;
    separator.setSize(sf::Vector2f(wc.WIDTH, wc.SEP_HEIGHT));
    separator.setPosition(wc.SEP_X, wc.SEP_Y);
    separator.setFillColor(wc.SEP_COLOR);

    // Scrolling variables
    float scrollOffset = 0.f;
    const float scrollSpeed = 20.f;
    const unsigned int visibleItems = 25;
    float maxScroll = 0.f;

    // Scrollbar control variables
    bool isDragging = false;
    float scrollbarAreaTop = 180.f;
    float scrollbarAreaHeight = wc.HEIGHT - 200.f;
    float lastMouseY = 0.f;

    // Scrollbar setup
    sf::RectangleShape scrollbarBackground(sf::Vector2f(10.f, scrollbarAreaHeight));
    scrollbarBackground.setPosition(wc.WIDTH - 20.f, scrollbarAreaTop);
    scrollbarBackground.setFillColor(sf::Color(200, 200, 200));

    // Set up scrollbar thumb
    sf::RectangleShape scrollbarThumb;
    float scrollbarRatio = 1.0f;
    float scrollbarThumbHeight = scrollbarAreaHeight;

    int currentAnswer = 0;
    bool isClicked = false;
    string text_body = "";

    // Track button clicked state for handling in the next frame
    bool buttonWasClicked = false;
    size_t clickedButtonIndex = 0;

    // Main window loop
    while (wc.window->isOpen()) {
        sf::Event event;
        bool showScrollbar = true;
        bool leftClickReleased = false;
        bool leftClickPressed = false;

        while (wc.window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                wc.window->close();
            }

            if (showScrollbar) {
                maxScroll = max(0.f, static_cast<float>(columnToBeSorted.size() - visibleItems) * 20.f);
            }

            if (showScrollbar && !columnToBeSorted.empty()) {
                scrollbarRatio = min(1.0f,
                    static_cast<float>(visibleItems) / static_cast<float>(columnToBeSorted.size()));
                scrollbarThumbHeight = scrollbarAreaHeight * scrollbarRatio;
            }

            scrollbarThumb.setSize(sf::Vector2f(10.f, scrollbarThumbHeight));
            scrollbarThumb.setFillColor(sf::Color(100, 100, 100));

            // Handle mouse wheel scrolling
            if (event.type == sf::Event::MouseWheelScrolled && showScrollbar) {
                scrollOffset -= event.mouseWheelScroll.delta * scrollSpeed;
                scrollOffset = max(0.f, scrollOffset);
                scrollOffset = min(maxScroll, scrollOffset);
            }
            // Handle mouse button pressed
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    leftClickPressed = true;
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                    // Check if click is on scrollbar thumb
                    if (scrollbarThumb.getGlobalBounds().contains(mousePos)) {
                        isDragging = true;
                        lastMouseY = mousePos.y;
                    }
                    // Check if click is on scrollbar background (jump to position)
                    else if (showScrollbar &&
                        mousePos.x >= wc.WIDTH - 20.f &&
                        mousePos.x <= wc.WIDTH - 10.f &&
                        mousePos.y >= scrollbarAreaTop &&
                        mousePos.y <= scrollbarAreaTop + scrollbarAreaHeight) {
                        float clickPosition = mousePos.y - scrollbarAreaTop;
                        float scrollPercentage = clickPosition / scrollbarAreaHeight;
                        scrollOffset = scrollPercentage * maxScroll;
                        scrollOffset = max(0.f, min(maxScroll, scrollOffset));
                        isDragging = true;
                        lastMouseY = mousePos.y;
                    }
                }
            }

            // Handle mouse button released
            else if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    leftClickReleased = true;
                    isDragging = false;
                }
            }
            // Handle mouse movement
            else if (event.type == sf::Event::MouseMoved && isDragging) {
                float deltaY = event.mouseMove.y - lastMouseY;
                lastMouseY = event.mouseMove.y;

                if (scrollbarAreaHeight > 0) {
                    float scrollPercentage = deltaY / scrollbarAreaHeight;
                    scrollOffset += scrollPercentage * maxScroll;
                    scrollOffset = max(0.f, min(maxScroll, scrollOffset));
                }
            }
        }

        // Update scrollbar thumb position
        if (showScrollbar && maxScroll > 0.f) {
            float thumbPosition = scrollbarAreaTop + (scrollOffset / maxScroll) *
                (scrollbarAreaHeight - scrollbarThumbHeight);
            scrollbarThumb.setPosition(wc.WIDTH - 20.f, thumbPosition);
        }

        // Prepare visible GPU data based on scroll position
        size_t startIndex = static_cast<size_t>(scrollOffset / 20.0f);
        size_t endIndex = min(startIndex + visibleItems, columnToBeSorted.size());

        size_t size = 0;
        float buttonY = wc.GPU_TEXT_Y;
        const float buttonWidth = 80.f;
        const float buttonHeight = 12.f;
        const float buttonSpacing = 4.f;
        const float button_text_size = 9.0f;
        vector<struct Button> buttons;
        stringstream ss;

        wc.window->clear(sf::Color::White);

        // If we have a pending button click from previous frame, process it now
        if (buttonWasClicked) {
            buttonWasClicked = false;
            stringstream new_ss;

            // Get the data for the clicked item
            auto items = columnToBeSorted[clickedButtonIndex];

            // Convert from list to vector for easier indexing
            vector<string> itemValues;
            for (auto& item : items) {
                itemValues.push_back(item);
            }

            // Output each value with its proper label
            for (size_t i = 0; i < itemValues.size() && i < dataInfo.columnMapping.size(); i++) {
                int originalIdx = dataInfo.columnMapping[i];
                if (dataInfo.labels.find(originalIdx) != dataInfo.labels.end()) {
                    new_ss << dataInfo.labels[originalIdx] << ": " << itemValues[i] << endl;
                }
                else {
                    new_ss << "Column " << originalIdx << ": " << itemValues[i] << endl;
                }
            }

            // Handle any extra values
            for (size_t i = dataInfo.columnMapping.size(); i < itemValues.size(); i++) {
                new_ss << "Extra data: " << itemValues[i] << endl;
            }

            isClicked = true;
            text_body = new_ss.str();
            cout << "Button clicked! Details for item " << clickedButtonIndex << endl;
        }

        if (!isClicked) {
            ss.clear();
            for (auto gpu_data : columnToBeSorted) if (gpu_data.front().empty()) size++;

            for (size_t i = startIndex + size; i < endIndex + size; i++) {
                if (endIndex + size > columnToBeSorted.size()) break;
                auto gpu_data = columnToBeSorted[i];
                if (!gpu_data.front().empty()) {
                    ss << gpu_data.front() << "\n";
                    float buttonX = wc.GPU_TEXT_X + 400.f;

                    // Store the button information
                    buttons.push_back(Button(buttonX, buttonY, buttonWidth, buttonHeight, i, false));

                    // Create visual button
                    gui.createButton("Details", button_text_size, buttonWidth, buttonHeight, buttonX, buttonY);
                    buttonY += buttonHeight + buttonSpacing;
                }
            }
            text_body = ss.str();
        }

        // Draw scrollbar if needed
        if (showScrollbar && !isClicked) {
            wc.window->draw(scrollbarBackground);
            wc.window->draw(scrollbarThumb);
        }

        // Create sort buttons
        float xButtonPadding = 140;

        sf::Mouse mouse;
        auto mouseCoords = mouse.getPosition(*(wc.window));
        int mouseX = mouseCoords.x;
        int mouseY = mouseCoords.y;

        // Check for left clicks
        bool leftClick = sf::Mouse::isButtonPressed(sf::Mouse::Left);

        // Category buttons
        if (mouseY >= wc.SORT_Y && mouseY <= wc.SORT_Y + wc.SORT_HEIGHT) {
            // Name button
            if (mouseX >= wc.SORT_X && mouseX <= wc.SORT_X + wc.SORT_WIDTH) {
                if (leftClickReleased) {
                    currentAnswer = 0;
                    dataInfo = loadData(columnToBeSorted, 0, fileName, sortAlgo);
                    isClicked = false;
                }
            }

            // Best resolution button
            if (mouseX >= wc.SORT_X + xButtonPadding && mouseX <= wc.SORT_X + wc.SORT_WIDTH + xButtonPadding) {
                if (leftClickReleased) {
                    currentAnswer = 1;
                    dataInfo = loadData(columnToBeSorted, 1, fileName, sortAlgo);
                    scrollOffset = 0.0f;
                    isClicked = false;
                }
            }

            // Memory button
            if (mouseX >= wc.SORT_X + xButtonPadding * 2 && mouseX <= wc.SORT_X + wc.SORT_WIDTH + xButtonPadding * 2) {
                if (leftClickReleased) {
                    currentAnswer = 2;
                    dataInfo = loadData(columnToBeSorted, 2, fileName, sortAlgo);
                    scrollOffset = 0.0f;
                    isClicked = false;
                }
            }

            // Memory type button
            if (mouseX >= wc.SORT_X + xButtonPadding * 3 && mouseX <= wc.SORT_X + wc.SORT_WIDTH + xButtonPadding * 3) {
                if (leftClickReleased) {
                    currentAnswer = 3;
                    dataInfo = loadData(columnToBeSorted, 3, fileName, sortAlgo);
                    scrollOffset = 0.0f;
                    isClicked = false;
                }
            }

            // Manufacturer button
            if (mouseX >= wc.SORT_X + xButtonPadding * 4 && mouseX <= wc.SORT_X + wc.SORT_WIDTH + xButtonPadding * 4) {
                if (leftClickReleased) {
                    currentAnswer = 4;
                    dataInfo = loadData(columnToBeSorted, 4, fileName, sortAlgo);
                    scrollOffset = 0.0f;
                    isClicked = false;
                }
            }
        }

        // Details buttons
        if (!isClicked && leftClickReleased) {
            for (int i = 0; i < buttons.size(); i++) {
                if (mouseY >= buttons[i].y && mouseY <= buttons[i].y + buttons[i].height) {
                    if (mouseX >= buttons[i].x && mouseX <= buttons[i].x + buttons[i].width) {
                        buttonWasClicked = true;
                        clickedButtonIndex = buttons[i].index;
                        break;
                    }
                }
            }
        }

        // Sorting algorithm buttons
        int xAlgoBtnPadding = 105;
        int yAlgoBtnPadding = 40;

        if (mouseY >= wc.SORT_Y + yAlgoBtnPadding && mouseY <= wc.SORT_Y + wc.SORT_HEIGHT + yAlgoBtnPadding) {
            // Heap sort button
            if (mouseX >= wc.SORT_X + xAlgoBtnPadding && mouseX <= wc.SORT_X + wc.SORT_WIDTH + xAlgoBtnPadding) {
                if (leftClickReleased) {
                    sortAlgo = 0;
                    dataInfo = loadData(columnToBeSorted, currentAnswer, fileName, sortAlgo);
                    isClicked = false;
                }
            }

            xAlgoBtnPadding += 140;

            // Quick sort button
            if (mouseX >= wc.SORT_X + xAlgoBtnPadding && mouseX <= wc.SORT_X + wc.SORT_WIDTH + xAlgoBtnPadding) {
                if (leftClickReleased) {
                    sortAlgo = 1;
                    dataInfo = loadData(columnToBeSorted, currentAnswer, fileName, sortAlgo);
                    isClicked = false;
                }
            }
        }


        // Draw sort buttons
        for (size_t i = 0; i < wc.SORT_CATS.size(); ++i) {
            gui.createButton(wc.SORT_CATS[i], wc.BUTTON_TEXT_SIZE, wc.SORT_WIDTH, wc.SORT_HEIGHT,
                wc.SORT_X + (xButtonPadding * static_cast<float>(i)), wc.SORT_Y);
        }

        // Draw sorting algorithm buttons
        gui.createButton(wc.SORT_ALGOS.at(0), wc.BUTTON_TEXT_SIZE, wc.SORT_WIDTH, wc.SORT_HEIGHT, wc.SORT_X + 105, wc.SORT_Y + 40);
        gui.createButton(wc.SORT_ALGOS.at(1), wc.BUTTON_TEXT_SIZE, wc.SORT_WIDTH, wc.SORT_HEIGHT, wc.SORT_X + 245, wc.SORT_Y + 40);

        // Draw GPU information
        sf::Text data(text_body, wc.FONTS.at("roboto-medium"));
        data.setCharacterSize(wc.GPU_TEXT_SIZE);
        data.setFillColor(wc.GPU_TEXT_COLOR);
        data.setPosition(wc.GPU_TEXT_X, wc.GPU_TEXT_Y);

        // Draw all elements
        wc.window->draw(windowTitle);
        wc.window->draw(sortByText);
        wc.window->draw(separator);
        wc.window->draw(sortAlgosText);
        wc.window->draw(data);

        wc.window->display();
    }

    delete wc.window;
    return 0;
}
