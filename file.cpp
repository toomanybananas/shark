#include "file.hpp"

bool FileExists(std::string path)
{
	std::ifstream f;
	f.open(path.c_str());
	if(f.is_open())
	{
		return true;
	}
	return false;
}
