#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <eigen3/Eigen/Eigen>
#include "../lagrange-sim.hpp"
#include "../phys-constants.h"
#include "../input-args-parser.hpp"

#define TOTAL_TIME ((long long)2 * 365 * 24 * 3600)
#define TIME_STEP 15
#define SAVE_COUNT 10000
#define OUTPUT_FILE "lagrange-stability.csv"

struct TestBodyGenParams {
    /**
     * @brief Rings inner radius, relative to Moon's semi-major axis (0 means start from center, 1 means start from Moon's orbit distance)
     * 
     */
    double rFrom;
    /**
     * @brief Rings outer radius, relative to Moon's semi-major axis. See `rFrom` for more details.
     * 
     */
    double rTo;
    /**
     * @brief Number of bodies in one ring
     * 
     */
    size_t countInRing;
    /**
     * @brief How many rings of bodies should there be
     * 
     */
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

    // Read arguments
    const std::string outputFile = argsParser.getValue("-o");
    const double rFromRel = std::stod(argsParser.getValue("--from", "0.9"));
    const double rToRel = std::stod(argsParser.getValue("--to", "1.1"));
    const size_t countInRing = std::stoul(argsParser.getValue("--in-ring", "10"));
    const size_t ringCount = std::stoul(argsParser.getValue("--rings", "5"));
    const double totalTime = std::stod(argsParser.getValue("--time", "1e6"));
    const double timeStep = std::stod(argsParser.getValue("--tstep", "5"));
    const size_t saveCount = std::stoul(argsParser.getValue("--save-times", "1000"));
    const bool noLog = argsParser.optionExists("--no-log");
    
    // Generate simulated bodies
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
    
    // Start the simulation
    std::cerr << "Starting simulation..." << std::endl;
    auto simStartTime = std::chrono::high_resolution_clock::now();
    simulateSatellites(bodies, simParams);
    auto simFinishTime = std::chrono::high_resolution_clock::now();
    auto simMsTime = std::chrono::duration_cast<std::chrono::milliseconds>(simFinishTime - simStartTime);
    std::cerr << "Simulation finished!" << std::endl;
    std::cerr << "It took " << simMsTime.count() << "ms" << std::endl;

    if (!noLog) {
        std::ofstream logStream = std::ofstream(outputFile + ".log");
        logStream << "outputFile: " << outputFile <<
            "\nfrom: " << rFromRel <<
            "\nto: " << rToRel <<
            "\nin-ring: " << countInRing <<
            "\nrings: " << ringCount <<
            "\ntime: " << totalTime <<
            "\ntstep: " << timeStep <<
            "\nsave-times: " << saveCount << 
            "\n\ncalculation time: " << simMsTime.count() << "ms" << std::endl;
        logStream.close();
    }

    return 0;
}