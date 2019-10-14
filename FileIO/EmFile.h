#ifndef EMFILE_H
#define EMFILE_H

#include "../Basics/Default.h"
#include "EmHeader.h"
#include "FileReader.h"
#include "FileWriter.h"


using namespace std;

//!  EmFile represents a *.Em file in memory and maps contained information to the default internal Image format. 
/*!
EmFile gives access to header infos, volume data and single projections.
\author Michael Kunz
\date   September 2011
\version 1.0
*/
class EmFile : public FileReader, public FileWriter
{
protected:
	EmHeader _fileHeader;
	uint _GetDataTypeSize(EmDataType_Enum aDataType);
	uint _dataStartPosition;
	void* _data;

public:
	//! Creates a new EmFile instance. The file name is only set internally; the file itself keeps untouched.
	EmFile(string aFileName);

	~EmFile();

	//! Opens the file File#mFileName and reads the entire content.
	/*!
	\throw FileIOException
	*/
	bool OpenAndRead();

	//! Opens the file File#mFileName and reads only the file header.
	/*!
	\throw FileIOException
	*/
	bool OpenAndReadHeader();

	//! Determines if a given image dimension and datatype can be written to a EM file
	static bool CanWriteAsEM(int aDimX, int aDimY, int aDimZ, DataType_enum aDatatype);

	//! Opens the file File#mFileName and writes the header.
	static bool InitHeader(string aFileName, int aDimX, int aDimY, float aPixelSize, DataType_enum aDatatype);

	//! Opens the file File#mFileName and writes the header.
	static bool InitHeader(string aFileName, int aDimX, int aDimY, int aDimZ, float aPixelSize, DataType_enum aDatatype);

	//! Opens the file File#mFileName and writes the header.
	static bool InitHeader(string aFileName, EmHeader& header);

	//! Opens the file File#mFileName and writes the data after the previously written header.
	static bool WriteRawData(string aFileName, void* aData, size_t aSize);

	//! Initialises a header structure
	static bool SetHeaderData(EmHeader& header, int aDimX, int aDimY, int aDimZ, float aPixelSize, DataType_enum aDatatype);

	//! Converts from Em data type enum to internal data type
	/*!
	EmFile::GetDataType dows not take into account if the data type is unsigned or signed as the
	Em file format cannot distinguish them.
	*/
	DataType_enum GetDataType();

	//! Returns the size of the data block. If the header is not yet read, it will return 0.
	size_t GetDataSize();

	//! Returns a reference to the inner Em file header.
	EmHeader& GetFileHeader();

	//! Returns the inner data pointer.
	void* GetData();

	//! Returns the inner data pointer shifted to image plane idx.
	void* GetData(size_t idx);
};

#endif