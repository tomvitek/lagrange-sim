#include "lagrange-sim.hpp"
#include <iostream>
#include <limits>
#include <eigen3/Eigen/Eigen>
#include "rk4-sim.hpp"
#include "phys-constants.h"

using namespace Eigen;

#define mu2 (EARTH_M / (EARTH_M + MOON_M)) //0.98784948
#define mu1 (MOON_M / (EARTH_M + MOON_M)) //0.0121505156
#define D MOON_A
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
    derivatives(all, seq(INDEX_VX, last, BODY_PARAM_COUNT)) = -G * (EARTH_M * (x + mu1 * D) / r1pow3 + MOON_M * (x - mu2 * D) / r2pow3) + Omega * Omega * x + 2 * Omega * vy;
    derivatives(all, seq(INDEX_VY, last, BODY_PARAM_COUNT)) = -G * (EARTH_M * y / r1pow3 + MOON_M * y / r2pow3) + Omega * Omega * y - 2 * Omega * vx;
    derivatives(all, seq(INDEX_X, last, BODY_PARAM_COUNT)) = vx;
    derivatives(all, seq(INDEX_Y, last, BODY_PARAM_COUNT)) = vy;

    // Collisions
    for (size_t i = 0; i < r1pow3.rows(); ++i) {
        if (r1pow3[i] < EARTH_R * EARTH_R * EARTH_R || r2pow3[i] < MOON_R * MOON_R * MOON_R) {
            const double nan = std::numeric_limits<double>::quiet_NaN();
            derivatives.row(i).fill(nan);
        }
    }

    return derivatives;
}

void simulateSatellite(const Eigen::Ref<Eigen::Vector2d>& pos, const Eigen::Ref<Eigen::Vector2d>& vel, SimParams& params) {
    ArrayXXd x0(1, BODY_PARAM_COUNT);
    x0 << pos.transpose(), vel.transpose();

    SimWriter<ArrayXXd> simWriter(params.outputFile);
    RK4Solver<ArrayXXd> solver(lagrangeFunc, simWriter);
    std::cout << "satellite simulation started" << std::endl;
    ArrayXXd result = solver.solve(params.time_from, params.time_step, params.time_to, x0, params.saveCount);
}

void simulateSatellites(std::vector<TestBody>& bodies, SimParams& params) {
    ArrayXXd x0(bodies.size(), BODY_PARAM_COUNT);

    for (size_t i = 0; i < bodies.size(); ++i) {
        const TestBody& body = bodies[i];
        x0.row(i) << body.pos.transpose(), body.vel.transpose();
    }

    SimWriter<ArrayXXd> simWriter(params.outputFile);
    RK4Solver<ArrayXXd> solver(lagrangeFunc, simWriter);
    solver.solve(params.time_from, params.time_step, params.time_to, x0, params.saveCount);
}