#ifndef DM4FILE_H
#define DM4FILE_H

#include "../Basics/Default.h"
#include "FileReader.h"
#include "Dm4FileTagDirectory.h"

//!  Dm4File represents a gatan *.dm4 file in memory and maps contained information to the default internal Image format.
/*!
	Dm4File gives access to header infos, image data.
	\author Michael Kunz
	\date   September 2011
	\version 1.0
*/
class Dm4File : public FileReader
{
public:
	Dm4File(std::string filename);
	Dm4File(istream* stream);
	virtual ~Dm4File();

	uint version;
	ulong64 fileSize;
	void* _data;

	Dm4FileTagDirectory* root;

	uint GetPixelDepthInBytes();
	uint GetImageSizeInBytes();
	void* GetImageData();
	uint GetImageDimensionX();
	uint GetImageDimensionY();
	uint GetImageDimensionZ();
	void* GetThumbnailData();
	uint GetThumbnailDimensionX();
	uint GetThumbnailDimensionY();
	float GetPixelSizeX();
	float GetPixelSizeY();
	float GetExposureTime();
	string GetAcquisitionDate();
	string GetAcquisitionTime();
	int GetCs();
	int GetVoltage();
	int GetMagnification();
	float GetTiltAngle(int aIndex);
	float GetTiltAngle();
	bool OpenAndRead();
	bool OpenAndReadHeader();
	DataType_enum GetDataType();

private:
	Dm4FileTagDirectory* GetImageDataDir();
	Dm4FileTagDirectory* GetImageTagsDir();
	Dm4FileTagDirectory* GetThumbnailDataDir();
	Dm4FileTagDirectory* GetThumbnailTagsDir();

	friend class Dm4FileStack;
};

#endif
