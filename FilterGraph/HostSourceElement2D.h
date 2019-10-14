#ifndef HOSTSOURCEELEMENT2D_H
#define HOSTSOURCEELEMENT2D_H

#include "IFilterElement.h"

class HostSourceElement2DComputeParameters : public IComputeParameters
{
public:
	void* SrcPointer;
};

class HostSourceElement2D : public IFilterElementSource2D
{
private:
	void* mSrcDataPointer;
	std::shared_ptr<BufferRequest> mOutputBuffer;

public:
	HostSourceElement2D();

	virtual bool CanConnect(IFilterElementBase* aFilter);
	virtual BufferRequestSet GetBufferRequests();

	virtual void Allocate();

	virtual void Prepare();

	virtual void SetComputeParameters(IComputeParameters* aParameters);
	virtual void GetComputeParameters(IComputeParameters* aParameters);

	virtual std::shared_ptr<BufferRequest> GetOutputImageBuffer(size_t aIndex);

	virtual void Execute(FilterROI aRoi);
	virtual void ExecuteOne(FilterROI aRoi);

	virtual void SetInputParameters(FilterParameter& aParameter, size_t aIndex);
	virtual FilterParameter GetOutputParameters(size_t aIndex);
};

#endif