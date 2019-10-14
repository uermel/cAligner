#ifndef DM3FILE_H
#define DM3FILE_H

#include "../Basics/Default.h"
#include "FileReader.h"
#include "Dm3FileTagDirectory.h"

//!  Dm3File represents a gatan *.dm3 file in memory and maps contained information to the default internal Image format. 
/*!
	Dm3File gives access to header infos, image data.
	\author Michael Kunz
	\date   September 2011
	\version 1.0
*/
class Dm3File : public FileReader
{
public:
	Dm3File(std::string filename);
	Dm3File(fstream* stream);
	Dm3File(istream* stream);
	virtual ~Dm3File();

	uint version;
	uint fileSize;
	void* _data;

	Dm3FileTagDirectory* root;

	uint GetPixelDepthInBytes();
	uint GetImageSizeInBytes();
	void* GetImageData();
	uint GetImageDimensionX();
	uint GetImageDimensionY();
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
	Dm3FileTagDirectory* GetImageDataDir();
	Dm3FileTagDirectory* GetImageTagsDir();

	friend class Dm3FileStack;
};

#endif