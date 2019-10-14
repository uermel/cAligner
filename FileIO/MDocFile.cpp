#include "MDocFile.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>


using namespace std;

string MDocFile::CropValue(string text)
{
	text.erase(std::remove(text.begin(), text.end(), ']'), text.end());
	text.erase(std::remove(text.begin(), text.end(), '['), text.end());

	int pos = text.find_last_of(" = ");
	text = text.substr(pos+1);
	return text;
}

vector<MDocEntry> MDocFile::GetEntries()
{
	return _entries;
}

MDocFile::MDocFile(string filename)
{
	_filename = filename;

	ifstream fstream(filename);

	string line;
	vector<string> lines;

	while (getline(fstream, line))
	{
		lines.push_back(line);
	}

	for (size_t i = 0; i < lines.size(); i++)
	{
		line = lines[i];
		if (line.find("[ZValue") == 0)
		{
			MDocEntry e;
			stringstream ss1(CropValue(line));
			ss1 >> e.ZIndex;

			for (size_t j = i+1; j < lines.size(); j++)
			{
				line = lines[j];
				if (line.find("[ZValue") == 0)
				{
					break;
				}
				int test = line.find("TiltAngle");
				if (line.find("TiltAngle") == 0)
				{
					string temp = CropValue(line);
					replace(temp.begin(), temp.end(), ',', '.');
					stringstream ss(temp);
					ss >> e.TiltAngle;
				}
				if (line.find("PixelSpacing") == 0)
				{
					string temp = CropValue(line);
					replace(temp.begin(), temp.end(), ',', '.');
					stringstream ss(temp);
					ss >> e.PixelSpacing;
				}
				if (line.find("SubFramePath") == 0)
				{
					string temp = CropValue(line);
					replace(temp.begin(), temp.end(), '\\', '/');
					int pos = temp.find_last_of('/') + 1;
					e.SubFramePath = temp.substr(pos);
				}
			}
			_entries.push_back(e);
		}
	}
	sort(_entries.begin(), _entries.end(), [](const MDocEntry& m1, const MDocEntry& m2) {
		return m1.TiltAngle < m2.TiltAngle;
	});
}