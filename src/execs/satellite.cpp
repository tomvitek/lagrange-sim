/**
 * Simulates a simple satellite trajectory
 * 
 */
#include "../lagrange-sim.hpp"
#include <iostream>
#include <eigen3/Eigen/Eigen>

#define X0 4.613e8
#define Y0 0
#define VX0 0
#define VY0 -1074
#define TOTAL_TIME (10 * 24 * 3600)

int main(int argc, const char **args) {
    std::cout << "Preparing the simulation..." << std::endl;
    
    Eigen::Vector2d initPos;
    Eigen::Vector2d initVel;
    initPos << X0, Y0;
    initVel << VX0, VY0;
    SimParams simParams {
        .outputFile = "satellite.csv",
        .time_from = 0,
        .time_to = TOTAL_TIME,
        .time_step = 1,
        .saveCount = 1000,
    };

    simulateSatellite(initPos.array(), initVel.array(), simParams);

    std::cout << "Done" << std::endl;
    return 0;
}