#ifndef DM4FILETAG_H
#define DM4FILETAG_H

#include "../Basics/Default.h"
#include "Dm3_4_Enums.h"

//!  Dm4FileTag represents a gatan *.dm4 tag. 
/*!
	\author Michael Kunz
	\date   September 2011
	\version 1.0
*/
class Dm4FileTag
{
public: 
	Dm4FileTag();
	virtual ~Dm4FileTag();

	std::string Name;
	ulong64 SizeInfoArray;
	ulong64* InfoArray; 
	char* Data;
	ulong64 SizeData;
	TagTypes tagType;

	uint GetSize(ulong64 typeId);
	uint GetSizePixel(ulong64 typeId);
	//void PrintValues(std::ostream& aStream);
	
	friend std::ostream& operator<<(std::ostream& stream, Dm4FileTag& tag);
	
	void PrintSingleValue(std::ostream& aStream, ulong64 aType, uint aStartPos = 0);
	int GetSingleValueInt(ulong64 aType, uint aStartPos = 0);
	float GetSingleValueFloat(ulong64 aType, uint aStartPos = 0);
	double GetSingleValueDouble(ulong64 aType, uint aStartPos = 0);
	std::string GetSingleValueString(ulong64 aType, uint aStartPos = 0);

	int GetStructValueAsInt(uint aIndex);
	void FreeData();
	/*uint GetValueAsUint();
	float GetValueAsFloat();
	double GetValueAsDouble();*/
	

};


#endif