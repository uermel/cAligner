#ifndef DM4FILETAGDIRECTORY_H
#define DM4FILETAGDIRECTORY_H

#include "../Basics/Default.h"
#include "FileReader.h"
#include "Dm4FileTag.h"
#include <sstream>

using namespace std;

#define TAG_ID 0x15
#define TAGDIR_ID 0x14
#define DIREOF 0x0
#define TAGSTRUCT 0x0f
#define TAGARRAY 0x14


//!  Dm4FileTagDirectory represents a gatan *.dm4 tag directory.
/*!
	\author Michael Kunz
	\date   September 2011
	\version 1.0
*/
class Dm4FileTagDirectory : public FileReader
{
public:
	Dm4FileTagDirectory(fstream* aStream, bool aIsLittleEndian);
	Dm4FileTagDirectory(fstream* aStream, bool aIsLittleEndian, bool nonRoot);
	Dm4FileTagDirectory(fstream* aStream, bool aIsLittleEndian, bool nonRoot, bool thumbnailOnly);
	Dm4FileTagDirectory(istream* aStream, bool aIsLittleEndian);
	Dm4FileTagDirectory(istream* aStream, bool aIsLittleEndian, bool nonRoot);
	virtual ~Dm4FileTagDirectory();

	bool sorted;
	bool closed;
	uint countTags;
	string Name;
	ushort LengthName;

	vector<Dm4FileTag*> Tags;
	vector<Dm4FileTagDirectory*> TagDirs;

	void Print(ostream& stream, uint id, string pre = "");
	Dm4FileTag* FindTag(string aName);
	Dm4FileTagDirectory* FindTagDir(string aName);

	bool OpenAndRead();
	DataType_enum GetDataType();
private:
	void readTag();
	void readTagHeaderOnly();
	void readTagThumbnail();
};

#endif
