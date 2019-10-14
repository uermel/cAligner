#ifndef MOVIESTACK_H
#define MOVIESTACK_H

#include "FileIO.h"
#include "ImageBase.h"

using namespace std;

class MovieStack : public ImageBase
{
public:
	MovieStack(string aFileName, void(*readStatusCallback)(FileReader::FileReaderStatus) = NULL);
	~MovieStack();

	virtual ImageType_enum GetImageType();
	virtual FileType_enum GetFileType();
	virtual DataType_enum GetFileDataType();
	virtual bool NeedsFlipOnYAxis();

	static bool CanReadFile(string aFilename);
	static bool CanReadFile(string aFilename, FileType_enum& fileType);
	static bool CanReadFile(string aFilename, FileType_enum& fileType, int& aWidth, int& aHeight, int& aFrameCount, DataType_enum& aDataType);

	void* GetData(size_t idx);
	uint GetWidth();
	uint GetHeight();
	uint GetImageCount();
	float GetPixelSize();

	static MovieStack* CreateInstance(string aFileName, void(*readStatusCallback)(FileReader::FileReaderStatus) = NULL);
private:

	FileType_enum _fileType;
	void* _file;

};

#endif