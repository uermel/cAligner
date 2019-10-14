#ifndef IMODFIDUCIALFILE_H
#define IMODFIDUCIALFILE_H

#include "../Basics/Default.h"
#include "FileReader.h"
#include "../FilterGraph/PointF.h"
#include <vector>

#define IMOD (0x494d4f44)
#define V12  (0x56312e32)
#define IEOF (0x49454f46)
#define OBJT (0x4f424a54)
#define CONT (0x434f4e54)

struct ImodModelDataStructure
{
	char name[128];
	int xmax;
	int ymax;
	int zmax;

	int objsize;
	uint flags;
	int drawmode;
	int mousemode;
	int blacklevel;
	int whitelevel;
	
	float xoffset;
	float yoffset;
	float zoffset;

	float xscale;
	float yscale;
	float zscale;

	int object;
	int contour;
	int point;

	int res;
	int thresh;

	float pixsize;
	int units;

	int csum;
	float alpha;
	float beta;
	float gamma;
};

struct ImodObjectDataStructure
{
	char name[64];
	uint reserved[16];

	int contsize;
	uint flags;
	int axis;
	int drawmode;

	float red;
	float green;
	float blue;

	int pdrawsize;
	uchar symbol;
	uchar symsize;
	uchar linewidth2;
	uchar linewidth;
	uchar linesty;
	uchar symflags;

	uchar sympad;
	uchar trans;
	int meshsize;
	int surfsize;

};

struct ContourDataStructure
{
	int psize;
	uint flags;
	int time;
	int surf;
	std::vector<float> ptX;
	std::vector<float> ptY;
	std::vector<float> ptZ;
};

class ImodFiducialFile :
	public FileReader
{
public:
	ImodFiducialFile(string aFilename);
	~ImodFiducialFile();

	bool OpenAndRead();
	std::vector<PointF> GetMarkers(size_t projectionIdx);
	size_t GetProjectionCount();
	size_t GetMarkerCount();
	std::vector<float> GetTiltAngles();

	DataType_enum GetDataType();

private:
	ImodModelDataStructure _fileHeader;
	ImodObjectDataStructure _objHeader;
	std::vector<ContourDataStructure> _contours;
	std::vector<float> tiltAngles;
	std::vector<float> preAligX;
	std::vector<float> preAligY;
	int _binning;

	bool ReadTiltAngles();
	bool ReadPrealignment();
};

#endif // !IMODFIDUCIALFILE_H
