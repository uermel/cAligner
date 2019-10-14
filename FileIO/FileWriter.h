#ifndef FILEWRITER_H
#define FILEWRITER_H

#include "../Basics/Default.h"
#include "File.h"
#define FILEWRITER_CHUNK_SIZE (10*1024*1024) //10MB

using namespace std;

//!  FileWriter provides endianess independent file write methods. 
/*!
	\author Michael Kunz
	\date   September 2011
	\version 1.0
*/
class FileWriter : public File
{
public:
	//! Creates a new FileWriter instance for file \p aFileName. File endianess is set to little Endian.
	FileWriter(string aFileName);
	//! Creates a new FileWriter instance for file \p aFileName. File endianess is set to \p aIsLittleEndian.
	FileWriter(string aFileName, bool aIsLittleEndian);
	//! Creates a new FileWriter instance for file stream \p aStream. File endianess is set to little Endian.
	FileWriter(fstream* aStream);
	//! Creates a new FileWriter instance for file stream \p aStream. File endianess is set to \p aIsLittleEndian.
	FileWriter(fstream* aStream, bool aIsLittleEndian);
	
	//virtual bool OpenAndWrite() = 0;
	//virtual void SetDataType(DataType_enum aType) = 0;
	
	void(*writeStatusCallback)(int);

//protected:	
	void WriteBE(ulong64& aX);
	void WriteLE(ulong64& aX);
	void WriteBE(uint& aX);
	void WriteLE(uint& aX);
	void WriteBE(ushort& aX);
	void WriteLE(ushort& aX);
	void Write(uchar& aX);
	void WriteBE(long64& aX);
	void WriteLE(long64& aX);
	void WriteBE(int& aX);
	void WriteLE(int& aX);
	void WriteBE(short& aX);
	void WriteLE(short& aX);
	void Write(char& aX);
	void WriteBE(double& aX);
	void WriteLE(double& aX);
	void WriteBE(float& aX);
	void WriteLE(float& aX);
	void Write(void* aX, size_t aCount);
	void Seek(size_t pos, ios_base::seekdir dir);

	bool OpenWrite(bool createNew);
	void CloseWrite();
	size_t Tell();
	void Write(char* src, size_t count);
	void WriteWithStatus(char* src, size_t count);
};

#endif