// MarkerAlignator.cpp : Defines the entry point for the console application.
//

#include "MarkerFile.h"
#include "getopt_pp.h"
#include <stdio.h>

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

using namespace std;
using namespace GetOpt;

static void status(int percent)
{
    int val = (int) (percent);
    int lpad = (int) (percent/100. * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
	//cout << percent << "%" << endl;
}

static void show_usage(){
    string msg = "ceresAligner aligns tomographic tilt series based on fiducial marker positions on the projection images.\n"
                 "\n"
                 "Usage:\n"
                 "\tceresAligner -input FILE -output FILE [OPTIONAL PARAMETERS]\n"
                 "\n"
                 "I/O (required):\n"
                 "\t-i; --input \t\t Input filename.\n"
                 "\t-o; --output \t\t Output filename.\n"
                 "\n"
                 "Alignment parameters:\n"
                 "\t-a; --activeMarker \t Marker to center alignment around (default 0).\n"
                 "\t-w; --width \t\t Projection width in pixels (default 7420).\n"
                 "\t-h; --height \t\t Projection height in pixels (default 7676).\n"
                 "\t--maAmount \t\t Magnification Anisotropy amount (default 1).\n"
                 "\t--maAangle \t\t Magnification Anisotropy angle in degrees (default 0).\n"
                 "\t--phi \t\t\t Beam declination angle in degrees (default 0).\n"
                 "\t--iter \t\t\t Number of iterations to run (default 20).\n"
                 "\t--iterSwitch \t\t Iteration after which to switch between theta/mags (default 10).\n"
                 "\t--zShift \t\t Additional z-shift applied during alignment.\n"
                 "\n"
                 "Alignment types:\n"
                 "\t--doPsi \t\t Align for image rotation.\n"
                 "\t--doFixedPsi \t\t Keep image rotation constant for all projections.\n"
                 "\t--doTheta \t\t Align tilt angles.\n"
                 "\t--doPhi \t\t Align for beam declination.\n"
                 "\t--doMags \t\t Align for magnification change.\n"
                 "\t--normMin \t\t Normalize mag change by minimum value\n"
                 "\t--normZeroTile \t\t Normalize mag change by 0-deg projection\n"
                 "\t--magsFirst \t\t Align for mag change first, then tilt angles\n";

    cout << msg << endl;
}

static void print_params(int& activeMarker, int& width, int& height, float& phi, bool& doPsi, bool& doFixedPsi, bool& doTheta,
                         bool& doPhi, bool& doMags, bool& normMin, bool& normZeroTilt, bool& magsFirst, int& iterSwitch, int& iter, float& zShift){

    string msg = "Alignment parameters:\n"
                 "\t Active marker:\t\t %d\n"
                 "\t Projection dimensions:\t %d x %d\n"
                 "\t Beam declination:\t %f\n"
                 "\t Additional z-shift:\t %f\n"
                 "\t Iterations:\t\t %d\n"
                 "\t Switching after:\t %d\n"
                 "\n"
                 "Aligning for:\n"
                 "\t Image rotation:\t\t %s (%s)\n"
                 "\t Tilt angles:\t\t\t %s\n"
                 "\t Beam declination:\t\t %s\n"
                 "\t Magnification change:\t\t %s\n"
                 "\t\t normalized by minimum:\t\t %s\n"
                 "\t\t normalized by maximum:\t\t %s\n"
                 "\t\t normalized by 0-deg projection: %s\n";

    printf(msg.c_str(),
            activeMarker,
            width, height,
            phi,
            zShift,
            iter,
            iterSwitch,
            doPsi?"true":"false", doFixedPsi?"fixed":"not fixed",
            doTheta?"true":"false",
            doPhi?"true":"false",
            doMags?"true":"false",
            normMin?"true":"false",
            !(normMin || normZeroTilt)?"true":"false",
            normZeroTilt?"true":"false");
}

int main(int argc, char* argv[])
{
    // file names
    string inputName = "";
    string outputName = "";

    // alignment parameters
    int activeMarker = 0;
    int width = 7420;
    int height = 7676;
    float magAnisotropyAmount = 1.;
    float magAnisotropyAngle = 0.;
    float phi = 0;
    int iter = 20;
    int iterSwitch = 10;
    float zShift = 0;

    // alignment type
    bool doPsi = true;
    bool doFixedPsi = false;
    bool doTheta = true;
    bool doPhi = false;
    bool doMags = false;
    bool normMin = false;
    bool normZeroTilt = false;
    bool magsFirst = false;

    GetOpt_pp ops(argc, argv);

    if (ops >> OptionPresent("help")){
        show_usage();
        return 0;
    }

    if (!(ops >> OptionPresent('i', "input")) || !(ops >> OptionPresent('o', "output"))){
        cout << "Input and output names are required parameters." << endl;
        return -1;
    }

    doPsi = (ops >> OptionPresent("doPsi"));
    doFixedPsi = (ops >> OptionPresent("doFixedPsi"));
    doTheta = (ops >> OptionPresent("doTheta"));
    doPhi = (ops >> OptionPresent("doPhi"));
    doMags = (ops >> OptionPresent("doMags"));
    normMin = (ops >> OptionPresent("normMin"));
    normZeroTilt = (ops >> OptionPresent("normZeroTilt"));
    magsFirst = (ops >> OptionPresent("magsFirst"));

    ops >> Option('i', "input", inputName)
        >> Option('o', "output", outputName)
        >> Option('a', "activeMarker", activeMarker)
        >> Option('w', "width", width)
        >> Option('h', "height", height)
        >> Option("maAmount", magAnisotropyAmount)
        >> Option("maAngle", magAnisotropyAngle)
        >> Option("phi", phi)
        >> Option("iter", iter)
        >> Option("iterSwitch", iterSwitch)
        >> Option("zShift", zShift);

	MarkerFile marker(inputName);
	float error = 0;
	cout << "Using marker #" << activeMarker+1 << endl;
	auto* perProj = new float[marker.GetProjectionCount()];
    auto* perMarker = new float[marker.GetMarkerCount()];
    auto* x = new float[marker.GetMarkerCount()];
    auto* y = new float[marker.GetMarkerCount()];
    auto* z = new float[marker.GetMarkerCount()];
    //int width = 927;
    //int height = 959;
	//marker.SetMagAnisotropy(1.016f, 42.0f, 3720*2,3838*2);
    marker.SetMagAnisotropy(magAnisotropyAmount, magAnisotropyAngle, (float)width, (float)height);

	//cout << "Aligning for beam declination ..." << endl;
	//marker.Align3D(activeMarker, width, height, error, phi, true, true, false, true, false, false, false, false, 0, 20, 0, perProj, perMarker, x, y, z, &status);
	//cout << "Phi: " << phi << endl;
	//cout << "Error score: " << error << endl;

    print_params(activeMarker, width, height, phi, doPsi, doFixedPsi, doTheta, doPhi, doMags, normMin, normZeroTilt, magsFirst, iterSwitch, iter, zShift);
    cout << "Starting Alignment ..." << endl;
    marker.Align3D(activeMarker, width, height, error, phi, doPsi, doFixedPsi, doTheta, doPhi, doMags, normMin, normZeroTilt, magsFirst, iterSwitch, iter, 0, perProj, perMarker, x, y, z, &status);
    cout << endl;
    cout << "Phi: " << phi << endl;
    cout << "Error score: " << error << endl;

    //cout << "Aligning for tilt angles and image rotation ..." << endl;
    //marker.Align3D(activeMarker, width, height, error, phi, true, false, true, false, false, false, false, false, 5, 20, 0, perProj, perMarker, x, y, z, &status);
	//cout << "Error score: " << error << endl;
	string name(outputName);
	//cout << "Name: " << name << endl;
	marker.Save(name);

    return 0;
}

