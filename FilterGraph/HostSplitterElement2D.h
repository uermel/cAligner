#ifndef HOSTSPLITTERELEMENT2D_H
#define HOSTSPLITTERELEMENT2D_H

#include "IFilterElement.h"

class HostSplitterElement2DComputeParameters : public IComputeParameters
{
public:
};

class HostSplitterElement2D : public IFilterElement2D
{
private:
	std::shared_ptr<BufferRequest> mInputOuput1Buffer;
	std::shared_ptr<BufferRequest> mOuput2Buffer;

public:
	HostSplitterElement2D();

	virtual bool CanConnect(IFilterElementBase* aFilter);
	virtual BufferRequestSet GetBufferRequests();

	virtual void Allocate();

	virtual void Prepare();

	virtual void SetComputeParameters(IComputeParameters* aParameters);
	virtual void GetComputeParameters(IComputeParameters* aParameters);

	virtual void SetInputImageBuffer(std::shared_ptr<BufferRequest> aBuffer, size_t aIndex);
	virtual std::shared_ptr<BufferRequest> GetOutputImageBuffer(size_t aIndex);

	virtual void Execute(FilterROI aRoi);
	virtual void ExecuteOne(FilterROI aRoi);

	virtual void SetInputParameters(FilterParameter& aParameter, size_t aIndex);
	virtual FilterParameter GetOutputParameters(size_t aIndex);
};

#endif