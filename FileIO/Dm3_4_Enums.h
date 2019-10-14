#ifndef DM3_4_ENUMS_H
#define DM3_4_ENUMS_H

enum DataTypesTag
{
	DTTI2 = 2,
	DTTI4 = 3,
	DTTUI2 = 4,
	DTTUI4 = 5,
	DTTF4 = 6,
	DTTF8 = 7,
	DTTI1 = 8,
	DTTCHAR = 9,
	DTTI1_2 = 10,
	DTTUI8 = 11
};

enum DataTypesImage
{
	DTT_UNKNOWN = 0,
	DTT_I2 = 1,
	DTT_F4 = 2,
	DTT_C8 = 3,
	DTT_OBSOLETE = 4,
	DTT_C4 = 5,
	DTT_UI1 = 6,
	DTT_I4 = 7,
	DTT_RGB_4UI1 = 8,
	DTT_I1 = 9,
	DTT_UI2 = 10,
	DTT_UI4 = 11,
	DTT_F8 = 12,
	DTT_C16 = 13,
	DTT_BINARY = 14,
	DTT_RGBA_4UI1 = 23
};

enum TagTypes
{
	SingleEntryTag,
	StructTag,
	ArrayTag,
	ArrayStructTag
};

enum DataTypes
{
	DTI2 = 2,
	DTI4 = 3,
	DTUI2 = 4,
	DTUI4 = 5,
	DTF4 = 6,
	DTF8 = 7,
	DTI1 = 8,
	DTCHAR = 9,
	DTI1_2 = 10,
	DTSTRUCT = 15,
	DTSTR = 18,
	DTARRAY = 20
};

#endif