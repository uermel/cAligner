#ifndef FILTERGRAPH_H
#define FILTERGRAPH_H

#include "../Basics/Default.h"
#include "IFilterElement.h"

class FilterGraph
{
public:

private:
	std::vector<IFilterElementSource*> mSourceFilter;
	std::vector<IFilterElementSink*> mSinkFilter; //to get faster access to it...

};

#endif