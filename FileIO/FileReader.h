#ifndef FILEREADER_H
#define FILEREADER_H

#include "../Basics/Default.h"
#include "File.h"
#include <istream>

using namespace std;

#define FILEREADER_CHUNK_SIZE (10*1024*1024) //10MB

//!  FileReader provides endianess independent file read methods. 
/*!
	FileReader is an abstract class.
	\author Michael Kunz
	\date   September 2011
	\version 1.0
*/
class FileReader : public File
{
protected:
	bool isFileStream;
	istream* mIStream;

public:
	struct FileReaderStatus_struct
	{
		size_t bytesToRead;
		size_t bytesRead;
	};
	typedef struct FileReaderStatus_struct FileReaderStatus;

	//! Creates a new FileReader instance for file \p aFileName. File endianess is set to little Endian.
	FileReader(string aFileName);
	//! Creates a new FileReader instance for file \p aFileName. File endianess is set to \p aIsLittleEndian.
	FileReader(string aFileName, bool aIsLittleEndian);
	//! Creates a new FileReader instance for file stream \p aStream. File endianess is set to little Endian.
	FileReader(fstream* aStream);
	//! Creates a new FileReader instance for file stream \p aStream. File endianess is set to \p aIsLittleEndian.
	FileReader(fstream* aStream, bool aIsLittleEndian);

	FileReader(istream* aStream, bool aIsLittleEndian);
	
	virtual bool OpenAndRead() = 0;
	virtual DataType_enum GetDataType() = 0;
	
	void(*readStatusCallback)(FileReaderStatus );

protected:
	long64 ReadI8LE();
	long64 ReadI8BE();
	int ReadI4LE();
	int ReadI4BE();
	short ReadI2LE();
	short ReadI2BE();
	char ReadI1();
	ulong64 ReadUI8LE();
	ulong64 ReadUI8BE();
	uint ReadUI4LE();
	uint ReadUI4BE();
	ushort ReadUI2LE();
	ushort ReadUI2BE();
	uchar ReadUI1();
	float ReadF4LE();
	float ReadF4BE();
	double ReadF8LE();
	double ReadF8BE();
	string ReadStr(int aCount);
	string ReadStrUTF(int aCount);
	bool OpenRead();
	void CloseRead();
	void Read(char* dest, size_t count);
	void ReadWithStatus(char* dest, size_t count);

	void Seek(size_t pos, ios_base::seekdir dir);
	size_t Tell();

	friend class ImageFileDirectoryEntry;
	friend class StringImageFileDirectory;
	friend class ByteImageFileDirectory;
	friend class SByteImageFileDirectory;
	friend class UShortImageFileDirectory;
	friend class ShortImageFileDirectory;
	friend class UIntImageFileDirectory;
	friend class IntImageFileDirectory;
	friend class RationalImageFileDirectory;
	friend class SRationalImageFileDirectory;
	friend class FloatImageFileDirectory;
	friend class DoubleImageFileDirectory;

	friend class IFDImageLength;
	friend class IFDImageWidth;
	friend class IFDRowsPerStrip;
	friend class IFDStripByteCounts;
	friend class IFDStripOffsets;

	friend class ImageFileDirectory;
};

#endif