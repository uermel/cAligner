#ifndef FILTERPARAMETER3D_H
#define FILTERPARAMETER3D_H

#include "../Basics/Default.h"
#include "FilterROI3D.h"
#include "FilterSize3D.h"

class FilterParameter3D
{
public:
	//! Size of the memory allocation the filter operates with
	FilterSize3D Size;
	//! The ROI the filter is computing output values
	FilterROI3D Roi;
	//! Data type
	DataType_enum DataType;
};

#endif