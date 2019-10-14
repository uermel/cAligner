//
// Created by uermel on 9/14/19.
//

#include "ceres/ceres.h"
#include "/usr/include/glog/logging.h"

class AlignmentProblem {
public:
    ~AlignmentProblem() {
        delete[] MarkerIndex_;
        delete[] ProjectionIndex_;
        delete[] observations_;
        delete[] parameters_;
    }

    int ObservationCount()          const { return ObservationCount_; }
    const double* observations()    const { return observations_; }
    double* mutable_points()              { return parameters_; }
    double* mutable_shifts()              { return parameters_ + 3 * MarkerCount_; }
    double* mutable_thetas()              { return parameters_ + 3 * MarkerCount_ + 2 * ProjectionCount_; }
    double* mutable_psis()                { return parameters_ + 3 * MarkerCount_ + 3 * ProjectionCount_; }
    double* mutable_mags()                { return parameters_ + 3 * MarkerCount_ + 4 * ProjectionCount_; }
    double* mutable_phi()                 { return parameters_ + 3 * MarkerCount_ + 5 * ProjectionCount_; }

    double* mutable_point_for_observation(int i) {
        return mutable_points() + MarkerIndex_[i] * 3;
    }

    double* mutable_shift_for_observation(int i) {
        return mutable_shifts() + ProjectionIndex_[i] * 2;
    }

    double* mutable_theta_for_observation(int i) {
        return mutable_thetas() + ProjectionIndex_[i];
    }

    double* mutable_psi_for_observation(int i) {
        return mutable_psis() + ProjectionIndex_[i];
    }

    double* mutable_mag_for_observation(int i) {
        return mutable_mags() + ProjectionIndex_[i];
    }

    double* mutable_phi_for_observation(int i) {
        return mutable_phi();
    }

    void initialize(double* x, double* y, double* z, double* shifts, double* thetas, double* psis, double* mags, double* phi, double* observations, int* MarkerIndex, int* ProjectionIndex, int MarkerCount, int ProjectionCount, int ObservationCount){

        MarkerCount_ = MarkerCount;
        MarkerIndex_ = new int[ObservationCount_];

        ProjectionCount_ = ProjectionCount;
        ProjectionIndex_ = new int[ObservationCount_];

        ObservationCount_ = ObservationCount;
        observations_ = new double[2 * ObservationCount_];

        ParameterCount_ = 3 * MarkerCount_ + 5 * ProjectionCount_ + 1;
        parameters_ = new double[ParameterCount_];

        mutable_phi()[0] = phi[0];

        for (int proj = 0; proj < ProjectionCount_; proj++){
            mutable_thetas()[proj] = thetas[proj];
            mutable_psis()[proj] = psis[proj];
            mutable_mags()[proj] = mags[proj];

            mutable_shifts()[0 + proj * 2] = shifts[proj + 0 + proj * 2];
            mutable_shifts()[1 + proj * 2] = shifts[proj + 0 + proj * 2];
        }

        for (int mark = 0; mark < MarkerCount_; mark++){
            mutable_points()[0 + mark * 3] = x[mark];
            mutable_points()[1 + mark * 3] = y[mark];
            mutable_points()[2 + mark * 3] = z[mark];
        }

        for (int obs = 0; obs < ObservationCount_; obs++){
            observations_[0 + obs * 2] = observations[0 + obs * 2];
            observations_[1 + obs * 2] = observations[1 + obs * 2];

            MarkerIndex_[obs] = MarkerIndex[obs];
            ProjectionIndex_[obs] = ProjectionIndex[obs];
        }
    }


private:
    int ProjectionCount_;
    int MarkerCount_;
    int ObservationCount_;
    int ParameterCount_;

    int* MarkerIndex_;
    int* ProjectionIndex_;
    double* observations_;
    double* parameters_;
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
        return (new ceres::NumericDiffCostFunction<ReprojectionError, ceres::CENTRAL, 2, 3, 2, 1, 1, 1, 1>(
                new ReprojectionError(observed_x, observed_y)));
    }

    double observed_x;
    double observed_y;
};


int main(int argc, char** argv){

    google::InitGoogleLogging(argv[0]);
    //if (argc != 2) {
    //    std::cerr << "usage: simple_bundle_adjuster <bal_problem>\n";
    //    return 1;
    //}

    int MarkerCount = 20;
    int ProjectionCount = 41;
    int ObservationCount = 820;

    double x[MarkerCount];
    double y[MarkerCount];
    double z[MarkerCount];
    double thetas[ProjectionCount];
    double psis[ProjectionCount];
    double mags[ProjectionCount];
    double observation[2*ProjectionCount*MarkerCount];
    int projIdx[ProjectionCount*MarkerCount];
    int markIdx[ProjectionCount*MarkerCount];
    double shifts[2*ProjectionCount];
    double phi[1] = {-0.0088};

    FILE *fp = fopen("data_in.bin", "rb");
    fread(x, sizeof(double), MarkerCount, fp);
    fread(y, sizeof(double), MarkerCount, fp);
    fread(z, sizeof(double), MarkerCount, fp);
    fread(thetas, sizeof(double), ProjectionCount, fp);
    fread(psis, sizeof(double), ProjectionCount, fp);
    fread(mags, sizeof(double), ProjectionCount, fp);
    fread(observation, sizeof(double), 2*ProjectionCount*MarkerCount, fp);
    fread(projIdx, sizeof(int), ProjectionCount*MarkerCount, fp);
    fread(markIdx, sizeof(int), ProjectionCount*MarkerCount, fp);
    fread(shifts, sizeof(double), 2*ProjectionCount, fp);
    fclose(fp);

    AlignmentProblem ali;
    ali.initialize(x, y, z, shifts, thetas, psis, mags, phi, observation, markIdx, projIdx, MarkerCount, ProjectionCount, ObservationCount);

    const double* observations = ali.observations();

    ceres::Problem problem;

    for (int obs = 0; obs < ali.ObservationCount(); obs++){

        ceres::CostFunction* cost_function =
                ReprojectionError::Create(observations[0 + obs * 2], observations[1 + obs * 2]);

        ceres::LossFunction* loss_function = new ceres::HuberLoss(1.0);

        problem.AddResidualBlock(cost_function,
                loss_function,
                ali.mutable_point_for_observation(obs),
                ali.mutable_shift_for_observation(obs),
                ali.mutable_theta_for_observation(obs),
                ali.mutable_psi_for_observation(obs),
                ali.mutable_mag_for_observation(obs),
                ali.mutable_phi_for_observation(obs));

        problem.SetParameterBlockConstant(ali.mutable_theta_for_observation(obs));
        problem.SetParameterBlockConstant(ali.mutable_psi_for_observation(obs));
        problem.SetParameterBlockConstant(ali.mutable_mag_for_observation(obs));
        problem.SetParameterBlockConstant(ali.mutable_phi_for_observation(obs));
    }



    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_SCHUR;
    options.minimizer_progress_to_stdout = true;

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    std::cout << summary.FullReport() << "\n";

    fp = fopen("data_out.bin", "wb");
    fwrite(ali.mutable_points(), sizeof(double), 3*MarkerCount, fp);
    fwrite(ali.mutable_thetas(), sizeof(double), ProjectionCount, fp);
    fwrite(ali.mutable_psis(), sizeof(double), ProjectionCount, fp);
    fwrite(ali.mutable_mags(), sizeof(double), ProjectionCount, fp);
    fwrite(ali.observations(), sizeof(double), 2*ProjectionCount*MarkerCount, fp);
    fwrite(ali.mutable_shifts(), sizeof(double), 2*ProjectionCount, fp);
    fclose(fp);

    return 0;
}