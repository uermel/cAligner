#ifndef MDOCFILE_H
#define MDOCFILE_H

#include "../Basics/Default.h"
#include <string>
#include <vector>

struct MDocEntry
{
public:
	int ZIndex;
	float TiltAngle;
	std::string SubFramePath;
	float PixelSpacing;
};

class MDocFile
{
private:
	std::string _filename;
	std::vector<MDocEntry> _entries;

	std::string CropValue(std::string text);

public:
	MDocFile(std::string filename);
	std::vector<MDocEntry> GetEntries();


	

};


#endif
