#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>

using namespace std;

// Structure to hold all window characteristics and GUI properties
struct WindowCharacteristics {
    // Window properties
    sf::RenderWindow* window;
    int WIDTH, HEIGHT;

    // Text and font properties
    string TITLE;
    string GUI_TITLE;
    string ROBOTO;
    string ROBOTO_MEDIUM;
    string ROBOTO_BOLD;
    map<string, sf::Font> FONTS;

    // GUI title properties
    float GUI_TITLE_SIZE;
    sf::Color GUI_TITLE_COLOR;
    float GUI_TITLE_X;
    float GUI_TITLE_Y;

    // "Sort by" text properties
    string SORT_BY_TEXT;
    float SORT_BY_SIZE;
    sf::Color SORT_BY_COLOR;
    float SORT_BY_X;
    float SORT_BY_Y;

    // "Sorting algorithms" text properties
    string SORT_ALGOS_TEXT;
    float SORT_ALGOS_SIZE;
    sf::Color SORT_ALGOS_COLOR;
    float SORT_ALGOS_X;
    float SORT_ALGOS_Y;

    // Sort button properties
    float SORT_WIDTH;
    float SORT_HEIGHT;
    sf::Color SORT_COLOR;
    sf::Color SORT_OUTLINE_COLOR;
    float SORT_OUTLINE_THICKNESS;
    float SORT_X;
    float SORT_Y;

    // Sorting categories and algorithms
    vector<string> SORT_CATS;
    vector<string> SORT_ALGOS;

    // Button text properties
    float BUTTON_TEXT_SIZE;
    sf::Color BUTTON_TEXT_COLOR;
    float BUTTON_TEXT_X;
    float BUTTON_TEXT_Y;

    // Chosen algorithm display properties
    string HEAP_ALGO_CHOSEN;
    string QUICK_ALGO_CHOSEN;
    float CHOSEN_ALGO_SIZE;
    sf::Color CHOSEN_ALGO_COLOR;
    float CHOSEN_ALGO_X;
    float CHOSEN_ALGO_Y;

    // Separator line properties
    float SEP_HEIGHT;
    float SEP_X;
    float SEP_Y;
    sf::Color SEP_COLOR = sf::Color::Black;

    // GPU text properties
    float GPU_TEXT_SIZE;
    sf::Color GPU_TEXT_COLOR;
    float GPU_TEXT_X;
    float GPU_TEXT_Y;
};

// Structure representing a button in the GUI
struct Button {
    float x, y, width, height;  // Position and dimensions
    size_t index;               // Index in a collection
    bool isOn;                  // State of the button

    // Constructor
    Button(float _x, float _y, float _width, float _height,
        size_t _index, bool _isOn)
        : x(_x), y(_y), width(_width), height(_height),
        index(_index), isOn(_isOn) {
    }
};

// Main GUI class
class GUI {
private:
    WindowCharacteristics wc;        // Stores all window characteristics
    map<string, sf::Font> loadFonts(); // Loads and returns fonts

public:
    // Constructor with default window size
    GUI(int _WIDTH = 1200, int _HEIGHT = 800);

    // Getter for window characteristics
    struct WindowCharacteristics getWindowChar();

    // Creates a button with given parameters
    void createButton(string text, float text_size, float width, float height, float x, float y);
};

// GUI constructor implementation
GUI::GUI(int _WIDTH, int _HEIGHT) {
    // Initialize window dimensions
    this->wc.WIDTH = _WIDTH;
    this->wc.HEIGHT = _HEIGHT;

    // Set window title and GUI title
    this->wc.TITLE = "Project 3";
    this->wc.GUI_TITLE = "GUI Information";

    // Create SFML window
    this->wc.window = new sf::RenderWindow(sf::VideoMode(wc.WIDTH, wc.HEIGHT), wc.TITLE);

    // Set font paths
    this->wc.ROBOTO = "fonts/roboto-regular.ttf";
    this->wc.ROBOTO_MEDIUM = "fonts/roboto-medium.ttf";
    this->wc.ROBOTO_BOLD = "fonts/roboto-bold.ttf";

    // Configure GUI title properties
    this->wc.GUI_TITLE_SIZE = 35;
    this->wc.GUI_TITLE_COLOR = sf::Color::Black;
    this->wc.GUI_TITLE_X = 120;
    this->wc.GUI_TITLE_Y = 20;

    // Configure "Sort by" text properties
    this->wc.SORT_BY_TEXT = "Sort by: ";
    this->wc.SORT_BY_SIZE = 20;
    this->wc.SORT_BY_COLOR = sf::Color::Black;
    this->wc.SORT_BY_X = 20;
    this->wc.SORT_BY_Y = 100;

    // Configure "Sorting algorithms" text properties
    this->wc.SORT_ALGOS_TEXT = "Sorting algorithms: ";
    this->wc.SORT_ALGOS_SIZE = 20;
    this->wc.SORT_ALGOS_COLOR = sf::Color::Black;
    this->wc.SORT_ALGOS_X = 20;
    this->wc.SORT_ALGOS_Y = 140;

    // Configure sort button properties
    this->wc.SORT_WIDTH = 130;
    this->wc.SORT_HEIGHT = 25;
    this->wc.SORT_COLOR = sf::Color(240, 240, 240);
    this->wc.SORT_OUTLINE_COLOR = sf::Color::Black;
    this->wc.SORT_OUTLINE_THICKNESS = 2;
    this->wc.SORT_X = 105;
    this->wc.SORT_Y = 100;

    // Initialize sorting categories and algorithms
    this->wc.SORT_CATS = { "Name", "Best Resolution", "Memory", "Memory Type", "Manufacturer" };
    this->wc.SORT_ALGOS = { "Heap", "Quick" };

    // Configure button text properties
    this->wc.BUTTON_TEXT_SIZE = 15;
    this->wc.BUTTON_TEXT_COLOR = sf::Color::Black;
    this->wc.BUTTON_TEXT_X = this->wc.SORT_X;
    this->wc.BUTTON_TEXT_Y = this->wc.SORT_Y + 3;

    // Configure separator line properties
    this->wc.SEP_HEIGHT = 1;
    this->wc.SEP_X = 0;
    this->wc.SEP_Y = 175;
    this->wc.SEP_COLOR = sf::Color::Black;

    // Configure GPU text properties
    this->wc.GPU_TEXT_SIZE = 14;
    this->wc.GPU_TEXT_COLOR = sf::Color::Black;
    this->wc.GPU_TEXT_X = 20;
    this->wc.GPU_TEXT_Y = 190;

    // Load fonts
    this->wc.FONTS = loadFonts();
}

// Loads and returns the fonts used in the GUI
map<string, sf::Font> GUI::loadFonts() {
    sf::Font roboto, robotoMedium, robotoBold;

    // Load font files
    roboto.loadFromFile(this->wc.ROBOTO);
    robotoMedium.loadFromFile(this->wc.ROBOTO_MEDIUM);
    robotoBold.loadFromFile(this->wc.ROBOTO_BOLD);

    // Store fonts in map
    this->wc.FONTS["roboto"] = roboto;
    this->wc.FONTS["roboto-medium"] = robotoMedium;
    this->wc.FONTS["roboto-bold"] = robotoBold;

    return this->wc.FONTS;
}

// Creates a button with the specified properties
void GUI::createButton(string text, float text_size, float width, float height, float x, float y) {
    // Create button shape
    sf::RectangleShape sortButton;
    // Create button text
    sf::Text buttonDisc(text, this->wc.FONTS["roboto"]);

    // Configure button appearance
    sortButton.setSize(sf::Vector2f(width, height));
    sortButton.setOutlineThickness(this->wc.SORT_OUTLINE_THICKNESS);
    sortButton.setPosition({ x, y });
    sortButton.setFillColor(this->wc.SORT_COLOR);
    sortButton.setOutlineColor(this->wc.SORT_OUTLINE_COLOR);

    // Text padding within button
    int xTextPadding = 10;
    int yTextPadding = 3;

    // Configure button text
    buttonDisc.setCharacterSize(text_size);
    buttonDisc.setFillColor(this->wc.BUTTON_TEXT_COLOR);
    buttonDisc.setPosition({ x + xTextPadding, y + yTextPadding });

    // Draw button and text to window
    this->wc.window->draw(sortButton);
    this->wc.window->draw(buttonDisc);
}

// Returns the window characteristics structure
struct WindowCharacteristics GUI::getWindowChar() {
    return this->wc;
}
=======
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>

using namespace std;

// Structure to hold all window characteristics and GUI properties
struct WindowCharacteristics {
    // Window properties
    sf::RenderWindow* window;
    int WIDTH, HEIGHT;

    // Text and font properties
    string TITLE;
    string GUI_TITLE;
    string ROBOTO;
    string ROBOTO_MEDIUM;
    string ROBOTO_BOLD;
    map<string, sf::Font> FONTS;

    // GUI title properties
    float GUI_TITLE_SIZE;
    sf::Color GUI_TITLE_COLOR;
    float GUI_TITLE_X;
    float GUI_TITLE_Y;

    // "Sort by" text properties
    string SORT_BY_TEXT;
    float SORT_BY_SIZE;
    sf::Color SORT_BY_COLOR;
    float SORT_BY_X;
    float SORT_BY_Y;

    // "Sorting algorithms" text properties
    string SORT_ALGOS_TEXT;
    float SORT_ALGOS_SIZE;
    sf::Color SORT_ALGOS_COLOR;
    float SORT_ALGOS_X;
    float SORT_ALGOS_Y;

    // Sort button properties
    float SORT_WIDTH;
    float SORT_HEIGHT;
    sf::Color SORT_COLOR;
    sf::Color SORT_OUTLINE_COLOR;
    float SORT_OUTLINE_THICKNESS;
    float SORT_X;
    float SORT_Y;

    // Sorting categories and algorithms
    vector<string> SORT_CATS;
    vector<string> SORT_ALGOS;
    
    // Button text properties
    float BUTTON_TEXT_SIZE;
    sf::Color BUTTON_TEXT_COLOR;
    float BUTTON_TEXT_X;
    float BUTTON_TEXT_Y;

    // Chosen algorithm display properties
    string HEAP_ALGO_CHOSEN;
    string QUICK_ALGO_CHOSEN;
    float CHOSEN_ALGO_SIZE;
    sf::Color CHOSEN_ALGO_COLOR;
    float CHOSEN_ALGO_X;
    float CHOSEN_ALGO_Y;

    // Separator line properties
    float SEP_HEIGHT;
    float SEP_X;
    float SEP_Y;
    sf::Color SEP_COLOR = sf::Color::Black;

    // GPU text properties
    float GPU_TEXT_SIZE;
    sf::Color GPU_TEXT_COLOR;
    float GPU_TEXT_X;
    float GPU_TEXT_Y;
};

// Structure representing a button in the GUI
struct Button {
    float x, y, width, height;  // Position and dimensions
    size_t index;               // Index in a collection
    bool isOn;                  // State of the button

    // Constructor
    Button(float _x, float _y, float _width, float _height,
        size_t _index, bool _isOn)
        : x(_x), y(_y), width(_width), height(_height),
        index(_index), isOn(_isOn) {}
};

// Main GUI class
class GUI {
private:
    WindowCharacteristics wc;        // Stores all window characteristics
    map<string, sf::Font> loadFonts(); // Loads and returns fonts

public:
    // Constructor with default window size
    GUI(int _WIDTH = 1200, int _HEIGHT = 800);
    
    // Getter for window characteristics
    struct WindowCharacteristics getWindowChar();
    
    // Creates a button with given parameters
    void createButton(string text, float text_size, float width, float height, float x, float y);
};

// GUI constructor implementation
GUI::GUI(int _WIDTH, int _HEIGHT) {
    // Initialize window dimensions
    this->wc.WIDTH = _WIDTH;
    this->wc.HEIGHT = _HEIGHT;
    
    // Set window title and GUI title
    this->wc.TITLE = "Project 3";
    this->wc.GUI_TITLE = "GUI Information";

    // Create SFML window
    this->wc.window = new sf::RenderWindow(sf::VideoMode(wc.WIDTH, wc.HEIGHT), wc.TITLE);
    
    // Set font paths
    this->wc.ROBOTO = "fonts/roboto-regular.ttf";
    this->wc.ROBOTO_MEDIUM = "fonts/roboto-medium.ttf";
    this->wc.ROBOTO_BOLD = "fonts/roboto-bold.ttf";

    // Configure GUI title properties
    this->wc.GUI_TITLE_SIZE = 35;
    this->wc.GUI_TITLE_COLOR = sf::Color::Black;
    this->wc.GUI_TITLE_X = 120;
    this->wc.GUI_TITLE_Y = 20;

    // Configure "Sort by" text properties
    this->wc.SORT_BY_TEXT = "Sort by: ";
    this->wc.SORT_BY_SIZE = 20;
    this->wc.SORT_BY_COLOR = sf::Color::Black;
    this->wc.SORT_BY_X = 20;
    this->wc.SORT_BY_Y = 100;

    // Configure "Sorting algorithms" text properties
    this->wc.SORT_ALGOS_TEXT = "Sorting algorithms: ";
    this->wc.SORT_ALGOS_SIZE = 20;
    this->wc.SORT_ALGOS_COLOR = sf::Color::Black;
    this->wc.SORT_ALGOS_X = 20;
    this->wc.SORT_ALGOS_Y = 140;

    // Configure sort button properties
    this->wc.SORT_WIDTH = 130;
    this->wc.SORT_HEIGHT = 25;
    this->wc.SORT_COLOR = sf::Color(240, 240, 240);
    this->wc.SORT_OUTLINE_COLOR = sf::Color::Black;
    this->wc.SORT_OUTLINE_THICKNESS = 2;
    this->wc.SORT_X = 105;
    this->wc.SORT_Y = 100;

    // Initialize sorting categories and algorithms
    this->wc.SORT_CATS = { "Name", "Best Resolution", "Memory", "Memory Type", "Manufacturer" };
    this->wc.SORT_ALGOS = { "Heap", "Quick" };
    
    // Configure button text properties
    this->wc.BUTTON_TEXT_SIZE = 15;
    this->wc.BUTTON_TEXT_COLOR = sf::Color::Black;
    this->wc.BUTTON_TEXT_X = this->wc.SORT_X;
    this->wc.BUTTON_TEXT_Y = this->wc.SORT_Y + 3;

    // Configure separator line properties
    this->wc.SEP_HEIGHT = 1;
    this->wc.SEP_X = 0;
    this->wc.SEP_Y = 175;
    this->wc.SEP_COLOR = sf::Color::Black;

    // Configure GPU text properties
    this->wc.GPU_TEXT_SIZE = 14;
    this->wc.GPU_TEXT_COLOR = sf::Color::Black;
    this->wc.GPU_TEXT_X = 20;
    this->wc.GPU_TEXT_Y = 190;

    // Load fonts
    this->wc.FONTS = loadFonts();
}

// Loads and returns the fonts used in the GUI
map<string, sf::Font> GUI::loadFonts() {
    sf::Font roboto, robotoMedium, robotoBold;
    
    // Load font files
    roboto.loadFromFile(this->wc.ROBOTO);
    robotoMedium.loadFromFile(this->wc.ROBOTO_MEDIUM);
    robotoBold.loadFromFile(this->wc.ROBOTO_BOLD);

    // Store fonts in map
    this->wc.FONTS["roboto"] = roboto;
    this->wc.FONTS["roboto-medium"] = robotoMedium;
    this->wc.FONTS["roboto-bold"] = robotoBold;

    return this->wc.FONTS;
}

// Creates a button with the specified properties
void GUI::createButton(string text, float text_size, float width, float height, float x, float y) {
    // Create button shape
    sf::RectangleShape sortButton;
    // Create button text
    sf::Text buttonDisc(text, this->wc.FONTS["roboto"]);

    // Configure button appearance
    sortButton.setSize(sf::Vector2f(width, height));
    sortButton.setOutlineThickness(this->wc.SORT_OUTLINE_THICKNESS);
    sortButton.setPosition({ x, y });
    sortButton.setFillColor(this->wc.SORT_COLOR);
    sortButton.setOutlineColor(this->wc.SORT_OUTLINE_COLOR);

    // Text padding within button
    int xTextPadding = 10;
    int yTextPadding = 3;

    // Configure button text
    buttonDisc.setCharacterSize(text_size);
    buttonDisc.setFillColor(this->wc.BUTTON_TEXT_COLOR);
    buttonDisc.setPosition({ x + xTextPadding, y + yTextPadding });

    // Draw button and text to window
    this->wc.window->draw(sortButton);
    this->wc.window->draw(buttonDisc);
}

// Returns the window characteristics structure
struct WindowCharacteristics GUI::getWindowChar() {
    return this->wc;
}
