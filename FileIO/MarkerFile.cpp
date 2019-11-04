#include "MarkerFile.h"
#include "../Minimization/AlignmentProblem.h"
#include "../Minimization/Solver.h"
#include <math.h>
#include "Eigen/Dense"
#include <algorithm>
#include <iostream>

#define MARKERFILE_X_DIMENSION 10
#define INVALID_MARKER_POSITION -1
#define _USE_MATH_DEFINES

MarkerFile::MarkerFile(string aFileName)
    : EmFile(aFileName), mMagAnisotropyAmount(1), mMagAnisotropyAngle(0), mMagAnisotropy(Eigen::Matrix3f::Zero())
{
	OpenAndRead();

	mMarkerXPos = new float[_fileHeader.DimZ];
	mMarkerYPos = new float[_fileHeader.DimZ];
	mMarkerZPos = new float[_fileHeader.DimZ];

	memset(mMarkerXPos, 0, _fileHeader.DimZ * sizeof(float));
	memset(mMarkerYPos, 0, _fileHeader.DimZ * sizeof(float));
	memset(mMarkerZPos, 0, _fileHeader.DimZ * sizeof(float));

	if (_fileHeader.IsNewHeaderFormat)
	{
		if (_fileHeader.MarkerOffset)
		{
			FileReader::OpenRead();
			FileReader::Seek(_fileHeader.MarkerOffset, ios::beg);
			for (size_t i = 0; i < _fileHeader.DimZ; i++)
			{
				mMarkerXPos[i] = FileReader::ReadF4LE();
				mMarkerYPos[i] = FileReader::ReadF4LE();
				mMarkerZPos[i] = FileReader::ReadF4LE();
			}
			FileReader::CloseRead();
		}
		SetMagAnisotropy(_fileHeader.MagAnisotropyFactor, _fileHeader.MagAnisotropyAngle, _fileHeader.ImageSizeX, _fileHeader.ImageSizeY);
	}
}

//MarkerFile::MarkerFile(int aProjectionCount, float * aTiltAngles)
//	: EmFile(""), mMagAnisotropyAmount(1), mMagAnisotropyAngle(0), mMagAnisotropy(Matrix<float>(3,3))
MarkerFile::MarkerFile(int aProjectionCount, float * aTiltAngles)
	: EmFile(""), mMagAnisotropyAmount(1), mMagAnisotropyAngle(0), mMagAnisotropy(Eigen::Matrix3f::Zero())
{
	SetHeaderData(_fileHeader, MARKERFILE_X_DIMENSION, aProjectionCount, 1, 0.0f, DT_FLOAT);
	_fileHeader.IsNewHeaderFormat = 1;
	_fileHeader.MagAnisotropyFactor = 1;
	_data = new float[MARKERFILE_X_DIMENSION * aProjectionCount];
	memset(_data, 0, MARKERFILE_X_DIMENSION * aProjectionCount * sizeof(float));

	for (int proj = 0; proj < aProjectionCount; proj++)
	{
		(*this)(MFI_TiltAngle, proj, 0) = aTiltAngles[proj];
		(*this)(MFI_X_Coordinate, proj, 0) = INVALID_MARKER_POSITION;
		(*this)(MFI_Y_Coordinate, proj, 0) = INVALID_MARKER_POSITION;
		(*this)(MFI_Magnifiaction, proj, 0) = 1.0f;
	}

	mMarkerXPos = new float[_fileHeader.DimZ];
	mMarkerYPos = new float[_fileHeader.DimZ];
	mMarkerZPos = new float[_fileHeader.DimZ];

	memset(mMarkerXPos, 0, _fileHeader.DimZ * sizeof(float));
	memset(mMarkerYPos, 0, _fileHeader.DimZ * sizeof(float));
	memset(mMarkerZPos, 0, _fileHeader.DimZ * sizeof(float));
}

MarkerFile::~MarkerFile()
{
	delete[] mMarkerXPos;
	delete[] mMarkerYPos;
	delete[] mMarkerZPos;
}

bool MarkerFile::CanReadAsMarkerfile(string aFilename)
{
	EmFile test(aFilename);
	bool ok = test.OpenAndReadHeader();

	if (!ok) return ok;

	if (test.GetFileHeader().DimX != MARKERFILE_X_DIMENSION) return false;

	if (test.GetFileHeader().DimY < 1) return false;

	if (test.GetFileHeader().DimZ < 1) return false;

	return true;
}

float* MarkerFile::GetData()
{
	return (float*)EmFile::GetData();
}

int MarkerFile::GetMarkerCount()
{
	return _fileHeader.DimZ;
}

int MarkerFile::GetProjectionCount()
{
	int count = 0;
	for (int i = 0; i < _fileHeader.DimY; i++)
		if (CheckIfProjIndexIsGood(i)) count++;
	return count;
}

int MarkerFile::GetTotalProjectionCount()
{
	return _fileHeader.DimY;
}

Marker & MarkerFile::operator()(const int aProjection, const int aMarker)
{
	Marker* fdata = (Marker*)_data;

	return fdata[aMarker * _fileHeader.DimY + aProjection];
}

bool MarkerFile::CheckIfProjIndexIsGood(const int index)
{
	int mRefMarker = 0;
	return ((*this)(MFI_X_Coordinate, index, mRefMarker) > INVALID_MARKER_POSITION && (*this)(MFI_Y_Coordinate, index, mRefMarker) > INVALID_MARKER_POSITION);
}

void MarkerFile::AddMarker()
{
	int count = GetMarkerCount();
	float* newData = new float[MARKERFILE_X_DIMENSION  * (count + 1) * GetTotalProjectionCount()];
	memset(newData, 0, MARKERFILE_X_DIMENSION  * (count + 1) * GetTotalProjectionCount() * sizeof(float));
	memcpy(newData, _data, MARKERFILE_X_DIMENSION  * (count)* GetTotalProjectionCount() * sizeof(float));
	delete[] _data;
	_data = newData;
	_fileHeader.DimZ++;
	for (int itilt = 0; itilt < GetTotalProjectionCount(); itilt++)
	{
		(*this)(MFI_TiltAngle, itilt, count) = (*this)(MFI_TiltAngle, itilt, count - 1);
		(*this)(MFI_X_Coordinate, itilt, count) = INVALID_MARKER_POSITION;
		(*this)(MFI_Y_Coordinate, itilt, count) = INVALID_MARKER_POSITION;
	}
	delete[] mMarkerXPos;
	delete[] mMarkerYPos;
	delete[] mMarkerZPos;

	mMarkerXPos = new float[_fileHeader.DimZ];
	mMarkerYPos = new float[_fileHeader.DimZ];
	mMarkerZPos = new float[_fileHeader.DimZ];

	memset(mMarkerXPos, 0, _fileHeader.DimZ * sizeof(float));
	memset(mMarkerYPos, 0, _fileHeader.DimZ * sizeof(float));
	memset(mMarkerZPos, 0, _fileHeader.DimZ * sizeof(float));
}

void MarkerFile::RemoveMarker(int idx)
{
	int count = GetMarkerCount();
	if (count < 2)
		return; //we can't delete the last marker!

	if (idx < 0 || idx >= count)
		return; //idx out of bounds

	float* newData = new float[MARKERFILE_X_DIMENSION  * (count - 1) * GetTotalProjectionCount()];
	Marker* m = (Marker*)newData;
	
	for (int imark = 0; imark < count; imark++)
	{
		for (int itilt = 0; itilt < GetTotalProjectionCount(); itilt++)
		{	
			if (imark < idx)
			{
				m[imark * GetTotalProjectionCount() + itilt] = (*this)(itilt, imark);
			}
			if (imark > idx)
			{
				m[(imark - 1) * GetTotalProjectionCount() + itilt] = (*this)(itilt, imark);
			}
		}
	}
    delete[] _data;
    _data = newData;
    _fileHeader.DimZ--;


	delete[] mMarkerXPos;
	delete[] mMarkerYPos;
	delete[] mMarkerZPos;

	mMarkerXPos = new float[_fileHeader.DimZ];
	mMarkerYPos = new float[_fileHeader.DimZ];
	mMarkerZPos = new float[_fileHeader.DimZ];

	memset(mMarkerXPos, 0, _fileHeader.DimZ * sizeof(float));
	memset(mMarkerYPos, 0, _fileHeader.DimZ * sizeof(float));
	memset(mMarkerZPos, 0, _fileHeader.DimZ * sizeof(float));
}

void MarkerFile::SetMarkerPosition(int aProjection, int aMarkerIdx, float aPositionX, float aPositionY)
{
	if (aProjection < 0 || aProjection >= GetTotalProjectionCount())
		return;

	if (aMarkerIdx < 0 || aMarkerIdx >= GetMarkerCount())
		return;

	(*this)(MFI_X_Coordinate, aProjection, aMarkerIdx) = aPositionX;
	(*this)(MFI_Y_Coordinate, aProjection, aMarkerIdx) = aPositionY;
}

void MarkerFile::SetTiltAngles(float * aTiltAngles)
{
	for (int proj = 0; proj < GetTotalProjectionCount(); proj++)
	{
		for (int imark = 0; imark < GetMarkerCount(); imark++)
		{
			(*this)(MFI_TiltAngle, proj, imark) = aTiltAngles[proj];
		}
	}
}

std::vector<PointF> MarkerFile::GetMarkersAt(int aProjection)
{
	if (aProjection < 0 || aProjection >= GetTotalProjectionCount())
		return std::vector<PointF>();

	std::vector<PointF> ret(GetMarkerCount());

	for (int i = 0; i < GetMarkerCount(); i++)
	{
		ret.push_back(PointF((*this)(MFI_X_Coordinate, aProjection, i), (*this)(MFI_Y_Coordinate, aProjection, i)));
	}
	return ret;
}

std::vector<PointF> MarkerFile::GetAlignedMarkersAt(int aProjection)
{
	if (aProjection < 0 || aProjection >= GetTotalProjectionCount())
		return std::vector<PointF>();

	std::vector<PointF> ret(GetMarkerCount());

	for (int i = 0; i < GetMarkerCount(); i++)
	{
		ret.push_back(PointF((*this)(MFI_ProjectedCoordinateX, aProjection, i), (*this)(MFI_ProjectedCoordinateY, aProjection, i)));
	}
	return ret;
}

void MarkerFile::SetImageShift(int aProjection, float aShiftX, float aShiftY)
{
	if (aProjection < 0 || aProjection >= GetTotalProjectionCount())
		return;

	for (int i = 0; i < GetMarkerCount(); i++)
	{
		(*this)(MFI_X_Shift, aProjection, i) = aShiftX;
		(*this)(MFI_Y_Shift, aProjection, i) = aShiftY;
	}
}

void MarkerFile::SetConstantImageRotation(float aAngleInDeg)
{
	while (aAngleInDeg <= -180.0f)
	{
		aAngleInDeg += 360.0f;
	}
	while (aAngleInDeg > 180.0f)
	{
		aAngleInDeg -= 360.0f;
	}

	for (int imark = 0; imark < GetMarkerCount(); imark++)
	{
		for (int itilt = 0; itilt < GetTotalProjectionCount(); itilt++)
		{
			(*this)(MFI_RotationPsi, itilt, imark) = aAngleInDeg;
		}
	}
}

void MarkerFile::AlignSimple(int aReferenceMarker, int imageWidth, int imageHeight, float& aErrorScore, float& aImageRotation)
{

	float* x = new float[GetMarkerCount()];
	float* y = new float[GetMarkerCount()];
	float* z = new float[GetMarkerCount()];

	AlignSimple(aReferenceMarker, imageWidth, imageHeight, aErrorScore, aImageRotation, x, y, z);

	delete[] x;
	delete[] y;
	delete[] z;
}


void MarkerFile::Align3D(int aReferenceMarker, int width, int height, float& aErrorScore, float& aPhi, bool DoPsi, bool DoFixedPsi, bool DoTheta, bool DoPhi, bool DoMags, bool normMin, bool normZeroTilt,
                         bool magsFirst, int iterSwitch, int iterations, float addZShift, float* projErrorList, float* markerErrorList, float* aX, float* aY, float* aZ, void(*updateprog)(int percent))
{
    int MarkerCount = GetMarkerCount();
    int ProjectionCount = GetTotalProjectionCount();
    _fileHeader.ImageSizeX = width;
    _fileHeader.ImageSizeY = height;

    SetMagAnisotropy(_fileHeader.MagAnisotropyFactor, _fileHeader.MagAnisotropyAngle, _fileHeader.ImageSizeX, _fileHeader.ImageSizeY);

    float* x, *y, *z;
    x = mMarkerXPos;
    y = mMarkerYPos;
    z = mMarkerZPos;

    float rotation = 0;

    //start with simple old alignment to get good start values for minimization
    //TODO: change back
    AlignSimple(aReferenceMarker, width, height, aErrorScore, rotation, x, y, z);

    // Build alignment problem
    AlignmentProblem ali(MarkerCount, ProjectionCount, 2 * MarkerCount * ProjectionCount, (double)width, (double)height);

    // Set phi
    ali.set_phi(aPhi);

    // Set 3D locations
    for (int marker = 0; marker < MarkerCount; marker++){
        ali.set_point(marker, x[marker], y[marker], z[marker]);
    }

    // Set angles/shifts/mags
    for (int projection = 0; projection < ProjectionCount; projection++){
        ali.set_theta(projection, (*this)(MFI_TiltAngle, projection, 0));
        ali.set_psi(projection, (*this)(MFI_RotationPsi, projection, 0));
        ali.set_mag(projection, 1);
        ali.set_shift(projection,  (*this)(MFI_X_Shift, projection, 0), (*this)(MFI_Y_Shift, projection, 0));
    }

    // Add observations
    float mx, my;

    for (int projection = 0; projection < ProjectionCount; projection++){
        for (int marker = 0; marker < MarkerCount; marker++){
            if ((*this)(MFI_X_Coordinate, projection, marker) > INVALID_MARKER_POSITION && (*this)(MFI_Y_Coordinate, projection, marker) > INVALID_MARKER_POSITION){
                mx = (*this)(MFI_X_Coordinate, projection, marker);
                my = (*this)(MFI_Y_Coordinate, projection, marker);

                MoveXYToMagDistort(mx, my, mMagAnisotropyAmount, mMagAnisotropyAngle, mMagAnisotropy, (float)width, (float)height);

                ali.addObservation((double)mx, (double)my, marker, projection);
            }
        }
    }

    // Start the actual process
    Solver solver(&ali);
    float totalProgress = 1; //shifts are always "on"
    float progressCounter = 0;

    if (DoPsi)
    {
        totalProgress++;
    }
    if (DoMags | DoTheta)
    {
        totalProgress++;
    }
    if (DoPhi)
    {
        //phi[0] = 0.001f;
        totalProgress++;
    }
    totalProgress *= iterations;

    for (int iter = 0; iter < iterations; iter++)
    {
        bool doMagsLocal = DoMags;
        bool doThetaLocal = DoTheta;

        if (magsFirst)
        {
            if (iter <= iterSwitch)
            {
                doMagsLocal = DoMags;
                doThetaLocal = false;
            }
            else
            {
                doMagsLocal = false;
                doThetaLocal = DoTheta;
            }
        }
        else
        {
            if (iter <= iterSwitch)
            {
                doMagsLocal = false;
                doThetaLocal = DoTheta;
            }
            else
            {
                doMagsLocal = DoMags;
                doThetaLocal = false;
            }
        }

        //if only one of them is selected, don't care about a switch...
        if (DoMags ^ DoTheta)
        {
            doMagsLocal = DoMags;
            doThetaLocal = DoTheta;
        }

        //Console.WriteLine("Vor Shift");
        //MinimizeShift(x, y, z, thetas, psis, mags, phi[0], shiftsX, shiftsY, width, height);
        //MinimizeShift(&data);
        //TODO: change back
        solver.minimizeShifts();
        //Console.WriteLine("Nach Shift");
        progressCounter++;
        if (updateprog)
            (*updateprog)((int)(progressCounter / totalProgress * 100.0f));
        if (DoPhi)
        {
            //Console.WriteLine("Vor Phi");
            //MinimizePhi(x, y, z, thetas, psis, mags, phi, shiftsX, shiftsY, width, height);
            //MinimizePhi(&data);
            solver.minimizePhi();
            //Console.WriteLine("Nach Phi");
            progressCounter++;
            if (updateprog)
                (*updateprog)((int)(progressCounter / totalProgress * 100.0f));
        }
        if (DoPsi)
        {
            if (DoFixedPsi)
            {
                //MinimizePsiFixed(x, y, z, thetas, psis, mags, phi[0], shiftsX, shiftsY, width, height);
                //MinimizePsiFixed(&data);
                solver.minimizePsis(true);
            }
            else
            {
                //MinimizePsis(x, y, z, thetas, psis, mags, phi[0], shiftsX, shiftsY, width, height);
                //MinimizePsis(&data);
                solver.minimizePsis(false);
            }
            //Console.WriteLine("Vor Psi");
            //Console.WriteLine("Nach Psi");
            progressCounter++;
            if (updateprog)
                (*updateprog)((int)(progressCounter / totalProgress * 100.0f));
        }
        if (doMagsLocal)
        {
            //Console.WriteLine("Vor Mag");
            //MinimizeMags(x, y, z, thetas, psis, mags, phi[0], shiftsX, shiftsY, width, height);
            //MinimizeMags(&data);
            solver.minimizeMags();
            //Console.WriteLine("Nach Mag");

//            if (normZeroTilt)
//            {
//                int zeroTilt = GetMinTiltIndex();
//                double normVal = data.mags[zeroTilt];
//
//                for (int i = 0; i < ProjectionCount; i++)
//                {
//                    data.mags[i] /= normVal;
//                }
//            }
//            else
//            {
//                double normVal;
//                if (normMin)
//                    normVal = 100000;
//                else
//                    normVal = -100000;
//
//                for (int i = 0; i < ProjectionCount; i++)
//                {
//                    if (normMin)
//                        normVal = std::min(normVal, data.mags[i]);
//                    else
//                        normVal = std::max(normVal, data.mags[i]);
//                }
//
//                for (int i = 0; i < ProjectionCount; i++)
//                {
//                    data.mags[i] /= normVal;
//                }
//            }
            progressCounter++;
            if (updateprog)
                (*updateprog)((int)(progressCounter / totalProgress * 100.0f));
        }
        if (doThetaLocal)
        {
            //Console.WriteLine("Vor Theta");
            //MinimizeThetas(x, y, z, thetas, psis, mags, phi[0], shiftsX, shiftsY, width, height);
            //MinimizeThetas(&data);
            solver.minimizeThetas();
            //Console.WriteLine("Nach Theta");
            progressCounter++;
            if (updateprog)
                (*updateprog)((int)(progressCounter / totalProgress * 100.0f));
        }
    }

    // Get final values
    int ObservationCount = ali.ObservationCount();
    //int marker, projection;

    for (int obs = 0; obs < ObservationCount; obs++){
        int marker = ali.markerIndex(obs);
        int projection = ali.projectionIndex(obs);

        double theta;
        ali.get_theta(projection, &theta);
        (*this)(MFI_TiltAngle, projection, marker) = (float)theta;

        double psi;
        ali.get_psi(projection, &psi);
        (*this)(MFI_RotationPsi, projection, marker) = (float)psi;

        double mag;
        ali.get_mag(projection, &mag);
        (*this)(MFI_Magnifiaction, projection, marker) = (float)mag;

        double sx, sy;
        ali.get_shift(projection, &sx, &sy);
        (*this)(MFI_X_Shift, projection, marker) = (float)sx;
        (*this)(MFI_Y_Shift, projection, marker) = (float)sy;
    }

    for (int marker = 0; marker < MarkerCount; marker++){
        if (aX) {
            double X, Y, Z;
            ali.get_point(marker, &X, &Y, &Z);
            aX[marker] = (float)X;
            mMarkerXPos[marker] = (float)X;
            mMarkerYPos[marker] = (float)Y;
            mMarkerZPos[marker] = (float)Z;
            aY[marker] = (float)Y;
            aZ[marker] = (float)Z;
        }
    }

    //FILE* fp = fopen("data_out.bin", "wb");
    //fwrite(aX, sizeof(float), MarkerCount, fp);
    //fwrite(aY, sizeof(float), MarkerCount, fp);
    //fwrite(aZ, sizeof(float), MarkerCount, fp);
    //fclose(fp);
    //float* diffs = new float[ProjectionCount * MarkerCount * 2];
    //int* usedBeads = new int[ProjectionCount];

    //float* diffsum = new float[ProjectionCount];
    //float* projectedPos = new float[ProjectionCount*MarkerCount * 2];

    float rms = ali.computeRMS();
    //float rms = computeRMS(&data);
    aErrorScore = rms;
    //aPhi = data.phi * 180 / M_PI;


    //float* projBead = new float[ProjectionCount * MarkerCount];

    /*float* projErrorList = new float[ProjectionCount];
    float* markerErrorList = new float[MarkerCount];*/

    /*for (int projection = 0; projection < ProjectionCount; projection++)
    {
        for (int marker = 0; marker < MarkerCount; marker++)
        {
            projBead[projection * MarkerCount + marker] = -99999.0f;
            (*this)(MFI_ProjectedCoordinateX, projection, marker) = projectedPos[GetBeadIndex(projection, marker, 0, MarkerCount)];
            (*this)(MFI_ProjectedCoordinateY, projection, marker) = projectedPos[GetBeadIndex(projection, marker, 1, MarkerCount)];

            if ((*this)(MFI_X_Coordinate, projection, marker) > INVALID_MARKER_POSITION && (*this)(MFI_Y_Coordinate, projection, marker) > INVALID_MARKER_POSITION)
            {
                float x = (*this)(MFI_X_Coordinate, projection, marker);
                float y = (*this)(MFI_Y_Coordinate, projection, marker);

                MoveXYToMagDistort(x, y, mMagAnisotropyAmount, mMagAnisotropyAngle, mMagAnisotropy, width, height);

                float diffX = x - projectedPos[GetBeadIndex(projection, marker, 0, MarkerCount)];
                float diffY = y - projectedPos[GetBeadIndex(projection, marker, 1, MarkerCount)];

                projBead[projection * MarkerCount + marker] = sqrtf(diffX * diffX + diffY * diffY);
            }
        }
    }*/

//    if (projErrorList != NULL)
//    {
//        memcpy(projErrorList, data.errorPerProjection, data.ProjectionCount * sizeof(float));
//        /*for (int projection = 0; projection < ProjectionCount; projection++)
//        {
//            double counter = 0;
//            double sumError = 0;
//            for (int marker = 0; marker < MarkerCount; marker++)
//            {
//                if (projBead[projection * MarkerCount + marker] != -99999.0f)
//                {
//                    sumError += projBead[projection * MarkerCount + marker];
//                    counter++;
//                }
//
//            }
//            if (counter > 0)
//            {
//                projErrorList[projection] = sumError / counter; //mean Error per projection
//            }
//            else
//            {
//                projErrorList[projection] = 0;
//            }
//        }*/
//    }

//    if (markerErrorList != NULL)
//    {
//        memcpy(markerErrorList, data.errorPerMarker, data.MarkerCount * sizeof(float));
//        /*for (int marker = 0; marker < MarkerCount; marker++)
//        {
//            double counter = 0;
//            double sumError = 0;
//            for (int projection = 0; projection < ProjectionCount; projection++)
//            {
//                if (projBead[projection * MarkerCount + marker] != -99999.0f)
//                {
//                    sumError += projBead[projection * MarkerCount + marker];
//                    counter++;
//                }
//
//            }
//            if (counter > 0)
//            {
//                markerErrorList[marker] = sumError / counter; //mean Error per marker
//            }
//            else
//            {
//                markerErrorList[marker] = 0;
//            }
//        }*/
//    }

    double phi;
    ali.get_phi(&phi);
    _fileHeader.BeamDeclination = (float)phi;
    aPhi = (float)phi;
    _fileHeader.AlignmentScore = aErrorScore;
}

void MarkerFile::AlignSimple(int aReferenceMarker, int imageWidth, int imageHeight, float& aErrorScore, float& aImageRotation, float* x, float* y, float* z)
{
	if (aReferenceMarker < 0 || aReferenceMarker >= GetMarkerCount())
		return;

	int imintilt = GetMinTiltIndex();
	float r[3];
	r[0] = (*this)(MFI_X_Coordinate, imintilt, aReferenceMarker);
	r[1] = (*this)(MFI_Y_Coordinate, imintilt, aReferenceMarker);
	r[2] = (imageWidth / 2.0f + 1.0f);

	MoveXYToMagDistort(r[0], r[1], mMagAnisotropyAmount, mMagAnisotropyAngle, mMagAnisotropy, imageWidth, imageHeight);

	float* meanx = new float[GetMarkerCount()];
	float* meany = new float[GetMarkerCount()];
	float* norm = new float[GetMarkerCount()];
	memset(meanx, 0, GetMarkerCount() * sizeof(float));
	memset(meany, 0, GetMarkerCount() * sizeof(float));
	memset(norm, 0, GetMarkerCount() * sizeof(float));

	for (int imark = 0; imark < GetMarkerCount(); imark++)
	{
		for (int itilt = 0; itilt < GetTotalProjectionCount(); itilt++)
		{
			if ((*this)(MFI_X_Coordinate, itilt, imark) > INVALID_MARKER_POSITION
				&& (*this)(MFI_Y_Coordinate, itilt, aReferenceMarker) > INVALID_MARKER_POSITION)
			{
				float x = (*this)(MFI_X_Coordinate, itilt, imark);
				float y = (*this)(MFI_Y_Coordinate, itilt, imark);
				float x2 = (*this)(MFI_X_Coordinate, itilt, aReferenceMarker);
				float y2 = (*this)(MFI_Y_Coordinate, itilt, aReferenceMarker);

				MoveXYToMagDistort(x, y, mMagAnisotropyAmount, mMagAnisotropyAngle, mMagAnisotropy, imageWidth, imageHeight);
				MoveXYToMagDistort(x2, y2, mMagAnisotropyAmount, mMagAnisotropyAngle, mMagAnisotropy, imageWidth, imageHeight);
				meanx[imark] += x - x2;
				meany[imark] += y - y2;
				norm[imark] += 1;
			}
		}
	}

	for (int imark = 0; imark < GetMarkerCount(); imark++)
	{
		meanx[imark] /= norm[imark];
		meany[imark] /= norm[imark];
	}

	float sumxx = 0;
	float sumyy = 0;
	float sumxy = 0;

	for (int imark = 0; imark < GetMarkerCount(); imark++)
	{
		for (int itilt = 0; itilt < GetTotalProjectionCount(); itilt++)
		{
			if ((*this)(MFI_X_Coordinate, itilt, imark) > INVALID_MARKER_POSITION
				&& (*this)(MFI_X_Coordinate, itilt, aReferenceMarker) > INVALID_MARKER_POSITION)
			{
				float x = (*this)(MFI_X_Coordinate, itilt, imark);
				float y = (*this)(MFI_Y_Coordinate, itilt, imark);
				float x2 = (*this)(MFI_X_Coordinate, itilt, aReferenceMarker);
				float y2 = (*this)(MFI_Y_Coordinate, itilt, aReferenceMarker);

				MoveXYToMagDistort(x, y, mMagAnisotropyAmount, mMagAnisotropyAngle, mMagAnisotropy, imageWidth, imageHeight);
				MoveXYToMagDistort(x2, y2, mMagAnisotropyAmount, mMagAnisotropyAngle, mMagAnisotropy, imageWidth, imageHeight);

				float tempX = x - x2 - meanx[imark];
				sumxx += tempX * tempX;

				float tempY = y - y2 - meany[imark];
				sumyy += tempY * tempY;

				sumxy += tempX * tempY;
			}
		}
	}

	double psi = 0.5 * atan(2.0 * sumxy / (sumxx - sumyy));
	double sign = signbit(psi) ? -1 : 1;
	if (sumxx > sumyy)
		psi = psi - 0.5 * M_PI * sign;

	aImageRotation = (float)(psi * 180 / M_PI);

	double cpsi = cos(psi);
	double spsi = sin(psi);

	double ndif = 0;
	sumxx = 0;

	for (int imark = 0; imark < GetMarkerCount(); imark++)
	{
		for (int itilt = 0; itilt < GetTotalProjectionCount(); itilt++)
		{
			if ((*this)(MFI_X_Coordinate, itilt, imark) > INVALID_MARKER_POSITION
				&& (*this)(MFI_X_Coordinate, itilt, aReferenceMarker) > INVALID_MARKER_POSITION)
			{
				if (imark != aReferenceMarker) ndif += 1;
				float x = (*this)(MFI_X_Coordinate, itilt, imark);
				float y = (*this)(MFI_Y_Coordinate, itilt, imark);
				float x2 = (*this)(MFI_X_Coordinate, itilt, aReferenceMarker);
				float y2 = (*this)(MFI_Y_Coordinate, itilt, aReferenceMarker);

				MoveXYToMagDistort(x, y, mMagAnisotropyAmount, mMagAnisotropyAngle, mMagAnisotropy, imageWidth, imageHeight);
				MoveXYToMagDistort(x2, y2, mMagAnisotropyAmount, mMagAnisotropyAngle, mMagAnisotropy, imageWidth, imageHeight);
				(*this)(MFI_DevOfMark, itilt, imark) = (float)
					((x - x2 - meanx[imark]) * cpsi +
					(y - y2 - meany[imark]) * spsi);
				sumxx += (*this)(MFI_DevOfMark, itilt, imark) * (*this)(MFI_DevOfMark, itilt, imark);
			}
		}
	}
	double sigma = sqrt(sumxx / (ndif - GetMarkerCount()));

	aErrorScore = (float)sigma;
		
	std::vector<float> wertung;
	std::vector<int> index;

	for (int i = 0; i < GetTotalProjectionCount(); i++)
	{
		wertung.push_back(0);
		for (int imark = 0; imark < GetMarkerCount(); imark++)
		{
			wertung[i] += fabs((*this)(MFI_DevOfMark, i, imark));
		}
		index.push_back(i);
	}


	for (int i = 0; i < GetTotalProjectionCount() - 1; i++)
	{
		for (int sort = 0; sort < GetTotalProjectionCount() - i - 1; sort++)
		{
			if (wertung[sort] > wertung[sort + 1])
			{
				int tempi = index[sort];
				float tempf = wertung[sort];

				index[sort] = index[sort + 1];
				wertung[sort] = wertung[sort + 1];
				index[sort + 1] = tempi;
				wertung[sort + 1] = tempf;
			}
		}
	}
	
	//2nd part: determination of shifts:
	float* theta = new float[GetTotalProjectionCount()];
	float* stheta = new float[GetTotalProjectionCount()];
	float* ctheta = new float[GetTotalProjectionCount()];
	
	for (int i = 0; i < GetTotalProjectionCount(); i++)
	{
		theta[i] = (float)((*this)(MFI_TiltAngle, i, 0) * M_PI / 180.0);
		stheta[i] = (float)sin(theta[i]);
		ctheta[i] = (float)cos(theta[i]);
	}
	
	for (int imark = 0; imark < GetMarkerCount(); imark++)
	{
		float P[3][3];
		sumxx = 0;
		sumyy = 0;
		sumxy = 0;
		float sumyx = 0;
		float salpsq = 0;
		float scalph = 0;
		norm[imark] = 0;
		float temp[3];

		for (int itilt = 0; itilt < GetTotalProjectionCount(); itilt++)
		{
			if ((*this)(MFI_X_Coordinate, itilt, imark) > INVALID_MARKER_POSITION
				&& (*this)(MFI_X_Coordinate, itilt, aReferenceMarker) > INVALID_MARKER_POSITION)
			{
				norm[imark] += 1;
				salpsq += stheta[itilt] * stheta[itilt];
				scalph += ctheta[itilt] * stheta[itilt];
				float x = (*this)(MFI_X_Coordinate, itilt, imark);
				float y = (*this)(MFI_Y_Coordinate, itilt, imark);
				float x2 = (*this)(MFI_X_Coordinate, itilt, aReferenceMarker);
				float y2 = (*this)(MFI_Y_Coordinate, itilt, aReferenceMarker);

				MoveXYToMagDistort(x, y, mMagAnisotropyAmount, mMagAnisotropyAngle, mMagAnisotropy, imageWidth, imageHeight);
				MoveXYToMagDistort(x2, y2, mMagAnisotropyAmount, mMagAnisotropyAngle, mMagAnisotropy, imageWidth, imageHeight);
				sumxx += (x - x2) * ctheta[itilt];
				sumyy += (y - y2) * ctheta[itilt];
				sumxy += (x - x2) * stheta[itilt];
				sumyx += (y - y2) * stheta[itilt];
			}
		}
		P[0][0] = norm[imark] - (float)(salpsq * spsi * spsi);
		P[0][1] = salpsq * (float)(cpsi * cpsi);
		P[0][2] = scalph * (float)spsi;
		P[1][0] = P[0][1];
		P[1][1] = norm[imark] - salpsq * (float)(cpsi * cpsi);
		P[1][2] = -scalph * (float)cpsi;
		P[2][0] = P[0][2];
		P[2][1] = P[1][2];
		P[2][2] = salpsq;
		float dt = det(P);

		temp[0] = (float)((sumxx * spsi - sumyy * cpsi) * spsi + (cpsi * meanx[imark] + spsi * meany[imark]) * cpsi * norm[imark]);
		temp[1] = (float)(-(sumxx * spsi - sumyy * cpsi) * cpsi + (cpsi * meanx[imark] + spsi * meany[imark]) * spsi * norm[imark]);
		temp[2] = (float)(sumxy * spsi - sumyx * cpsi);

		if (dt != 0)
		{
			//float[, ] P_t = copy(P);
			float P_t[3][3];
			memcpy(P_t, P, 9 * sizeof(float));
			P_t[0][0] = temp[0];
			P_t[1][0] = temp[1];
			P_t[2][0] = temp[2];
			x[imark] = det(P_t) / dt;

			//P_t = copy(P);
			memcpy(P_t, P, 9 * sizeof(float));
			P_t[0][1] = temp[0];
			P_t[1][1] = temp[1];
			P_t[2][1] = temp[2];
			y[imark] = det(P_t) / dt;

			//P_t = copy(P);
			memcpy(P_t, P, 9 * sizeof(float));
			P_t[0][2] = temp[0];
			P_t[1][2] = temp[1];
			P_t[2][2] = temp[2];
			z[imark] = det(P_t) / dt;

			x[imark] += r[0] - imageWidth / 2 + 1;
            y[imark] += r[1] - imageHeight / 2 + 1;
			z[imark] += r[2] - imageWidth / 2 + 1;
		}
	}

	for (int itilt = 0; itilt < GetTotalProjectionCount(); itilt++)
	{
		for (int imark = 0; imark < GetMarkerCount(); imark++)
		{
			float shiftX, shiftY;
			shiftX = (float)(x[imark] * (spsi * spsi * ctheta[itilt] + cpsi * cpsi) +
				y[imark] * spsi * cpsi * (1 - ctheta[itilt]) +
				z[imark] * spsi * stheta[itilt] + imageWidth / 2 + 1);

			shiftY = (float)(x[imark] * spsi * cpsi * (1 - ctheta[itilt]) +
				y[imark] * (cpsi * cpsi * ctheta[itilt] + spsi * spsi) -
                z[imark] * cpsi * stheta[itilt] + imageHeight / 2 + 1);
			float x = (*this)(MFI_X_Coordinate, itilt, imark);
			float y = (*this)(MFI_Y_Coordinate, itilt, imark);

			MoveXYToMagDistort(x, y, mMagAnisotropyAmount, mMagAnisotropyAngle, mMagAnisotropy, imageWidth, imageHeight);

            (*this)(MFI_X_Shift, itilt, imark) = x - shiftX;
            (*this)(MFI_Y_Shift, itilt, imark) = y - shiftY;
			(*this)(MFI_RotationPsi, itilt, imark) = aImageRotation;
		}
	}


    for (int imark = 0; imark < GetMarkerCount(); imark++)
    {
            z[imark] = -z[imark];
    }


	delete[] meanx;
	delete[] meany;
	delete[] norm;
	delete[] theta;
	delete[] stheta;
	delete[] ctheta;
}

int MarkerFile::GetMinTiltIndex()
{
	int mintiltIndex = -1;
	float mintilt = FLT_MAX;

	for (int i = 0; i < GetTotalProjectionCount(); i++)
	{
		if (fabs((*this)(MFI_TiltAngle, i, 0)) < mintilt)
		{
			mintiltIndex = i;
			mintilt = fabs((*this)(MFI_TiltAngle, i, 0));
		}
	}
	return mintiltIndex;
}

bool MarkerFile::Save(string aFileName)
{
	_fileHeader.IsNewHeaderFormat = 1;
	_fileHeader.MarkerOffset = sizeof(_fileHeader) + _fileHeader.DimX * _fileHeader.DimY * _fileHeader.DimZ * sizeof(float);
	FileWriter::mFileName = aFileName;

	bool check = EmFile::CanWriteAsEM(_fileHeader.DimX, _fileHeader.DimY, _fileHeader.DimZ, GetDataType());
	if (!check)
		return false;

	check &= InitHeader(aFileName, _fileHeader);
	check &= WriteRawData(aFileName, _data, GetDataSize());

	check &= OpenWrite(false);
	FileWriter::Seek(_fileHeader.MarkerOffset, ios::beg);
	for (size_t i = 0; i < GetMarkerCount(); i++)
	{
		WriteLE(mMarkerXPos[i]);
		WriteLE(mMarkerYPos[i]);
		WriteLE(mMarkerZPos[i]);
	}
	CloseWrite();
	return check;
}

void MarkerFile::SetMagAnisotropy(float aAmount, float angInDeg, float dimX, float dimY)
{
	_fileHeader.MagAnisotropyAngle = angInDeg;
	_fileHeader.MagAnisotropyFactor = aAmount;

	mMagAnisotropyAmount = aAmount;
	mMagAnisotropyAngle = angInDeg / 180.0f * M_PI;

	//Matrix<float> shiftCenter(3, 3);
	//Matrix<float> shiftBack(3, 3);
	//Matrix<float> rotMat1 = Matrix<float>::GetRotationMatrix3DZ(mMagAnisotropyAngle);
	//Matrix<float> rotMat2 = Matrix<float>::GetRotationMatrix3DZ(-mMagAnisotropyAngle);
	//Matrix<float> stretch(3, 3);
	Eigen::Matrix3f shiftCenter;
	Eigen::Matrix3f shiftBack;
	Eigen::Matrix3f rotMat1;
	Eigen::Matrix3f rotMat2;
	Eigen::Matrix3f stretch;

	rotMat1(0,0) = cos(mMagAnisotropyAngle); rotMat1(0,1) = -sin(mMagAnisotropyAngle); rotMat1(0,2) = 0;
	rotMat1(1,0) = sin(mMagAnisotropyAngle); rotMat1(1,1) = cos(mMagAnisotropyAngle) ; rotMat1(1,2) = 0;
	rotMat1(2,0) = 0; 			 rotMat1(2,1) = 0; 			   rotMat1(2,2) = 1;

	rotMat2(0,0) = cos(-mMagAnisotropyAngle); rotMat2(0,1) = -sin(-mMagAnisotropyAngle); rotMat2(0,2) = 0;
	rotMat2(1,0) = sin(-mMagAnisotropyAngle); rotMat2(1,1) = cos(-mMagAnisotropyAngle) ; rotMat2(1,2) = 0;
	rotMat2(2,0) = 0; 			  rotMat2(2,1) = 0; 			     rotMat2(2,2) = 1;

	shiftCenter(0, 0) = 1;
	shiftCenter(0, 1) = 0;
	shiftCenter(0, 2) = -dimX / 2.0f;
	shiftCenter(1, 0) = 0;
	shiftCenter(1, 1) = 1;
	shiftCenter(1, 2) = -dimY / 2.0f;
	shiftCenter(2, 0) = 0;
	shiftCenter(2, 1) = 0;
	shiftCenter(2, 2) = 1;

	shiftBack(0, 0) = 1;
	shiftBack(0, 1) = 0;
	shiftBack(0, 2) = dimX / 2.0f;
	shiftBack(1, 0) = 0;
	shiftBack(1, 1) = 1;
	shiftBack(1, 2) = dimY / 2.0f;
	shiftBack(2, 0) = 0;
	shiftBack(2, 1) = 0;
	shiftBack(2, 2) = 1;

	stretch(0, 0) = aAmount;
	stretch(0, 1) = 0;
	stretch(0, 2) = 0;
	stretch(1, 0) = 0;
	stretch(1, 1) = 1;
	stretch(1, 2) = 0;
	stretch(2, 0) = 0;
	stretch(2, 1) = 0;
	stretch(2, 2) = 1;

	mMagAnisotropy = shiftBack * rotMat2 * stretch * rotMat1 * shiftCenter;
}

void MarkerFile::GetMagAnisotropy(float & aAmount, float & angInDeg)
{
	aAmount = 1.0f;
	angInDeg = 0;

	if (_fileHeader.IsNewHeaderFormat)
	{
		aAmount = _fileHeader.MagAnisotropyFactor;
		angInDeg = _fileHeader.MagAnisotropyAngle;
	}
}

void MarkerFile::GetBeamDeclination(float & aPhi)
{
	aPhi = 0;

	if (_fileHeader.IsNewHeaderFormat)
	{
		aPhi = _fileHeader.BeamDeclination;
	}
}


string MarkerFile::GetFilename()
{
	return FileWriter::mFileName;
}

void MarkerFile::GetMarkerPositions(float * xPos, float * yPos, float * zPos)
{
	if (_fileHeader.IsNewHeaderFormat)
	{
		for (int i = 0; i < GetMarkerCount(); i++)
		{
			xPos[i] = mMarkerXPos[i];
			yPos[i] = mMarkerYPos[i];
			zPos[i] = mMarkerZPos[i];
		}
	}
	else
	{
		for (int i = 0; i < GetMarkerCount(); i++)
		{
			xPos[i] = 0;
			yPos[i] = 0;
			zPos[i] = 0;
		}
	}
}

void MarkerFile::DeleteAllMarkersInProjection(int idx)
{
	if (idx < 0 || idx >= GetTotalProjectionCount())
		return;

	for (int imark = 0; imark < GetMarkerCount(); imark++)
	{
		(*this)(MFI_X_Coordinate, idx, imark) = INVALID_MARKER_POSITION;
		(*this)(MFI_Y_Coordinate, idx, imark) = INVALID_MARKER_POSITION;
	}
}

MarkerFile * MarkerFile::ImportFromIMOD(string aFilename)
{
	ImodFiducialFile imod(aFilename);
	if (!imod.OpenAndRead())
		return NULL;

	int projCount = imod.GetProjectionCount();
	vector<float> tilts = imod.GetTiltAngles();

	MarkerFile* m = new MarkerFile(projCount, &tilts[0]);
	for (size_t i = 1; i < imod.GetMarkerCount(); i++) //first marker is already there...
	{
		m->AddMarker();
	}

	for (size_t p = 0; p < imod.GetProjectionCount(); p++)
	{
		vector<PointF> pos = imod.GetMarkers(p);
		for (size_t i = 0; i < imod.GetMarkerCount(); i++)
		{
			(*m)(MFI_X_Coordinate, p, i) = pos[i].x;
			(*m)(MFI_Y_Coordinate, p, i) = pos[i].y;
		}
	}
	
	return m;
}

float MarkerFile::det(float P[3][3])
{
	return P[0][0] * P[1][1] * P[2][2] + P[0][1] * P[1][2] * P[2][0] + P[0][2] * P[1][0] * P[2][1] - P[0][2] * P[1][1] * P[2][0] - P[0][1] * P[1][0] * P[2][2] - P[0][0] * P[1][2] * P[2][1];
}

float& MarkerFile::operator() (const MarkerFileItem_enum aItem, const int aProjection, const int aMarker)
{
	float* fdata = (float*)_data;

	return fdata[aMarker * _fileHeader.DimX * _fileHeader.DimY + aProjection * _fileHeader.DimX + aItem];
}

void MarkerFile::MoveXYToMagDistort(float & x, float & y, float amplitude, float angleInRad, Eigen::Matrix3f& m, float dimX, float dimY)
{
	if (amplitude == 1)
		return;

    //Matrix<float> vec(3, 1);
	Eigen::Vector3f vec;
	vec(0, 0) = x;
	vec(1, 0) = y;
	vec(2, 0) = 1;
	
    //Matrix<float> erg = m * vec;
	Eigen::RowVector3f erg = m * vec;
	x = erg(0, 0);
	y = erg(0, 1);
}
