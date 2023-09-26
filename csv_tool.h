#pragma once
#include <iostream>
#include <string>
#include <type_traits>
#include <functional>
#include "math.h"
#include "stdlib.h"
#include <map>
using namespace std;

namespace csv_tool
{
	
	struct csv_column {
  		vector<double> items;
		string name;
  		};
  		
  	
  	
	class csv_class {
		public:
			csv_class();
			~csv_class();
			
			bool loadCSV(const string file_name);
			size_t getColumnCount(); 
			string getColumnStats(size_t column) 		const;
			bool   getColumnStats(const csv_column& column,size_t& count,double& mean,double& std,double& min,double& max) const;
			void showLabelColumn(bool b);
			void setPrecision(size_t precision);
		private:
			enum row_read_state {
				whitespace,in_quotes,in_number
			};
			
			char m_quote {'"'};
			char m_comma {','};
			size_t m_precision {2};
			bool m_show_label_column{false};
			map<size_t,csv_column> m_columns;
			bool addItem(size_t column_number,string value); 
			bool addRowItems(const string row);		
		};
}


