//
// Created by uermel on 9/14/19.
//

#ifndef CUDALIGNATOR_REPROJECTION_H
#define CUDALIGNATOR_REPROJECTION_H


struct ReprojectionError {
    ReprojectionError(double observed_x, double observed_y)
    : observed_x(observed_x), observed_y(observed_y) {}

    template <typename T>
    bool operator()(const T* const point,
                    const T* const shift,
                    const T* const theta,
                    const T* const psi,
                    const T* const phi,
                    const T* const mag,
                    T* residuals) const {

        T p[2]; // the projected position
        ProjectPoint(point, theta, psi, phi, mag);

        // shift[0] is x-shift, shift[1] is y-shift
        p[0] += shift[0];
        p[1] += shift[1];

        residuals[0] = p[0] - observed_x;
        residuals[1] = p[1] - observed_y;

        return true;
    }

    static ceres::CostFunction* Create(const double observed_x,
                                       const double observed_y) {
        return (new ceres::AutoDiffCostFunction<ReprojectionError, 2, 3, 2, 1, 1, 1, 1>(
                new ReprojectionError(observed_x, observed_y)));
    }

    double observed_x;
    double observed_y;
};

#endif //CUDALIGNATOR_REPROJECTION_H
