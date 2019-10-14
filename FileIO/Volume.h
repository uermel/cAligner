#ifndef VOLUME_H
#define VOLUME_H

#include "FileIO.h"
#include "ImageBase.h"

using namespace std;

class Volume : public ImageBase
{
public:
	Volume(string aFileName, void(*readStatusCallback)(FileReader::FileReaderStatus) = NULL);
	~Volume();

	virtual ImageType_enum GetImageType();
	virtual FileType_enum GetFileType();
	virtual DataType_enum GetFileDataType();
	virtual bool NeedsFlipOnYAxis();

	static bool CanReadFile(string aFilename);
	static bool CanReadFile(string aFilename, FileType_enum& fileType);
	static bool CanReadFile(string aFilename, FileType_enum& fileType, int& aWidth, int& aHeight, int& aDepth, DataType_enum& aDataType);

	void* GetData();
	uint GetWidth();
	uint GetHeight();
	uint GetDepth();
	float GetPixelSize();

	static Volume* CreateInstance(string aFileName, void(*readStatusCallback)(FileReader::FileReaderStatus) = NULL);
private:

	FileType_enum _fileType;
	void* _file;

};

#endif