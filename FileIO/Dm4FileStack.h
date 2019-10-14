#ifndef DM4FILESTACK_H
#define DM4FILESTACK_H

#include "../../Basics/Default.h"
#include "Dm4File.h"
#include "FileReader.h"

//!  Dm4FileStack represents a tilt series in gatan's dm4 format.
/*!
	Dm4FileStack reads all projections with the same name base into one volume in memory.
	The provided file name must end with file index "0000".
	\author Michael Kunz
	\date   September 2011
	\version 1.0
*/
class Dm4FileStack : public FileReader
{
private:
	bool fexists(std::string filename);
	std::string GetStringFromInt(int aInt);
	std::string GetFileNameFromIndex(int aIndex, std::string aFileName);
	int CountFilesInStack(std::string aFileName);
	int _fileCount;
	int _firstIndex;

	std::vector<Dm4File*> _dm4files;

public:
	Dm4FileStack(std::string aFileName);
	virtual ~Dm4FileStack();

	bool OpenAndRead();
	DataType_enum GetDataType();
	void ReadHeaderInfo();
	void WriteInfoToHeader();
	char* GetData();

};

#endif
