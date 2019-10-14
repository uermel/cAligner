//
// Created by uermel on 9/16/19.
//

#ifndef CUDALIGNATOR_ALIGNMENTPROBLEM_H
#define CUDALIGNATOR_ALIGNMENTPROBLEM_H
#include "ceres/ceres.h"

class AlignmentProblem {
public:
    // Initialize class with maximum observations possible
    AlignmentProblem(int MarkerCount, int ProjectionCount, int maxObservationCount, double imDimX, double imDimY);
    ~AlignmentProblem();

    // Setters
    void set_point(int marker, double x, double y, double z);
    void set_shift(int projection, double x, double y);
    void set_theta(int projection, double theta);
    void set_psi(int projection, double theta);
    void set_mag(int projection, double mag);
    void set_phi(double phi);

    // Add observations
    void addObservation(double x, double y, int marker, int projection);

    int ObservationCount() const;
    const double* observations() const;

    // pointers to the start of one parameter type
    double* mutable_points();
    double* mutable_shifts();
    double* mutable_thetas();
    double* mutable_psis();
    double* mutable_mags();
    double* mutable_phi();

    // pointers to the parameters for on residual pair
    double* mutable_point_for_observation(int i);
    double* mutable_shift_for_observation(int i);
    double* mutable_theta_for_observation(int i);
    double* mutable_psi_for_observation(int i, bool fixed);
    double* mutable_mag_for_observation(int i);
    double* mutable_phi_for_observation(int i);

    // Getters
    void get_point(int marker, double* x, double* y, double* z);
    void get_shift(int projection, double* x, double* y);
    void get_theta(int projection, double* theta);
    void get_psi(int projection, double* theta);
    void get_mag(int projection, double* mag);
    void get_phi(double* phi);

    // Getters (observation based)
    void get_point_obs(int obs, double* x, double* y, double* z);
    void get_shift_obs(int obs, double* x, double* y);
    void get_theta_obs(int obs, double* theta);
    void get_psi_obs(int obs, double* theta);
    void get_mag_obs(int obs, double* mag);

    // Marker/Projection index from observation
    int markerIndex(int obs);
    int projectionIndex(int obs);

    // Errors
    float computeRMS();
    void computeRMSPerProjection(float* ret);
    void computeRMSPerMarker(float* ret);

private:
    int ProjectionCount_;
    int MarkerCount_;
    int ObservationCount_;
    int ParameterCount_;

    int* MarkerIndex_;
    int* ProjectionIndex_;
    double* observations_;
    double* parameters_;

    double imDimXhalf_;
    double imDimYhalf_;
};

template <typename T>
void ProjectPoint(T* result, const T* point, const T* theta, const T* psi, const T* phi, const T* mag, const T* shift){

    // Helper values
    T cpsi = cos(psi[0]);
    T spsi = sin(psi[0]);

    T ctheta = cos(theta[0]);
    T stheta = sin(theta[0]);

    T cphi = cos(phi[0]);
    T sphi = sin(phi[0]);

    // Matrix
    T m11 = (cphi * cphi * cpsi + sphi * (sphi * cpsi * ctheta + spsi * stheta)) / mag[0];
    T m12 = (sphi * cpsi * stheta - spsi * ctheta) / mag[0];
    T m13 = (-(sphi * cpsi * (ctheta - 1.) + spsi * stheta) * cphi) / mag[0];

    T m21 = (cphi * cphi * spsi + sphi * (sphi * spsi * ctheta - cpsi * stheta)) / mag[0];
    T m22 = (sphi * spsi * stheta + cpsi * ctheta) / mag[0];
    T m23 = (-(sphi * spsi * (ctheta - 1.) - cpsi * stheta) * cphi) / mag[0];

    // Project
    result[0] = ((m11 * point[0]) +
                 (m12 * point[1]) +
                 (m13 * point[2]) +
                 (shift[0]));

    result[1] = ((m21 * point[0]) +
                 (m22 * point[1]) +
                 (m23 * point[2]) +
                 (shift[1]));
}

struct ReprojectionError {
    ReprojectionError(double observed_x, double observed_y)
            : observed_x(observed_x), observed_y(observed_y) {}

    template <typename T>
    bool operator()(const T* const point,
                    const T* const shift,
                    const T* const theta,
                    const T* const psi,
                    const T* const mag,
                    const T* const phi,
                    T* residuals) const {

        T p[2]; // the projected position
        ProjectPoint(p, point, theta, psi, phi, mag, shift);

        residuals[0] = p[0] - observed_x;
        residuals[1] = p[1] - observed_y;

        return true;
    }

    static ceres::CostFunction* Create(const double observed_x,
                                       const double observed_y) {
//        return (new ceres::NumericDiffCostFunction<ReprojectionError, ceres::CENTRAL, 2, 3, 2, 1, 1, 1, 1>(
//                new ReprojectionError(observed_x, observed_y)));

        return (new ceres::AutoDiffCostFunction<ReprojectionError, 2, 3, 2, 1, 1, 1, 1>(
                new ReprojectionError(observed_x, observed_y)));
    }

    double observed_x;
    double observed_y;

    double weight;
};




#endif //CUDALIGNATOR_ALIGNMENTPROBLEM_H
