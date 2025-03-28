#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <map>
#include <list>
using namespace std;

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

int main()
{
    vector<vector<string>> data;
    string file_name = "All_GPUs.csv";
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
    
	  //placeholder until we can get the GUI working
    int answer;
    
    cout << "Which category do you want to sort by: Name(0), Best Resolution(1), Memory(2), Memory Type(3), or Manufacturer(4)?" << endl;
    cin >> answer;
    
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
    
    // Print the selected column
    int column_index = category_idx[main_node_idx];
    cout << "Printing column: " << main_node_idx << " (index " << column_index << ")" << endl;
    
	  vector<list<string>> column_to_be_sorted;
	  parse_column(data, column_to_be_sorted, column_index);

	  for(auto item :  column_to_be_sorted[0]) cout << item << endl;
    
    return 0;
}