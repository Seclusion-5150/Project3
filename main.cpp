#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <map>
#include <list>
using namespace std;

// Constants for category selection
const vector<string> CATEGORIES = {
    "Name", "Best_Resolution", "Memory", "Memory_Type", "Manufacturer"
};

// Heap Sort implementation
void heapify(vector<list<string>>& data, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && data[left].front() > data[largest].front())
        largest = left;

    if (right < n && data[right].front() > data[largest].front())
        largest = right;

    if (largest != i) {
        swap(data[i], data[largest]);
        heapify(data, n, largest);
    }
}

void heapSort(vector<list<string>>& data) {
    int n = data.size();

    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(data, n, i);

    // Extract elements
    for (int i = n - 1; i > 0; i--) {
        swap(data[0], data[i]);
        heapify(data, i, 0);
    }
}


// Properly parse CSV, handling quoted fields
vector<string> parseCSVLine(const string& line) {
    vector<string> result;
    bool inQuotes = false;
    string field;
    
    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            result.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    result.push_back(field); // Add the last field
    
    return result;
}


void parse_column(vector<vector<string>>& data, vector<list<string>> &column_data,  int column)
{
	//create a vector of lists where the category we want to sort by is the first in the list
	
	int col_idx = 0;
    for (const auto& row : data) 
	{
		//iterate through the row and push the sorting category to the front
		list<string> augmented_row;
		
		for(int i = 0; i < data[col_idx].size();i++)
		{
			if(i != column) augmented_row.push_back(row[i]);
			else augmented_row.push_front(row[i]);
		}
		
		column_data.emplace_back(augmented_row.begin(), augmented_row.end());

		col_idx++;		
	}

}
void load_data(vector<list<string>> &column_to_be_sorted, int answer, string file_name)
{
	vector<vector<string>> data;
    ifstream file(file_name);
    
    map<string, int> category_idx;
    
    string line;
    getline(file, line);
    
    // Parse header row properly
    vector<string> headers = parseCSVLine(line);
    
    for (size_t i = 0; i < headers.size(); i++) {
        // Trim whitespace if needed
        string& header = headers[i];
        header.erase(0, header.find_first_not_of(" \t\r\n\""));
        header.erase(header.find_last_not_of(" \t\r\n\"") + 1);
        
        category_idx[header] = i;
    }
    
    string main_node_idx = "";
    switch(answer)
    {
        case 0:
            main_node_idx = "Name";
            break;
        case 1:
            main_node_idx = "Best_Resolution";
            break;
        case 2:
            main_node_idx = "Memory";
            break;
        case 3:
            main_node_idx = "Memory_Type";
            break;
        case 4:
            main_node_idx = "Manufacturer";
            break;
    };
    
    
    // Read and parse data rows properly
    while(getline(file, line))
    {
        vector<string> row = parseCSVLine(line);
        data.push_back(row);
    }
    int column_index = category_idx[main_node_idx];
	parse_column(data, column_to_be_sorted, column_index);

}

int main()
{
  
    string file_name = "All_GPUs.csv";
	vector<list<string>> column_to_be_sorted;
	load_data(column_to_be_sorted, 0, file_name);
	
    return 0;
}
