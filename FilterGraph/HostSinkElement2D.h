#ifndef HOSTSINKELEMENT2D_H
#define HOSTSINKELEMENT2D_H

#include "IFilterElement.h"

class HostSinkElement2DComputeParameters : public IComputeParameters
{
public:
	void* DestPointer;
};

class HostSinkElement2D : public IFilterElementSink2D
{
private:
	void* mDestDataPointer;
	std::shared_ptr<BufferRequest> mInputBuffer;

public:
	HostSinkElement2D();

	virtual bool CanConnect(IFilterElementBase* aFilter);
	virtual BufferRequestSet GetBufferRequests();

	virtual void Allocate();

	virtual void Prepare();

	virtual void SetComputeParameters(IComputeParameters* aParameters);
	virtual void GetComputeParameters(IComputeParameters* aParameters);

	virtual void SetInputImageBuffer(std::shared_ptr<BufferRequest> aBuffer, size_t aIndex);

	virtual void Execute(FilterROI aRoi);
	virtual void ExecuteOne(FilterROI aRoi);

	virtual void SetInputParameters(FilterParameter& aParameter, size_t aIndex);
	virtual FilterParameter GetOutputParameters(size_t aIndex);
};

#endif