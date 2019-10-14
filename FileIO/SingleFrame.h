#ifndef SINGLEFRAME_H
#define SINGLEFRAME_H

#include "FileIO.h"
#include "ImageBase.h"

using namespace std;

class SingleFrame: public ImageBase
{
public:
	SingleFrame(string aFileName, void(*readStatusCallback)(FileReader::FileReaderStatus) = NULL);
	~SingleFrame();

	virtual ImageType_enum GetImageType();
	virtual FileType_enum GetFileType();
	virtual DataType_enum GetFileDataType();
	virtual bool NeedsFlipOnYAxis();

	static bool CanReadFile(string aFilename);
	static bool CanReadFile(string aFilename, FileType_enum& fileType);
	static bool CanReadFile(string aFilename, FileType_enum& fileType, int& aWidth, int& aHeight, DataType_enum& aDataType);

	void* GetData();
	uint GetWidth();
	uint GetHeight();
	float GetPixelSize();
	bool GetIsPlanar();

	static SingleFrame* CreateInstance(string aFileName, void(*readStatusCallback)(FileReader::FileReaderStatus) = NULL);
private:

	FileType_enum _fileType;
	void* _file;

};

#endif