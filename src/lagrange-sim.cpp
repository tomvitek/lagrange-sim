#include "lagrange-sim.hpp"
#include <iostream>
#include <eigen3/Eigen/Eigen>
#include "rg4-sim.hpp"

using namespace Eigen;

#define G 6.67259e-11
#define M 5.974e24
#define m 7.348e22
#define mu2 (M / (M + m)) //0.98784948
#define mu1 (m / (M + m)) //0.0121505156
#define D 3.844e8
#define d 4.669e6
#define Omega 2.661e-6

#define INDEX_X 0
#define INDEX_Y 1
#define INDEX_VX 2
#define INDEX_VY 3
#define BODY_PARAM_COUNT 4

ArrayXXd lagrangeFunc(double t, const ArrayXXd& data) {
    ArrayXd x = data(all, seq(INDEX_X, last, BODY_PARAM_COUNT));
    ArrayXd y = data(all, seq(INDEX_Y, last, BODY_PARAM_COUNT));
    ArrayXd vx = data(all, seq(INDEX_VX, last, BODY_PARAM_COUNT));
    ArrayXd vy = data(all, seq(INDEX_VY, last, BODY_PARAM_COUNT));
    ArrayXd r1pow3 = Eigen::pow(Eigen::pow(x + d, 2) + Eigen::pow(y, 2), 1.5);
    ArrayXd r2pow3 = Eigen::pow(Eigen::pow(x + d - D, 2) + Eigen::pow(y, 2), 1.5);

    ArrayXXd derivatives(data.rows(), data.cols());
    derivatives(all, seq(INDEX_VX, last, BODY_PARAM_COUNT)) = -G * (M * (x + mu1 * D) / r1pow3 + m * (x - mu2 * D) / r2pow3) + Omega * Omega * x + 2 * Omega * vy;
    derivatives(all, seq(INDEX_VY, last, BODY_PARAM_COUNT)) = -G * (M * y / r1pow3 + m * y / r2pow3) + Omega * Omega * y - 2 * Omega * vx;
    derivatives(all, seq(INDEX_X, last, BODY_PARAM_COUNT)) = vx;
    derivatives(all, seq(INDEX_Y, last, BODY_PARAM_COUNT)) = vy;

    return derivatives;
}

void simulateSatellite(const Eigen::Ref<Eigen::Vector2d>& pos, const Eigen::Ref<Eigen::Vector2d>& vel, SimParams& params) {
    ArrayXXd x0(1, BODY_PARAM_COUNT);
    x0 << pos.transpose(), vel.transpose();

    SimWriter<ArrayXXd> simWriter(params.outputFile);
    RG4Solver<ArrayXXd> solver(lagrangeFunc, simWriter);
    std::cout << "satellite simulation started" << std::endl;
    ArrayXXd result = solver.solve(params.time_from, params.time_step, params.time_to, x0, params.saveCount);
}

