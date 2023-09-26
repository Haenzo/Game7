
#include <iostream>
#include <string>
#include <type_traits>
#include <functional>
#include "math.h"
#include "stdlib.h"
#include <map>
#include "csv_tool.h"

using namespace std;

using namespace csv_tool;

int main(int argc, char *argv[])
{
    /*
    Programmer: Reginald J. Armond
    Purpose: HW for Game7
    Comment: lol, trivial but ~367 lines, didn't want to add to many unrequested features
    */
    string option; 
    string value;
    string display;
    bool show_help = (argc==1);
    bool show_labels = false;
    bool quiet = false;
    string filename;
    size_t precision = 18;
    
    for(size_t count=1; count<argc; ++count)
    {
	option = argv[count]; 
	value.clear();
	if((count+1) < argc)
	{
		value = argv[count+1];
	}
	// apply options 
	if("--help" == option)
	{
		show_help = true;
		break;
	}
	if("--f" == option)
		filename = value;
	if("--labels" == option)
		show_labels=true;
	if("--precision" == option)
		precision = (size_t) std::stoi(value);
	if("--quiet" == option)
		quiet = true;
	//

	if(value.empty())
		++count;
    }
    
    if(filename.empty())
    	show_help = true;
    
    if(!quiet)
    {
    	cout << "Option Summary: " << endl;
    	cout << "  filename: " << filename << endl;
    	cout << "  show labels: " << ((show_labels) ? "Yes":"No") << endl;
    	if(precision)
    		cout << "  precision: " << precision << endl;
    	cout << "  by R.J.A - 20230926" << endl;
    }
    
    if(show_help)
    {
    	display ="Valid parameters:";
	display+="\n\t--help                    (this text)";
	display+="\n\t--f <file path and name>  (csv file to use)";
	display+="\n\t--labels                  (add label columns)";
	display+="\n\t--precision <digits>      (digits after decimal point)";
	display+="\n\t--quiet                   (don't show option summary)";
	display+="\n\nExamples:";
	display+="\n\tcsv_tool --f random_numbers.csv --quiet";
	display+="\n\tcsv_tool --f random_numbers.csv --labels";
	display+="\n\tcsv_tool --f random_numbers.csv --labels --quiet --precision 8";
    }
    
    if(!display.empty())
    {
    	cout << display << endl;
    	return 0;
    }

    
    csv_class csv;
    csv.showLabelColumn(show_labels);
    csv.setPrecision(precision);
    if(csv.loadCSV(filename))
    {
    	size_t column_count = csv.getColumnCount();
    	for(size_t count=0; count<column_count; ++count)
    		cout << "[" << csv.getColumnStats(count) << "]" << endl;
    }
    
}
