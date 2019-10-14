#include "HostSourceElement2D.h"
#include <exception>


HostSourceElement2D::HostSourceElement2D():
	mSrcDataPointer(NULL), IFilterElementSource2D(1)
{
}

bool HostSourceElement2D::CanConnect(IFilterElementBase * aFilter)
{
	throw std::runtime_error("NotImplemented yet!");
}

BufferRequestSet HostSourceElement2D::GetBufferRequests()
{
	return mBufferRequests;
}

void HostSourceElement2D::Allocate()
{
	MemoryPool::Get()->FreeAllocations(mBufferRequests);
	mBufferRequests.Clear();

	mOutputBuffer = make_shared<BufferRequest>(BT_DefaultHost, mOutputParameter[0].DataType, mOutputParameter[0].Size.width, mOutputParameter[0].Size.height);
	mBufferRequests.AddBufferRequest(mOutputBuffer);
}

void HostSourceElement2D::Prepare()
{
	//Nothing to prepare, propagate to successors:
	for (auto elem: mSuccessor)
	{
		elem->Prepare();
	}
}

void HostSourceElement2D::SetComputeParameters(IComputeParameters * aParameters)
{
	HostSourceElement2DComputeParameters* compParams = dynamic_cast<HostSourceElement2DComputeParameters*>(aParameters);
	if (!compParams)
	{
		return;
	}
	mSrcDataPointer = compParams->SrcPointer;
}

void HostSourceElement2D::GetComputeParameters(IComputeParameters * aParameters)
{
	HostSourceElement2DComputeParameters* compParams = dynamic_cast<HostSourceElement2DComputeParameters*>(aParameters);
	if (!compParams)
	{
		return;
	}
	compParams->SrcPointer = mSrcDataPointer;
}

std::shared_ptr<BufferRequest> HostSourceElement2D::GetOutputImageBuffer(size_t aIndex)
{
	if (aIndex != 0)
	{
		throw std::runtime_error("aIndex is out of bounds!");
	}
	return mOutputBuffer;
}

void HostSourceElement2D::Execute(FilterROI aRoi)
{
	ExecuteOne(aRoi);

	//propagate to successors:
	for (auto elem: mSuccessor)
	{
		IFilterElement2DBase* temp = dynamic_cast<IFilterElement2DBase*>(elem);
		if (temp) //always true...
		{
			temp->Execute(aRoi);
		}
	}
}

void HostSourceElement2D::ExecuteOne(FilterROI aRoi)
{
	if (!mROI.Contains(aRoi))
	{
		throw std::runtime_error("ROI mismatch!");
	}

	size_t typeSize = GetDataTypeSize(mOutputParameter[0].DataType);
	//pitched and ROI based copy:
	for (int y = aRoi.Top(); y <= aRoi.Bottom(); y++)
	{
		memcpy((char*)mOutputBuffer->mPtr + y * mOutputBuffer->mAllocatedPitch + aRoi.Left() * typeSize,
			(char*)mSrcDataPointer + y * mSize.width * typeSize + aRoi.Left() * typeSize,
			aRoi.width * typeSize);
	}
}

void HostSourceElement2D::SetInputParameters(FilterParameter& aParameter, size_t aIndex)
{
	if (aIndex != 0)
	{
		throw std::runtime_error("aIndex is out of bounds!");
	}
	//Four source filter elements, we directly set output equal to input:
	mOutputParameter[0] = aParameter;
	mSize = aParameter.Size;
	mROI = FilterROI(0, 0, aParameter.Size);
}

FilterParameter HostSourceElement2D::GetOutputParameters(size_t aIndex)
{
	if (aIndex != 0)
	{
		throw std::runtime_error("aIndex is out of bounds!");
	}
	return mOutputParameter[0];
}
