#include "info.hpp"


std::map<std::string, std::string> LoadInfo(std::string path, int& result)
{
	result = 0;
	std::ifstream infofile;
	infofile.open(path.c_str());
	if(!infofile.is_open())
	{
		std::cout << "Error: could not load info file. Package malformed\n";
		result = 1;
		//return *new std::map<std::string, std::string>;
	}
	std::map<std::string, std::string> vals;
	while(infofile.good())
	{
		std::string line;
		std::getline(infofile, line);
		if(line == "EOF")
			break;
		int comment = line.find("#");
		if(comment != std::string::npos)
		{
			std::string line = line.substr(comment);
		}
		std::string field;
		int equals = line.find("=");
		field = line.substr(0, equals);
		std::string value = line.substr(equals+1);
		//trim the spaces
		field.erase(std::remove_if(field.begin(), field.end(), ::isspace), field.end());
		value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
		vals[field] = value;
	}
	return vals;
}
