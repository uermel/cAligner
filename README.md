# cAligner
3D alignment of cryo electron tomographic tilt series using Google's [Ceres](https://github.com/ceres-solver/ceres-solver) 
for non-linear least squares optimization. 

Based on the [Artiatomi](https://github.com/uermel/Artiatomi) package for processing cryo-tomography data.
Using the acquisition model described in:

Castaño-Díez, D., Seybert, A., Frangakis, A.S. (2006). _[Tilt-series and electron microscope alignment for the correction of the non-perpendicularity of beam and tilt-axis.](https://doi.org/10.1016/j.jsb.2005.12.009)_ Journal of Structural Biology.

Uses [getoptpp](https://code.google.com/archive/p/getoptpp/) for command line input.

## Dependencies

* Ceres 1.14 ([installation guide](http://ceres-solver.org/installation.html))

## Build
cAligner can be built using cmake. 

```bash
git clone https://github.com/uermel/cAligner.git
cd cAligner
mkdir build
cd build
cmake ..
make
```

## Usage

```bash
$ ./cAligner --help
cAligner aligns tomographic tilt series based on fiducial marker positions on the projection images.

Usage:
	cAligner -input FILE -output FILE [OPTIONAL PARAMETERS]

I/O (required):
	-i; --input 		 Input filename.
	-o; --output 		 Output filename.

Alignment parameters:
	-a; --activeMarker 	 Marker to center alignment around (default 0).
	-w; --width 		 Projection width in pixels (default 7420).
	-h; --height 		 Projection height in pixels (default 7676).
	--maAmount 		 Magnification Anisotropy amount (default 1).
	--maAangle 		 Magnification Anisotropy angle in degrees (default 0).
	--phi 			 Beam declination angle in degrees (default 0).
	--iter 			 Number of iterations to run (default 20).
	--iterSwitch 		 Iteration after which to switch between theta/mags (default 10).
	--zShift 		 Additional z-shift applied during alignment.

Alignment types:
	--doPsi 		 Align for image rotation.
	--doFixedPsi 		 Keep image rotation constant for all projections.
	--doTheta 		 Align tilt angles.
	--doPhi 		 Align for beam declination.
	--doMags 		 Align for magnification change.
	--normMin 		 Normalize mag change by minimum value
	--normZeroTile 		 Normalize mag change by 0-deg projection
	--magsFirst 		 Align for mag change first, then tilt angles

```
