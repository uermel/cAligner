#ifndef FILTERPARAMETER_H
#define FILTERPARAMETER_H

#include "../Basics/Default.h"
#include "FilterROI.h"
#include "FilterSize.h"

class FilterParameter
{
public:
	//! Size of the memory allocation the filter operates with
	FilterSize Size;
	////! The ROI the filter is computing output values
	//FilterROI Roi;
	//! Data type
	DataType_enum DataType;
};

#endif