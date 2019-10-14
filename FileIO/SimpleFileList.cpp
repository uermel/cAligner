#include "SimpleFileList.h"
#include <fstream>
#include <iostream>

using namespace std;

SimpleFileList::SimpleFileList(std::string aFilename)
{
	_filename = aFilename;

	ifstream fstream(_filename);

	string line;
	vector<string> lines;

	while (getline(fstream, line))
	{
		//ignore empty lines
		if (line.length() > 3)
		{
			lines.push_back(line);
		}
	}
}

std::vector<std::string> SimpleFileList::GetEntries()
{
	return _entries;
}
