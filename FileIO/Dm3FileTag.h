#ifndef DM3FILETAG_H
#define DM3FILETAG_H

#include "../Basics/Default.h"
#include "Dm3_4_Enums.h"

//!  Dm3FileTag represents a gatan *.dm3 tag. 
/*!
	\author Michael Kunz
	\date   September 2011
	\version 1.0
*/
class Dm3FileTag
{
public: 
	Dm3FileTag();
	virtual ~Dm3FileTag();

	std::string Name;
	uint SizeInfoArray;
	uint* InfoArray; 
	char* Data;
	uint SizeData;
	TagTypes tagType;

	uint GetSize(uint typeId);
	uint GetSizePixel(uint typeId);
	//void PrintValues(std::ostream& aStream);
	
	friend std::ostream& operator<<(std::ostream& stream, Dm3FileTag& tag);
	
	void PrintSingleValue(std::ostream& aStream, uint aType, uint aStartPos = 0);
	int GetSingleValueInt(uint aType, uint aStartPos = 0);
	float GetSingleValueFloat(uint aType, uint aStartPos = 0);
	double GetSingleValueDouble(uint aType, uint aStartPos = 0);
	std::string GetSingleValueString(uint aType, uint aStartPos = 0);

	int GetStructValueAsInt(uint aIndex);
	void FreeData();
	/*uint GetValueAsUint();
	float GetValueAsFloat();
	double GetValueAsDouble();*/
	

};


#endif