#include "ImageBase.h"
#include "../MKLog/MKLog.h"

using namespace std;

ImageBase::ImageBase(){}
ImageBase::~ImageBase(){}

FileType_enum ImageBase::GuessFileTypeFromEnding(string aFilename)
{
	string::size_type dotPos = aFilename.find_last_of('.');
	if (dotPos == string::npos)
	{
		throw FileIOException(aFilename, "Cannot determine filetype from file ending.");
	}

	string ending = aFilename.substr(dotPos);

	MKLOG("Found file ending: " + ending);

	if (ending == ".mrc")
		return FileType_enum::FT_MRC;
	if (ending == ".st")
		return FileType_enum::FT_MRC;
	if (ending == ".rec")
		return FileType_enum::FT_MRC;
	if (ending == ".dm4")
		return FileType_enum::FT_DM4;
	if (ending == ".dm3")
		return FileType_enum::FT_DM3;
	if (ending == ".em")
		return FileType_enum::FT_EM;
	if (ending == ".ser")
		return FileType_enum::FT_SER;
	if (ending == ".tif")
		return FileType_enum::FT_TIFF;
	if (ending == ".tiff")
		return FileType_enum::FT_TIFF;
	if (ending == ".lsm")
		return FileType_enum::FT_TIFF;

	throw FileIOException(aFilename, "Cannot determine filetype from file ending.");
}