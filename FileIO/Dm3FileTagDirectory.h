#ifndef DM3FILETAGDIRECTORY_H
#define DM3FILETAGDIRECTORY_H

#include "../Basics/Default.h"
#include "FileReader.h"
#include "Dm3FileTag.h"
#include <sstream>

using namespace std;

#define TAG_ID 0x15
#define TAGDIR_ID 0x14
#define DIREOF 0x0
#define TAGSTRUCT 0x0f
#define TAGARRAY 0x14


//!  Dm3FileTagDirectory represents a gatan *.dm3 tag directory. 
/*!
	\author Michael Kunz
	\date   September 2011
	\version 1.0
*/
class Dm3FileTagDirectory : public FileReader
{
public: 
	Dm3FileTagDirectory(fstream* aStream, bool aIsLittleEndian);
	Dm3FileTagDirectory(fstream* aStream, bool aIsLittleEndian, bool nonRoot);
	Dm3FileTagDirectory(istream* aStream, bool aIsLittleEndian);
	Dm3FileTagDirectory(istream* aStream, bool aIsLittleEndian, bool nonRoot);
	Dm3FileTagDirectory(fstream* aStream, bool aIsLittleEndian, bool nonRoot, bool headerOnly);
	virtual ~Dm3FileTagDirectory();

	bool sorted;
	bool closed;
	uint countTags;
	string Name;
	ushort LengthName;

	vector<Dm3FileTag*> Tags;
	vector<Dm3FileTagDirectory*> TagDirs;
	
	void Print(ostream& stream, uint id, string pre = "");
	Dm3FileTag* FindTag(string aName);
	Dm3FileTagDirectory* FindTagDir(string aName);

	bool OpenAndRead();
	DataType_enum GetDataType();
private:
	void readTag();
	void readTagHeaderOnly();
};

#endif