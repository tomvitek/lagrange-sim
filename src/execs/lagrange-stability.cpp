#include <vector>
#include <iostream>
#include <eigen3/Eigen/Eigen>
#include "../lagrange-sim.hpp"
#include "../phys-constants.h"
#include "../input-args-parser.hpp"

#define TOTAL_TIME ((long long)2 * 365 * 24 * 3600)
#define TIME_STEP 15
#define SAVE_COUNT 10000
#define OUTPUT_FILE "lagrange-stability.csv"

struct TestBodyGenParams {
    double rFrom;
    double rTo;
    size_t countInRing;
    size_t ringCount;
};

std::vector<TestBody> generateTestBodies(TestBodyGenParams p) {
    std::vector<TestBody> bodies;
    bodies.reserve(p.countInRing * p.ringCount);

    for (size_t ring = 0; ring < p.ringCount; ++ring) {
        double r = p.rFrom + (p.rTo - p.rFrom) * ring / p.ringCount;
        double v = sqrt(G * EARTH_M / r) - 2 * M_PI * r / MOON_T_SIDERIC;

        for (size_t angle_i = 0; angle_i < p.countInRing; ++angle_i) {
            double angle = 2 * M_PI * angle_i / p.countInRing;
            TestBody body;
            body.pos << r * cos(angle), r * sin(angle);
            body.vel << -v * sin(angle), v * cos(angle);
            bodies.push_back(body);
        }
    }

    return bodies;
}

int main(int argc, const char **argv) {
    InputArgsParser argsParser(argc, argv);

    const std::string outputFile = argsParser.getValue("-o");
    const double rFromRel = std::stod(argsParser.getValue("--from", "0.9"));
    const double rToRel = std::stod(argsParser.getValue("--to", "1.1"));
    const size_t countInRing = std::stoul(argsParser.getValue("--in-ring", "10"));
    const size_t ringCount = std::stoul(argsParser.getValue("--rings", "5"));
    const double totalTime = std::stod(argsParser.getValue("--time", "1e6"));
    const double timeStep = std::stod(argsParser.getValue("--tstep", "5"));
    const size_t saveCount = std::stoul(argsParser.getValue("--save-times", "1000"));

    std::cerr << "Generating test bodies..." << std::endl;
    TestBodyGenParams genParams {
        .rFrom = MOON_A * rFromRel,
        .rTo = MOON_A * rToRel,
        .countInRing = countInRing,
        .ringCount = ringCount
    };

    std::vector<TestBody> bodies = generateTestBodies(genParams);
    std::cerr << "Total bodies count: " << bodies.size() << std::endl;

    SimParams simParams {
        .outputFile = outputFile.c_str(),
        .time_from = 0,
        .time_to = totalTime,
        .time_step = timeStep,
        .saveCount = saveCount
    };
    std::cerr << "Starting simulation..." << std::endl;
    simulateSatellites(bodies, simParams);
    std::cerr << "Simulation finished!" << std::endl;

    return 0;
}