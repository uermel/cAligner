//
// Created by uermel on 9/16/19.
//


#include "/usr/include/glog/logging.h"
#include "AlignmentProblem.h"
#include <math.h>

#define INVALID_MARKER_POSITION -1

AlignmentProblem::AlignmentProblem(int MarkerCount, int ProjectionCount, int maxObservationCount, double imDimX, double imDimY) {
    MarkerCount_ = MarkerCount;
    MarkerIndex_ = new int[maxObservationCount];

    ProjectionCount_ = ProjectionCount;
    ProjectionIndex_ = new int[maxObservationCount];

    ObservationCount_ = 0;
    observations_ = new double[2 * maxObservationCount];

    ParameterCount_ = 3 * MarkerCount_ + 5 * ProjectionCount_ + 1;
    parameters_ = new double[ParameterCount_];

    imDimXhalf_ = imDimX / 2.;
    imDimYhalf_ = imDimY / 2.;
}

AlignmentProblem::~AlignmentProblem() {
        delete[] MarkerIndex_;
        delete[] ProjectionIndex_;
        delete[] observations_;
        delete[] parameters_;
}

void AlignmentProblem::set_point(int marker, double x, double y, double z){
    mutable_points()[0 + marker * 3] = x;
    mutable_points()[1 + marker * 3] = y;
    mutable_points()[2 + marker * 3] = z;
}

void AlignmentProblem::set_shift(int projection, double x, double y){
    mutable_shifts()[0 + projection * 2] = x;
    mutable_shifts()[1 + projection * 2] = y;
}

void AlignmentProblem::set_theta(int projection, double theta){
    mutable_thetas()[projection] = theta / 180.0 * M_PI;
}

void AlignmentProblem::set_psi(int projection, double psi){
    mutable_psis()[projection] = psi / 180.0 * M_PI;
}

void AlignmentProblem::set_mag(int projection, double mag){
    mutable_mags()[projection] = mag;
}

void AlignmentProblem::set_phi(double phi){
    mutable_phi()[0] = phi / 180.0 * M_PI;
}

void AlignmentProblem::addObservation(double x, double y, int marker, int projection) {

    int idx = ObservationCount_;

    observations_[0 + idx * 2] = x - imDimXhalf_;
    observations_[1 + idx * 2] = y - imDimYhalf_;

    MarkerIndex_[idx] = marker;
    ProjectionIndex_[idx] = projection;

    ObservationCount_++;
}

int AlignmentProblem::ObservationCount()          const { return ObservationCount_; }
const double* AlignmentProblem::observations()    const { return observations_; }
double* AlignmentProblem::mutable_points()              { return parameters_; }
double* AlignmentProblem::mutable_shifts()              { return parameters_ + 3 * MarkerCount_; }
double* AlignmentProblem::mutable_thetas()              { return parameters_ + 3 * MarkerCount_ + 2 * ProjectionCount_; }
double* AlignmentProblem::mutable_psis()                { return parameters_ + 3 * MarkerCount_ + 3 * ProjectionCount_; }
double* AlignmentProblem::mutable_mags()                { return parameters_ + 3 * MarkerCount_ + 4 * ProjectionCount_; }
double* AlignmentProblem::mutable_phi()                 { return parameters_ + 3 * MarkerCount_ + 5 * ProjectionCount_; }

double* AlignmentProblem::mutable_point_for_observation(int i) {
    return mutable_points() + MarkerIndex_[i] * 3;
}

double* AlignmentProblem::mutable_shift_for_observation(int i) {
    return mutable_shifts() + ProjectionIndex_[i] * 2;
}

double* AlignmentProblem::mutable_theta_for_observation(int i) {
    return mutable_thetas() + ProjectionIndex_[i];
}

double* AlignmentProblem::mutable_psi_for_observation(int i, bool fixed) {
    if (fixed){
        return mutable_psis();
    }
    else {
        return mutable_psis() + ProjectionIndex_[i];
    }

}

double* AlignmentProblem::mutable_mag_for_observation(int i) {
    return mutable_mags() + ProjectionIndex_[i];
}

double* AlignmentProblem::mutable_phi_for_observation(int i) {
    return mutable_phi();
}

void AlignmentProblem::get_point(int marker, double* x, double* y, double* z){
    x[0] = mutable_points()[0 + marker * 3];
    y[0] = mutable_points()[1 + marker * 3];
    z[0] = mutable_points()[2 + marker * 3];
}

void AlignmentProblem::get_shift(int projection, double* x, double* y){
    x[0] = mutable_shifts()[0 + projection * 2];
    y[0] = mutable_shifts()[1 + projection * 2];
}

void AlignmentProblem::get_theta(int projection, double* theta){
    theta[0] = mutable_thetas()[projection] * 180 / M_PI;
}

void AlignmentProblem::get_psi(int projection, double* psi){
    psi[0] = mutable_psis()[projection] * 180 / M_PI;
}

void AlignmentProblem::get_mag(int projection, double* mag){
    mag[0] = mutable_mags()[projection];
}

void AlignmentProblem::get_phi(double* phi){
    phi[0] = mutable_phi()[0] * 180 / M_PI;
}

void AlignmentProblem::get_point_obs(int obs, double* x, double* y, double* z){
    int marker = MarkerIndex_[obs];
    x[0] = mutable_points()[0 + marker * 3];
    y[0] = mutable_points()[1 + marker * 3];
    z[0] = mutable_points()[2 + marker * 3];
}

void AlignmentProblem::get_shift_obs(int obs, double* x, double* y){
    int projection = ProjectionIndex_[obs];
    x[0] = mutable_shifts()[0 + projection * 2];
    y[0] = mutable_shifts()[1 + projection * 2];
}

void AlignmentProblem::get_theta_obs(int obs, double* theta){
    int projection = ProjectionIndex_[obs];
    theta[0] = mutable_thetas()[projection] * 180 / M_PI;
}

void AlignmentProblem::get_psi_obs(int obs, double* psi){
    int projection = ProjectionIndex_[obs];
    psi[0] = mutable_psis()[projection] * 180 / M_PI;
}

void AlignmentProblem::get_mag_obs(int obs, double* mag){
    int projection = ProjectionIndex_[obs];
    mag[0] = mutable_mags()[projection];
}

int AlignmentProblem::markerIndex(int obs) { return MarkerIndex_[obs]; }
int AlignmentProblem::projectionIndex(int obs) { return ProjectionIndex_[obs]; }

float AlignmentProblem::computeRMS() {

    double sum = 0;

    for (int obs = 0; obs < ObservationCount_; obs++) {
        double res[2] = {0.,0.};

        ReprojectionError error(observations()[0 + obs * 2], observations()[1 + obs * 2]);
        error(mutable_point_for_observation(obs),
              mutable_shift_for_observation(obs),
              mutable_theta_for_observation(obs),
              mutable_psi_for_observation(obs, false),
              mutable_mag_for_observation(obs),
              mutable_phi_for_observation(obs),
              res);

        sum += res[0] * res[0] + res[1] * res[1];
    }

    return (float)sqrt(sum/(2*ObservationCount_));
}