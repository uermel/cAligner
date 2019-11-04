#ifndef MARKERFILE_H
#define MARKERFILE_H
#include "../Basics/Default.h"
#include "../FilterGraph/PointF.h"
#include "EmFile.h"
#include <vector>
//#include "../FilterGraph/Matrix.h"
#include "Eigen/Dense"
#include "ImodFiducialFile.h"

//! Definition of marker file items
enum MarkerFileItem_enum
{
	MFI_TiltAngle = 0,
	MFI_X_Coordinate,
	MFI_Y_Coordinate,
	MFI_DevOfMark,
	MFI_X_Shift,
	MFI_Y_Shift,
	MFI_ProjectedCoordinateX,
	MFI_ProjectedCoordinateY,
	MFI_Magnifiaction,
	MFI_RotationPsi = 9
};

struct Marker
{
	float TiltAngle;
	float CoordinateX;
	float CoordinateY;
	float DevOfMark;
	float ShiftX;
	float ShiftY;
	float ProjectedCoordinateX;
	float ProjectedCoordinateY;
	float Magnifiaction;
	float RotationPsi;
};


struct Minimize
{
    // host ptrs
    double* buf;
    double* measured;
    double* work;
    double* used;
    double* vector;
    double* result;
    double* shifts;
    double* thetas;
    double* psis;
    double* mags;
    double* error;
    double* errorPerMarker;
    double* errorPerProjection;
    double* usedPerMarker;
    double* usedPerProjection;

    double* jac;

    // device ptrs
    double* d_buf;
    double* d_measured;
    double* d_work;
    double* d_used;
    double* d_vector;
    double* d_result;
    double* d_shifts;
    double* d_thetas;
    double* d_psis;
    double* d_mags;
    double* d_jac;

    // buffer sizes
    int buf_sz;
    int measured_sz;
    int work_sz;
    int used_sz;
    int vector_sz;
    int result_sz;
    int shifts_sz;
    int thetas_sz;
    int psis_sz;
    int mags_sz;
    int error_sz;
    int errorPerMarker_sz;
    int errorPerProjection_sz;
    int usedPerMarker_sz;
    int usedPerProjection_sz;
    int jac_sz;
    int temp_jac_sz;

    // constant values
    double imdimX;
    double imdimY;
    double delta;
    double phi;
    double totalUsed;
    int ProjectionCount;
    int MarkerCount;
    int ParamCount;
    int temp_ParamCount;

    // Minimization mode
    bool Shift;
    bool Psi;
    bool PsiFixed;
    bool Theta;
    bool Phi;
    bool Magnitude;
};

//! Represents a marker file stored in EM-file format.
/*!
\author Michael Kunz
\date   September 2011
\version 1.0
*/
class MarkerFile : private EmFile
{
public:
	//! Creates a new MarkerFile instance. The data is directly read from file.
	MarkerFile(string aFileName);

	//! Initializes a new MarkerFile instance with one unclicked marker per projection.
	MarkerFile(int aProjectionCount, float* aTiltAngles);

	virtual ~MarkerFile();

	//! Checks if the given file fits the dimensions of a marker file.
	static bool CanReadAsMarkerfile(string aFilename);

	//! Returns the number of markers in the marker file.
	int GetMarkerCount();

	//! Returns the number of valid projections in the marker file.
	int GetProjectionCount();

	//! Returns the number of all valid and unvalid projections in the marker file.
	int GetTotalProjectionCount();

	//! Returns a pointer to the inner data array.
	float* GetData();

	//! Returns a reference to value with index (\p aItem, \p aProjection, \p aMarker).
	float& operator() (const MarkerFileItem_enum aItem, const int aProjection, const int aMarker);

	//! Returns a reference to value with index (\p aProjection, \p aMarker).
	Marker& operator() (const int aProjection, const int aMarker);

	//! Tests if the first gold bead in projection \p index is clicked.
	bool CheckIfProjIndexIsGood(const int index);

	//! Adds a new marker to projections at position (INVALID, INVALID), i.e. unclicked
	void AddMarker();

	//! Removes the marker \p idx from the marker list
	void RemoveMarker(int idx);

	//! Sets the clicked position of marker \p aMarkerIdx in projection \p aProjection to (\p aPositionX, \p aPositionY)
	void SetMarkerPosition(int aProjection, int aMarkerIdx, float aPositionX, float aPositionY);

	//! Resets all tilt angles to \p aTiltAngles (necessary to be done before re-aligning the markers)
	void SetTiltAngles(float* aTiltAngles);

	//! Returns a vector of positions from all clicked markers in projection \p aProjection
	std::vector<PointF> GetMarkersAt(int aProjection);

	//! Returns a vector of the projected positions from all clicked markers in projection \p aProjection
	std::vector<PointF> GetAlignedMarkersAt(int aProjection);

	//! Sets the image shift of projection \p aProjection to (\p aShiftX, \p aShiftY) determined elsewhere.
	void SetImageShift(int aProjection, float aShiftX, float aShiftY);

	//! Sets the image rotation (or tilt axis in IMOD) of all projections to \p aAngleInDeg (determined elsewhere).
	void SetConstantImageRotation(float aAngleInDeg);

	//! Alignes the markerfile using the old and simple alignment method
	void AlignSimple(int aReferenceMarker, int imageWidth, int imageHeight, float& aErrorScore, float& aImageRotation);

	//! Alignes the markerfile using the new 3D alignment method
	void Align3D(int aReferenceMarker, int width, int height, float& aErrorScore, float& aPhi, bool DoPsi, bool DoFixedPsi, bool DoTheta, bool DoPhi, bool DoMags, bool normMin, bool normZeroTilt, bool magsFirst, int iterSwitch, int iterations, float addZShift, float* projErrorList = NULL, float* markerErrorList = NULL, float* aX = NULL, float* aY = NULL, float* aZ = NULL, void (*updateprog)(int percent) = NULL);

	//! Returns the index of the projection with the smallest tilt angle
	int GetMinTiltIndex();

	bool Save(string aFileName);

	void SetMagAnisotropy(float aAmount, float angInDeg, float dimX, float dimY);

	void GetMagAnisotropy(float& aAmount, float& angInDeg);

	void GetBeamDeclination(float& aPhi);

	static void projectTestBeads(int imdim, int markerCount, float* x, float* y, float* z, int projectionCount, float* thetas, float* psis, float phi, float* mags, float* shiftX, float* shiftY, float magAniso, float** posX, float** posY);

	string GetFilename();

	void GetMarkerPositions(float* xPos, float* yPos, float* zPos);

	void DeleteAllMarkersInProjection(int idx);

	static MarkerFile* ImportFromIMOD(string aFilename);

private:
	float mMagAnisotropyAmount;
	float mMagAnisotropyAngle;
	//Matrix<float> mMagAnisotropy;
	Eigen::Matrix3f mMagAnisotropy;
	float* mMarkerXPos;
	float* mMarkerYPos;
	float* mMarkerZPos;

	float det(float matrix[3][3]);

	void AlignSimple(int aReferenceMarker, int imageWidth, int imageHeight, float& aErrorScore, float& aImageRotation, float* x, float* y, float* z);

	static int GetBeadIndex(int proj, int bead, int xy, int MarkerCount);

    static void projectBeads(double *p, double *hx, int m, int n, void *adata);

	static double computeRMS(Minimize *data);

    static void computeJacobian(double *p, struct splm_ccsm *jac, int nvars, int nobs, void *adata);

	static void SetLevmarOptions(double options[5]);

	void initMinimization(Minimize *data);

	void MinimizeShift(Minimize *data);

	void MinimizePsis(Minimize *data);

	void MinimizePsiFixed(Minimize *data);

	void MinimizeThetas(Minimize *data);

	void MinimizeMags(Minimize *data);

	void MinimizePhi(Minimize *data);

	void finalizeMinimization(Minimize *data);

	//static void MoveXYToMagDistort(float& x, float& y, float amplitude, float angleInRad, Matrix<float>& m, float dimX, float dimY);
	static void MoveXYToMagDistort(float & x, float & y, float amplitude, float angleInRad, Eigen::Matrix3f& m, float dimX, float dimY);

};

#endif //MARKERFILE_H
