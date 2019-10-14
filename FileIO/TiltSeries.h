#ifndef TILTSERIES_H
#define TILTSERIES_H

#include "FileIO.h"
#include "ImageBase.h"
#include <vector>

using namespace std;

class TiltSeries : public ImageBase
{
public:
	TiltSeries(string aFileName, void(*readStatusCallback)(FileReader::FileReaderStatus) = NULL);
	~TiltSeries();

	virtual ImageType_enum GetImageType();
	virtual FileType_enum GetFileType();
	virtual DataType_enum GetFileDataType();
	virtual bool NeedsFlipOnYAxis();

	static bool CanReadFile(string aFilename);
	static bool CanReadFile(string aFilename, FileType_enum& fileType);
	static bool CanReadFile(string aFilename, FileType_enum& fileType, int& aWidth, int& aHeight, int &aImageCount, DataType_enum& aDataType);

	void* GetData(size_t idx);
	uint GetWidth();
	uint GetHeight();
	uint GetImageCount();
	float GetPixelSize();
	float GetTiltAngle(size_t idx);

	static TiltSeries* CreateInstance(string aFileName, void(*readStatusCallback)(FileReader::FileReaderStatus) = NULL);
private:

	FileType_enum _fileType;
	vector<void*> _files;

	static bool FileIsPartOfMultiFiles(string aFileName);
	static vector<string> GetMultiFilesFromFilename(string aFileName);
	static int TiltSeries::CountFilesInStack(std::string aFileName, int& aFirstIndex);
	static bool TiltSeries::fexists(string aFileName);
	static string TiltSeries::GetStringFromInt(int aInt);
	static string TiltSeries::GetFileNameFromIndex(int aIndex, string aFileName);

};

#endif