#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <boost/algorithm/string.hpp>

using namespace std;

class CSVReader
{
	string fileName;
	string delimeter;
public:
	CSVReader(string filename, string delm = ",") :
			fileName(filename), delimeter(delm)
	{ }
	// Function to fetch data from a CSV File
	vector<vector<string> > getData();
};

vector<vector<string> > CSVReader::getData()
{
	ifstream file(fileName);
	vector<vector<string> > dataList;
	string line = "";
	// Iterate through each line and split the content using delimeter
	while (getline(file, line))
	{
		vector<string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
		dataList.push_back(vec);
	}
	// Close the File
	file.close();
	return dataList;
}

void read_records() { 

	// File pointer 
	fstream moneyData; 

	// Open an existing file 
	moneyData.open("moneydata.csv", ios::in); 

	// Get the roll number 
	// of which the data is required 
	int year = 0, year2, count = 0;
	string month, month2; 
	cout << "Enter when you want to see: ";
	cin >> month >> year; 
	cout << endl;

	// Read the Data from the file 
	// as String Vector 
	vector<string> row; 
	string line, word, temp; 
	
	int rows=0;
	ifstream file("moneydata.csv");
	while (getline(file, line)){ rows++; }


	for (int i = 0; i < rows; i++){
		row.clear();
		getline(moneyData, line); 
		stringstream s(line); 
		while (getline(s, word, ',')) { 
			row.push_back(word); 
		} 
		
		month2 = row[0];
    	year2 = stoi(row[2]); 
	
		if (year2 == year && month2 == month) {
			if (stoi(row[3]) > 0){
				cout << "\033[32m---------------------------------------------\n|\n";
			}
			if (stoi(row[3]) < 0){
				cout << "\033[31m---------------------------------------------\n|\n";
			}
			cout << "| Date: " << row[0] << " " << row[1] << ", " << row[2] << "\n";
			if (stoi(row[3]) > 0){
				cout << "| Money Change: +$" << stof(row[3]) << " to your account\n";
			}
			if (stoi(row[3]) < 0){
				cout << "| Money Change: -$" << stof(row[3]) - 2*(stof(row[3])) << " in your " << row[4] << " section\n";
			}
			cout << "| Reason: " << row[5] << "\n|\n";
			cout << "---------------------------------------------\033[0m\n";

			count = 1;
		}
	}
	
	if (count == 0) {
		cout << "\033[33m----------------------------------------------\n";
		cout <<         "|                                            |\n";
		cout <<         "|              Record not found              |\n";
		cout <<         "|                                            |\n";
		cout <<         "----------------------------------------------\033[0m\n";
	}

	cout << endl;

	return;
} 

void new_record() { 
	// File pointer 
	fstream moneyData; 
	ofstream newMoneyData;
	fstream circleData;

	// Open an existing file 
	moneyData.open("moneydata.csv"); 
	newMoneyData.open("newmoneydata.csv");
	circleData.open("moneycircle.csv");

	int day, year; 
	float money;
	vector<float> circle = {};
	CSVReader reader("moneycircle.csv");
    vector<vector<string> > dataList = reader.getData();
    for(vector<string> vec : dataList)
	{
		for(string data : vec)
		{
			circle.push_back(stof(data));
		}
	}
	
	// Read the Data from the file 
	// as String Vector 
	vector<string> row; 
	string line, word, temp, month, circleSection = "all", why;

	cout << "\033[36mDate: ";
	cin >> month >> day >> year;
	cout << "Money change: ";
	cin >> money;
	if (money < 0){
		cout << "which section in the circle? ";
		cin >> circleSection;
		
		while (circleSection != "Spend" && circleSection != "spend" && circleSection != "Share" && circleSection != "share"){
			cout << "which section in the circle? ";
			cin >> circleSection;
		}

        if (circleSection == "Spend" || circleSection == "spend"){
            circle[0] = circle[0] + money;
		}
		if (circleSection == "Share" || circleSection == "share"){
			circle[2] = circle[2] + money;
		}
	}
	else if (money > 0){
	    circle[0] = circle[0] + (money/2);
		circle[1] = circle[1] + (money/4);
	    circle[2] = circle[2] + (money/4);
	}

	cout << "Why did the money change? ";
	cin >> why;
	cout << "\033[0m" << endl;

	int rows = 0;
	ifstream file("moneydata.csv");
	while (getline(file, line)){ rows++; }
	
	row.push_back(month);
	row.push_back(to_string(day));
	row.push_back(to_string(year));
	row.push_back(to_string(money));
	row.push_back(circleSection);
	row.push_back(why);

	newMoneyData << row[0] << "," << row[1] << "," << row[2] << "," << row[3] << "," << row[4] << "," << row[5] << "\n";

	for (int i = 0; i < rows; i++){
		row.clear();
		getline(moneyData, line); 
		stringstream s(line); 
		while (getline(s, word, ',')) { 
			row.push_back(word); 
		}
		newMoneyData << row[0] << "," << row[1] << "," << row[2] << "," << row[3] << "," << row[4] << "," << row[5] << "\n";

	}

    circleData << circle[0] << "\n" << circle[1] << "\n" << circle[2] << "\n" << 0;

    circleData.close();
    moneyData.close();
	newMoneyData.close();

	remove("moneydata.csv");

	// renaming the updated file with the existing file name
	rename("newmoneydata.csv", "moneydata.csv");

	return;
} 

void update_records() {
	// File pointer
	fstream fin, fout;

	// Open an existing record
	fin.open("moneydata.csv", ios::in);

	// Create a new file to store updated data
	fout.open("moneydataNew.csv", ios::out);

	int day, year, inp2 = 0, inp3 = 0, marks, count = 0, i;
	char sub;
	int index;
	string line, word, month, inp1, new_marks;
	vector<string> row;

	// Get the roll number from the user
	cout << "Enter the date you want to update: ";
	cin >> month >> day >> year;

	// Get the data to be updated
	cout << "Enter the subject "
		<< "to be updated(M/D/Y/$/S/?): ";
	cin >> sub;

	if (sub == 'm' || sub == 'M'){index = 0;}
	else if (sub == 'd' || sub == 'D'){index = 1;}
	else if (sub == 'y' || sub == 'Y'){index = 2;}
	else if (sub == '$'){index = 3;}
	else if (sub == 's' || sub == 'S'){index = 4;}
    else if (sub = '?'){index = 5;}
	else {
		cout << "Wrong choice. Enter again\n";
		update_records();
	}

	// Get the new marks
	cout << "Enter new marks: ";
	cin >> new_marks;

	int rows=0;
	ifstream file("moneydata.csv");
	while (getline(file, line)){ rows++; }

	row.clear();
	getline(fin, line); 
	stringstream s(line); 
	while (getline(s, word, ',')) { 
		row.push_back(word); 
	} 

	inp1 = row[0];
	inp2 = stoi(row[1]);
	inp3 = stoi(row[2]);
	int row_size = row.size();
	
	// Traverse the file
	while (!fin.eof()) {

		row.clear();

		getline(fin, line);
		stringstream s(line);

		while (getline(s, word, ',')) {
			row.push_back(word);
		}

		inp1 = row[0];
		inp2 = stoi(row[1]);
		inp3 = stoi(row[2]);
		int row_size = row.size();

		if (inp1 == month && inp2 == day && inp3 == year) {
			count = 1;
			cout << endl;
			// stringstream convert;

			// sending a number as a stream into output string
			// convert >> new_marks;

			// the str() converts number into string
			row[index] = new_marks;

			if (!fin.eof()) {
				for (i = 0; i < row_size - 1; i++) {
					fout << row[i] << ",";
				}

				fout << row[row_size - 1] << "\n";
			}

		}
		else {
			if (!fin.eof()) {
				for (i = 0; i < row_size - 1; i++) {
					fout << row[i] << ",";
				}

				fout << row[row_size - 1] << "\n";
			}
		}
		if (fin.eof())
			break;
	}

	cout << endl;

	if (count == 0)
		cout << "Record not found\n\n";

	fin.close();
	fout.close();

	// removing the existing file
	remove("moneydata.csv");

	// renaming the updated file with the existing file name
	rename("moneydataNew.csv", "moneydata.csv");

	return;
}

int main(){
    system("clear");
    
    string newMonth;
    int newDay, newYear, newMoney;
	int spend, save, share;
    string why;
    string what;

    string monthIndex;
    int dayIndex;
    int yearIndex;
    string yesOrNo;
    int count;	
		
	while (what != "done"){
		cout << "New, records, or circle?\nType 'done' If you want to exit. ";
		cin >> what;

		while (what != "records" && what != "circle" && what != "new" && what != "done" && what != "clear"){
			cout << "Can't do that.\n";
			cout << "New, records, or circle? (or done). ";
			cin >> what;
		}

		fstream moneyData;
		fstream moneyRecords;
		fstream moneyCircle;

		moneyData.open ("moneydata.csv");
		moneyRecords.open ("moneyrecords.csv");
		moneyCircle.open("moneycircle.csv");

		cout << endl;
		
		if (what == "new"){
			new_record();
		}
		
		//done.
		else if (what == "circle"){
			vector<float> circle = {};
			// Creating an object of CSVWriter
			CSVReader reader("moneycircle.csv");
			// Get the data from CSV File
			vector<vector<string> > dataList = reader.getData();
			// Print the content of row by row on screen
			for(vector<string> vec : dataList)
			{
				for(string data : vec)
				{
					circle.push_back(stof(data));
				}
			}

			cout << "\033[1;32mSpend: $" << circle[0] << endl;
			cout << "\033[1;31mSave: $" << circle[1] << endl;
			cout << "\033[1;34mShare: $" << circle[2] << endl << "\033[0m\n";

		}

		else if (what == "done"){
			return 0;
		}
		
		else if (what == "clear"){
			system("clear");
		}

		//done.
		else {
			read_records();
		}
	}
}
