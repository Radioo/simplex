#include <iostream>
#include <vector>

class SimplexSolver {
private:
    std::vector<std::vector<double>> tableau;
    std::vector<int> basicVariables;
    int numConstraints;
    int numVariables;
    bool isMaximization; // Indicates whether it's a maximization problem

    void initializeTableau(const std::vector<std::vector<double>>& A, const std::vector<double>& b, const std::vector<double>& c, const std::vector<std::string>& signs) {
        numConstraints = A.size();
        numVariables = c.size();
        tableau.resize(numConstraints + 1, std::vector<double>(numVariables + numConstraints + 1));

        // Initialize coefficients of constraints
        for (int i = 0; i < numConstraints; ++i) {
            for (int j = 0; j < numVariables; ++j) {
                tableau[i][j] = A[i][j];
            }
            if (signs[i] == "<=") {
                tableau[i][numVariables + i] = 1.0; // Slack variables
            } else if (signs[i] == ">=") {
                tableau[i][numVariables + i] = -1.0; // Excess variables
            }
            tableau[i][numVariables + numConstraints] = b[i]; // RHS
        }

        // Initialize coefficients of objective function
        for (int j = 0; j < numVariables; ++j) {
            tableau[numConstraints][j] = (isMaximization ? c[j] : -c[j]);
        }
    }

    int findEnteringVariable() const {
        int enteringVar = -1;
        for (int j = 0; j < numVariables + numConstraints; ++j) {
            if (isMaximization) {
                if (tableau[numConstraints][j] > 0) {
                    enteringVar = j;
                    break;
                }
            } else {
                if (tableau[numConstraints][j] < 0) {
                    enteringVar = j;
                    break;
                }
            }
        }
        return enteringVar;
    }

    int findLeavingVariable(int enteringVar) const {
        int leavingVar = -1;
        double minRatio = std::numeric_limits<double>::infinity();

        for (int i = 0; i < numConstraints; ++i) {
            if (tableau[i][enteringVar] > 0) {
                double ratio = tableau[i][numVariables + numConstraints] / tableau[i][enteringVar];
                if (ratio < minRatio) {
                    minRatio = ratio;
                    leavingVar = i;
                }
            }
        }
        return leavingVar;
    }

    void pivot(int enteringVar, int leavingVar) {
        basicVariables[leavingVar] = enteringVar;

        // Adjust the leaving row
        double pivotValue = tableau[leavingVar][enteringVar];
        for (int j = 0; j < numVariables + numConstraints + 1; ++j) {
            tableau[leavingVar][j] /= pivotValue;
        }

        // Adjust other rows
        for (int i = 0; i < numConstraints + 1; ++i) {
            if (i != leavingVar) {
                double multiplier = tableau[i][enteringVar];
                for (int j = 0; j < numVariables + numConstraints + 1; ++j) {
                    tableau[i][j] -= multiplier * tableau[leavingVar][j];
                }
            }
        }
    }

public:
    SimplexSolver(const std::vector<std::vector<double>>& A, const std::vector<double>& b, const std::vector<double>& c, const std::vector<std::string>& signs, bool isMaximization) {
        this->isMaximization = isMaximization;
        initializeTableau(A, b, c, signs);
        basicVariables.resize(numConstraints);
        for (int i = 0; i < numConstraints; ++i) {
            if (signs[i] == "<=") {
                basicVariables[i] = numVariables + i; // Slack variables
            } else if (signs[i] == ">=") {
                basicVariables[i] = numVariables + numConstraints + i; // Excess variables
            }
        }
    }

    void solve() {
        while (true) {
            int enteringVar = findEnteringVariable();
            if (enteringVar == -1) break; // Optimality reached

            int leavingVar = findLeavingVariable(enteringVar);
            if (leavingVar == -1) {
                std::cerr << "Unbounded problem" << std::endl;
                return;
            }

            pivot(enteringVar, leavingVar);
        }
    }

    double getObjectiveValue() const {
        return (isMaximization ? -tableau[numConstraints][numVariables + numConstraints] : tableau[numConstraints][numVariables + numConstraints]);
    }

    std::vector<double> getSolution() const {
        std::vector<double> solution(numVariables, 0);
        for (int i = 0; i < numConstraints; ++i) {
            if (basicVariables[i] < numVariables) {
                solution[basicVariables[i]] = tableau[i][numVariables + numConstraints];
            }
        }
        return solution;
    }
};