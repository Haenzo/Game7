#include <iostream>
#include <string>
#include <type_traits>
#include <functional>
#include <fstream>
#include <sstream>
#include "math.h"
#include "stdlib.h"
#include "csv_tool.h"

using namespace std;

namespace csv_tool {

auto square = [](double mean, double item) {
		double d = (item - mean) * (item - mean);
		return d;
	};

	double stddev (double mean, const csv_column& column) {
  		double total {0.0};
  		if(column.items.size()<2)
	  		return 0;

  		for(auto it:column.items)
	  		total += apply(square,tuple(mean,it));
  		return sqrt(total/((double) (column.items.size()-1)));
	};
	
	
  	
  	bool is_data_char(char c)
  	{
  		if(c>='0' && c<='9')
  			return true;
  		if('.' == c)
  			return true;
  			
  		if('_' == c)
  			return true;
  		if((c & 0x7F)>='a' && (c & 0x7F)<='z')
  			return true;
  		return false;
  	}
 }

csv_tool::csv_class::csv_class() {
	
}

csv_tool::csv_class::~csv_class() {
}


bool csv_tool::csv_class::addItem(size_t column_number,string value) {
	if(value.empty())
		return false;
		
	auto itr = m_columns.find(column_number);
	if(m_columns.end() == itr)
	{
		csv_column item;
		item.name = value;
		m_columns.insert(make_pair(column_number,item));
		return true;
	}
	
	m_columns[column_number].items.push_back(std::stod(value));

	return true;
}

bool csv_tool::csv_class::addRowItems(const string row)
{


	size_t column=0;
	string value;
	row_read_state rrs=whitespace;
	for(int c=0;c<row.size();++c) {
		switch(rrs) {
			case whitespace:
				{
					if(row[c] == m_quote)
						rrs = in_quotes;
					else
					if(row[c] == m_comma)
						++column;
					else
					if(csv_tool::is_data_char(row[c]))
					{
						rrs = in_number;
						--c;
					}
					break;
				}
			case in_number:
				{
					if(row[c] == m_comma)
					{
						addItem(column,value);
						value.clear();
						++column;
						rrs = whitespace;
						break;
					}
					else
					if(is_data_char(row[c]))
					{
						value.push_back(row[c]);
					}
					
					break;
				}
			case in_quotes:
				{
					if(row[c] == m_quote)
					{
						rrs = whitespace;
						addItem(column,value);
						
						value.clear();
						break;
					}
					value.push_back(row[c]);
					break;
				}
		}
	}
	return true;
}

void csv_tool::csv_class::showLabelColumn(bool b)
{
	m_show_label_column = b;
}

void csv_tool::csv_class::setPrecision(size_t precision)
{
	m_precision = precision;
}

bool csv_tool::csv_class::loadCSV(const string file_name) {
	fstream file(file_name, std::ios::in);
	
	if(!file.is_open())
		return false;
	string row;
	
	while(getline(file,row)) 
		addRowItems(row);
	
	
	file.close();
	
	return true;		
} 

size_t csv_tool::csv_class::getColumnCount() {
	return m_columns.size();	
}

bool csv_tool::csv_class::getColumnStats(const csv_column& column,size_t& count,double& mean,double& std,double& min,double& max) const {
	
		if(column.items.size()<2)
	  		return false;
	  		
		double total{0.0};
		double double_max = 10000.0; // (uint64_t) ~0;
		
		double _min = (double) double_max;
		double _max = (double) -double_max;
   		for(auto it:column.items)
   		{
	  		total += it;
	  		_min = (it < _min) ? it:_min;
	  		_max = (it > _max) ? it:_max;
	  	}
	  	
	  	count = column.items.size();
	  	mean  = total / ((double) column.items.size());
	  	min   = _min;
	  	max   = _max;
	  	std   = stddev(mean,column);
		return true;
	}

string csv_tool::csv_class::getColumnStats(size_t column) const
{
	stringstream ss;
	auto itr = m_columns.find(column);
	if(m_columns.end() == itr)
		return "";
	
	size_t count{0};
	double mean{0.0}, std{0.0},min{0.0},max{0.0};
	
	if(!getColumnStats(itr->second,count,mean,std,min,max))
	{
		return "";
	}
	
	if(m_show_label_column)
		ss << itr->second.name << ",";
	ss << count << ",";
	
	if(m_precision) 
		ss.precision(m_precision);
		
	ss << mean << "," << std << "," << min << "," << max;
	
	return ss.str();
}


