#ifndef SIMPLEFILELIST_H
#define SIMPLEFILELIST_H


#include "../Basics/Default.h"
#include <string>
#include <vector>

class SimpleFileList
{
private:
	std::string _filename;
	std::vector<std::string> _entries;

public:
	SimpleFileList(std::string aFilename);
	std::vector<std::string> GetEntries();
};

#endif