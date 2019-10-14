#include "Dm4FileStack.h"
#include <iomanip>
#include <sstream>
#include <string.h>

using namespace std;

Dm4FileStack::Dm4FileStack(string aFileName)
	: FileReader(""), _dm4files(0), _fileCount(0)
{
	_fileCount = CountFilesInStack(aFileName);
	for (int i = _firstIndex; i < _fileCount; i++)
	{
		Dm4File* dm4 = new Dm4File(GetFileNameFromIndex(i, aFileName));
		_dm4files.push_back(dm4);
	}
}

Dm4FileStack::~Dm4FileStack()
{
	for (uint i = 0; i < _dm4files.size(); i++)
	{
		delete _dm4files[i];
	}

}


bool Dm4FileStack::fexists(string aFileName)
{
  ifstream ifile(aFileName.c_str());
  return !!ifile;
}

string Dm4FileStack::GetStringFromInt(int aInt)
{
	ostringstream ss;
	ss << setfill('0') << setw(4);
	ss << aInt;
	return ss.str();
}

string Dm4FileStack::GetFileNameFromIndex(int aIndex, std::string aFileName)
{
	string fileEnding(".dm4");
	const int count0 = 4;
	size_t pos = aFileName.find_last_of(fileEnding);
	if (pos == string::npos) return "";

	//Substract length of search string:
	pos -= fileEnding.length()+3;
	return aFileName.replace(pos, count0, GetStringFromInt(aIndex));
}

int Dm4FileStack::CountFilesInStack(std::string aFileName)
{
	bool notFinished = true;
	int fileCounter = 0;
	bool firstFound = false;
	_firstIndex = -1;
	int notUnlimited = 0;

	while (notFinished)
	{
		string file = GetFileNameFromIndex(fileCounter, aFileName);
		if (!fexists(file))
		{
			if (firstFound)
			{
				notFinished = false;
				fileCounter--;
			}
			else
			{
				notUnlimited++;
			}
		}
		else
		{
			if (!firstFound)
				_firstIndex = fileCounter;
			firstFound = true;
		}
		fileCounter++;
		if (notUnlimited > 100) return 0;
	}
	return fileCounter;
}


bool Dm4FileStack::OpenAndRead()
{
    if (_fileCount == 0) return false;
	bool res = true;

	if (_data)
		delete[] _data;
	_data = NULL;

	int imgsize = 0;

	for (int i = _firstIndex; i < _fileCount; i++)
	{
		res &= _dm4files[i-_firstIndex]->OpenAndRead();

		if (i == _firstIndex)
		{
			imgsize = _dm4files[0]->GetImageSizeInBytes();
			_data = new char[(long long)imgsize * (long long)(_fileCount - _firstIndex)];
		}
		char* img = _dm4files[i-_firstIndex]->GetImageData();
		memcpy(_data + (long long)imgsize * (long long)(i - _firstIndex), img, (long long)imgsize);
		_dm4files[i-_firstIndex]->GetImageDataDir()->FindTag("Data")->FreeData();
		_dm4files[i-_firstIndex]->_data = NULL;
		std::cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
		std::cout << "File index: " << i << std::flush;
	}
    std::cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";

	if (!res)
	{
		delete[] _data;
		_data = NULL;
	}
	return res;
}

DataType_enum Dm4FileStack::GetDataType()
{
	if (_dm4files.size() < 1) return DT_UNKNOWN;
	return _dm4files[0]->GetDataType();
}



void Dm4FileStack::ReadHeaderInfo()
{
	if (_dm4files.size() < 1) return;


	ClearImageMetadata();

	DimX = _dm4files[0]->GetImageDimensionX();
	DimY = _dm4files[0]->GetImageDimensionY();
	DimZ = _fileCount - _firstIndex;

	CellSizeX = _dm4files[0]->GetPixelSizeX();
	CellSizeY = _dm4files[0]->GetPixelSizeY();
	CellSizeZ = _dm4files[0]->GetPixelSizeY();

	DataType = _dm4files[0]->GetDataType();

	Cs = _dm4files[0]->GetCs();
	Voltage = _dm4files[0]->GetVoltage();

	AllocArrays(_fileCount - _firstIndex);

	for (int i = 0; i < _fileCount - _firstIndex; i++)
	{
		TiltAlpha[i] = _dm4files[i]->GetTiltAngle();

		Defocus[i] = 0;
		ExposureTime[i] = _dm4files[i]->GetExposureTime();
		TiltAxis[i] = 0;
		PixelSize[i] = _dm4files[i]->GetPixelSizeX();
		Magnification[i] = (float)_dm4files[i]->GetMagnification();
	}
}

void Dm4FileStack::WriteInfoToHeader()
{
	//We can't write dm4 files...
}

char* Dm4FileStack::GetData()
{
	return _data;
}
