#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

void print_data(vector<vector<string>> data)
{
	for(unsigned int i = 0; i < 1;i++)
	{
		for(unsigned int j = 0; j < data[i].size();j++)
		{
			cout << data[i][j] << " ";
		}
		cout << endl;
	}
}

int main()
{

	vector<vector<string>> data;
	string file_name = "All_GPUs.csv";
	ifstream file(file_name);
	
	string line;
	while(getline(file, line))
	{
		vector<string> row;
		stringstream ss(line);
		string cell;
		while(getline(ss, cell, ','))
		{
			row.push_back(cell);
		}
		data.push_back(row);
	}
	
	print_data(data);
	cout << data[0].size() << endl;
	return 0;
}
