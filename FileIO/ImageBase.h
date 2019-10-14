#ifndef IMAGEBASE_H
#define IMAGEBASE_H

#include "../Basics/Default.h"
#include "FileIO.h"
#include "FileIOException.h"

class ImageBase
{
public:
	ImageBase();
	~ImageBase();

	virtual ImageType_enum GetImageType() = 0;
	virtual FileType_enum GetFileType() = 0;
	virtual DataType_enum GetFileDataType() = 0;
	virtual bool NeedsFlipOnYAxis() = 0;

	static FileType_enum GuessFileTypeFromEnding(std::string aFileName);
 
private:

protected:
};

#endif