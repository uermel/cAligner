#ifndef FILEIO_H
#define FILEIO_H

#include "../Basics/Default.h"

enum FileType_enum
{
	FT_DM3,
	FT_DM4,
	FT_MRC,
	FT_EM,
	FT_SER,
	FT_TIFF,
	FT_NONE
};

#include "MRCFile.h"
#include "Dm3File.h"
#include "Dm4File.h"
#include "SERFile.h"
#include "EmFile.h"
#include "TIFFFile.h"

#endif
