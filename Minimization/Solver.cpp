//
// Created by uermel on 9/16/19.
//

#include "Solver.h"
#include "ceres/ceres.h"

Solver::Solver(AlignmentProblem* ali) {
    ali_ = ali;
}

void Solver::minimizeShifts() {

    const double* observations = ali_->observations();
    //std::cout << "Observations: " << ali_->ObservationCount() << std::flush;
    ceres::Problem problem;

    for (int obs = 0; obs < ali_->ObservationCount(); obs++){

        ceres::CostFunction* costFunction =
                ReprojectionError::Create(observations[0 + obs * 2], observations[1 + obs * 2]);

        ceres::LossFunction* lossFunction = new ceres::HuberLoss(1.0);

        problem.AddResidualBlock(costFunction,
                                 lossFunction,
                                 ali_->mutable_point_for_observation(obs),
                                 ali_->mutable_shift_for_observation(obs),
                                 ali_->mutable_theta_for_observation(obs),
                                 ali_->mutable_psi_for_observation(obs, false),
                                 ali_->mutable_mag_for_observation(obs),
                                 ali_->mutable_phi_for_observation(obs));

        problem.SetParameterBlockConstant(ali_->mutable_theta_for_observation(obs));
        problem.SetParameterBlockConstant(ali_->mutable_psi_for_observation(obs, false));
        problem.SetParameterBlockConstant(ali_->mutable_mag_for_observation(obs));
        problem.SetParameterBlockConstant(ali_->mutable_phi_for_observation(obs));
    }

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::SPARSE_SCHUR;
    options.use_nonmonotonic_steps = true;
    options.num_threads = 8;
    //options.minimizer_progress_to_stdout = true;

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    //std::cout << summary.FullReport() << "\n";
}


void Solver::minimizeThetas() {

    const double* observations = ali_->observations();

    ceres::Problem problem;

    for (int obs = 0; obs < ali_->ObservationCount(); obs++){

        ceres::CostFunction* costFunction =
                ReprojectionError::Create(observations[0 + obs * 2], observations[1 + obs * 2]);

        ceres::LossFunction* lossFunction = new ceres::HuberLoss(1.0);

        problem.AddResidualBlock(costFunction,
                                 lossFunction,
                                 ali_->mutable_point_for_observation(obs),
                                 ali_->mutable_shift_for_observation(obs),
                                 ali_->mutable_theta_for_observation(obs),
                                 ali_->mutable_psi_for_observation(obs, false),
                                 ali_->mutable_mag_for_observation(obs),
                                 ali_->mutable_phi_for_observation(obs));

        problem.SetParameterBlockConstant(ali_->mutable_shift_for_observation(obs));
        problem.SetParameterBlockConstant(ali_->mutable_psi_for_observation(obs, false));
        problem.SetParameterBlockConstant(ali_->mutable_mag_for_observation(obs));
        problem.SetParameterBlockConstant(ali_->mutable_phi_for_observation(obs));
    }

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::SPARSE_SCHUR;
    options.use_nonmonotonic_steps = true;
    options.num_threads = 8;
    //options.minimizer_progress_to_stdout = true;

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    //std::cout << summary.FullReport() << "\n";
}


void Solver::minimizePsis(bool fixed) {

    const double* observations = ali_->observations();

    ceres::Problem problem;

    for (int obs = 0; obs < ali_->ObservationCount(); obs++){

        ceres::CostFunction* costFunction =
                ReprojectionError::Create(observations[0 + obs * 2], observations[1 + obs * 2]);

        ceres::LossFunction* lossFunction = new ceres::HuberLoss(1.0);

        problem.AddResidualBlock(costFunction,
                                 lossFunction,
                                 ali_->mutable_point_for_observation(obs),
                                 ali_->mutable_shift_for_observation(obs),
                                 ali_->mutable_theta_for_observation(obs),
                                 ali_->mutable_psi_for_observation(obs, fixed),
                                 ali_->mutable_mag_for_observation(obs),
                                 ali_->mutable_phi_for_observation(obs));

        problem.SetParameterBlockConstant(ali_->mutable_theta_for_observation(obs));
        problem.SetParameterBlockConstant(ali_->mutable_shift_for_observation(obs));
        problem.SetParameterBlockConstant(ali_->mutable_mag_for_observation(obs));
        problem.SetParameterBlockConstant(ali_->mutable_phi_for_observation(obs));
    }

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::SPARSE_SCHUR;
    options.use_nonmonotonic_steps = true;
    options.num_threads = 8;
    //options.minimizer_progress_to_stdout = true;

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    // Write the single psi value to all observations
    if (fixed) {
        for (int obs = 0; obs < ali_->ObservationCount(); obs++){
            ali_->mutable_psi_for_observation(obs, false)[0] = ali_->mutable_psi_for_observation(obs, fixed)[0];
        }
    }
    //std::cout << summary.FullReport() << "\n";
}


void Solver::minimizeMags() {

    const double* observations = ali_->observations();

    ceres::Problem problem;

    for (int obs = 0; obs < ali_->ObservationCount(); obs++){

        ceres::CostFunction* costFunction =
                ReprojectionError::Create(observations[0 + obs * 2], observations[1 + obs * 2]);

        ceres::LossFunction* lossFunction = new ceres::HuberLoss(1.0);

        problem.AddResidualBlock(costFunction,
                                 lossFunction,
                                 ali_->mutable_point_for_observation(obs),
                                 ali_->mutable_shift_for_observation(obs),
                                 ali_->mutable_theta_for_observation(obs),
                                 ali_->mutable_psi_for_observation(obs, false),
                                 ali_->mutable_mag_for_observation(obs),
                                 ali_->mutable_phi_for_observation(obs));

        problem.SetParameterBlockConstant(ali_->mutable_shift_for_observation(obs));
        problem.SetParameterBlockConstant(ali_->mutable_theta_for_observation(obs));
        problem.SetParameterBlockConstant(ali_->mutable_psi_for_observation(obs, false));
        problem.SetParameterBlockConstant(ali_->mutable_phi_for_observation(obs));
    }

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::SPARSE_SCHUR;
    options.use_nonmonotonic_steps = true;
    options.num_threads = 8;
    //options.minimizer_progress_to_stdout = true;

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    //std::cout << summary.FullReport() << "\n";
}


void Solver::minimizePhi() {

    const double* observations = ali_->observations();

    ceres::Problem problem;

    for (int obs = 0; obs < ali_->ObservationCount(); obs++){

        ceres::CostFunction* costFunction =
                ReprojectionError::Create(observations[0 + obs * 2], observations[1 + obs * 2]);

        ceres::LossFunction* lossFunction = new ceres::HuberLoss(1.0);

        problem.AddResidualBlock(costFunction,
                                 lossFunction,
                                 ali_->mutable_point_for_observation(obs),
                                 ali_->mutable_shift_for_observation(obs),
                                 ali_->mutable_theta_for_observation(obs),
                                 ali_->mutable_psi_for_observation(obs, false),
                                 ali_->mutable_mag_for_observation(obs),
                                 ali_->mutable_phi_for_observation(obs));

        problem.SetParameterBlockConstant(ali_->mutable_shift_for_observation(obs));
        problem.SetParameterBlockConstant(ali_->mutable_theta_for_observation(obs));
        problem.SetParameterBlockConstant(ali_->mutable_psi_for_observation(obs, false));
        problem.SetParameterBlockConstant(ali_->mutable_mag_for_observation(obs));
    }

    ceres::Solver::Options options;
    options.linear_solver_type = ceres::SPARSE_SCHUR;
    options.use_nonmonotonic_steps = true;
    options.num_threads = 8;
    //options.minimizer_progress_to_stdout = true;

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    //std::cout << summary.FullReport() << "\n";
}