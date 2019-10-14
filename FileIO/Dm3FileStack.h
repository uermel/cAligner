#ifndef DM3FILESTACK_H
#define DM3FILESTACK_H

#include "../../Basics/Default.h"
#include "Dm3File.h"
#include "FileReader.h"

//!  Dm3FileStack represents a tilt series in gatan's dm3 format. 
/*!
	Dm3FileStack reads all projections with the same name base into one volume in memory.
	The provided file name must end with file index "0000".
	\author Michael Kunz
	\date   September 2011
	\version 1.0
*/
class Dm3FileStack : public FileReader
{
private:
	bool fexists(std::string filename);
	std::string GetStringFromInt(int aInt);
	std::string GetFileNameFromIndex(int aIndex, std::string aFileName);
	int CountFilesInStack(std::string aFileName);
	int _fileCount;

	std::vector<Dm3File*> _dm3files;
	
public:
	Dm3FileStack(std::string aFileName);
	virtual ~Dm3FileStack();
	
	bool OpenAndRead();
	DataType_enum GetDataType();
	void ReadHeaderInfo();
	void WriteInfoToHeader();
	char* GetData();

};

#endif