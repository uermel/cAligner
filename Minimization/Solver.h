//
// Created by uermel on 9/16/19.
//

#ifndef CUDALIGNATOR_SOLVER_H
#define CUDALIGNATOR_SOLVER_H

#include "AlignmentProblem.h"

class Solver {
public:
    Solver(AlignmentProblem* ali);

    void minimizeShifts();
    void minimizeThetas();
    void minimizePsis(bool fixed);
    void minimizeMags();
    void minimizePhi();

private:
    AlignmentProblem* ali_;
};


#endif //CUDALIGNATOR_SOLVER_H
