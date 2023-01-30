#include <iostream>
#include <chrono>
#include <fstream>
#include <eigen3/Eigen/Eigen>
#include "../lagrange-sim.hpp"
#include "../phys-constants.h"
#include "../input-args-parser.hpp"

#define CMD_GEN_TYPE_RINGS "rings"
#define CMD_GEN_TYPE_SPAN "span"

enum TestBodyGenType {
    /**
     * @brief Rings around the Moon
     * 
     */
    RINGS,
    /**
     * @brief Partial rings centered in the center of the system
     * 
     */
    SPAN
};

struct TestBodyGenParams {
    /**
     * @brief Rings inner radius, relative to Moon's semi-major axis (0 means start from center, 1 means start from Moon's orbit distance)
     * 
     */
    double rFromRel;
    /**
     * @brief Rings outer radius, relative to Moon's semi-major axis. See `rFrom` for more details.
     * 
     */
    double rToRel;
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
    /**
     * @brief What pattern should the bodies have
     * 
     */
    TestBodyGenType genType;
    /**
     * @brief If `genType` is `SPAN`, this defines how wide the span is
     * 
     */
    double spanAngle;
    /**
     * @brief If `genType` is `SPAN`, this defines how much the span is offset from the Moon (in radians).
     * 
     * Useful for simulating bodies around L3 point (than this would be `M_PI`).
     */
    double spanOffset;
};

std::vector<TestBody> generateTestBodies(const TestBodyGenParams& p) {
    const double l12Distance = (MOON_A * pow(MOON_M / 3 / EARTH_M, 1.0/3.0));
    Eigen::Vector2d l1Pos, l2Pos;
    l1Pos << MOON_A - l12Distance, 0;
    l2Pos << MOON_A + l12Distance, 0;

    std::vector<TestBody> bodies;
    bodies.reserve(p.countInRing * p.ringCount * 2); 

    for (size_t ring = 0; ring < p.ringCount; ++ring) {
        for (size_t angle_i = 0; angle_i < p.countInRing; ++angle_i) {
            if (p.genType == TestBodyGenType::RINGS) {
                const double rFrom = p.rFromRel * l12Distance;
                const double rTo = p.rToRel * l12Distance;
                const double angle = 2 * M_PI * angle_i / p.countInRing;
                const double r = rFrom + (rTo - rFrom) * ring / p.ringCount;

                Eigen::Vector2d offsetPos;
                offsetPos << r * cos(angle), r * sin(angle);
                TestBody bodyL1, bodyL2;
                bodyL1.pos = l1Pos + offsetPos;
                bodyL2.pos = l2Pos + offsetPos;
                bodyL1.vel << 0, 0;
                bodyL2.vel << 0, 0;

                bodies.push_back(bodyL1);
                bodies.push_back(bodyL2);
                if (r == 0.0)
                    break; // Don't generate multiple bodies in the same spot
            }
            else if (p.genType == TestBodyGenType::SPAN) {
                const double angle = p.spanAngle * angle_i / p.countInRing - p.spanAngle / 2 + p.spanOffset;
                const double rFrom = p.rFromRel * MOON_A;
                const double rTo = p.rToRel * MOON_A;
                const double r = rFrom + (rTo - rFrom) * ring / p.ringCount;

                TestBody body;
                body.pos << r * cos(angle), r * sin(angle);
                body.vel << 0, 0;
                bodies.push_back(body);
            }
        }
    }

    return bodies;
}

int main(const int argc, const char *argv[]) {
    InputArgsParser argsParser(argc, argv);

    // Read arguments
    const std::string outputFile = argsParser.getValue("-o");
    const double rFromRel = std::stod(argsParser.getValue("--from", "0.0"));
    const double rToRel = std::stod(argsParser.getValue("--to", "0.5"));
    const size_t countInRing = std::stoul(argsParser.getValue("--in-ring", "10"));
    const size_t ringCount = std::stoul(argsParser.getValue("--rings", "5"));
    const double totalTime = std::stod(argsParser.getValue("--time", "1e6"));
    const double timeStep = std::stod(argsParser.getValue("--tstep", "5"));
    const size_t saveCount = std::stoul(argsParser.getValue("--save-times", "1000"));
    const std::string genTypeStr = argsParser.getValue("--gen-type", CMD_GEN_TYPE_RINGS);
    const double spanAngle = std::stod(argsParser.getValue("--span", "20")) * M_PI / 180;
    const double spanOffset = std::stod(argsParser.getValue("--span-offset")) * M_PI / 180;
    const bool noLog = argsParser.optionExists("--no-log");

    TestBodyGenType genType;
    if (genTypeStr == CMD_GEN_TYPE_RINGS)
        genType = TestBodyGenType::RINGS;
    else if (genTypeStr == CMD_GEN_TYPE_SPAN)
        genType = TestBodyGenType::SPAN;
    else {
        std::cerr << "Unknown gen-type: \"" << genTypeStr << '"' << std::endl;
        return -1;
    }

    std::cerr << "Output file: " << outputFile << std::endl;
    std::cerr << "rFrom: " << rFromRel
        << "\r\nrTo: " << rToRel
        << "\r\nrings: " << ringCount
        << "\r\ncount in rings: " << countInRing
        << "\r\ntotal sim time: " << totalTime
        << "\r\ntime step: " << timeStep
        << "\r\nsave count: " << saveCount
        << std::endl;


    // Generate bodies
    std::cerr << "Generating test bodies..." << std::endl;

    const TestBodyGenParams genParams {
        .rFromRel = rFromRel,
        .rToRel = rToRel,
        .countInRing = countInRing,
        .ringCount  = ringCount,
        .genType = genType,
        .spanAngle = spanAngle,
        .spanOffset = spanOffset,
    };
    std::vector<TestBody> bodies = generateTestBodies(genParams);
    
    // Start the simulation
    std::cerr << "Starting simulation..." << std::endl;
    SimParams simParams {
        .outputFile = outputFile.c_str(),
        .time_from = 0,
        .time_to = totalTime,
        .time_step = timeStep,
        .saveCount = saveCount
    };
    auto simStartTime = std::chrono::high_resolution_clock::now();
    simulateSatellites(bodies, simParams);
    auto simFinishTime = std::chrono::high_resolution_clock::now();
    auto simMsTime = std::chrono::duration_cast<std::chrono::milliseconds>(simFinishTime - simStartTime);
    std::cerr << "Simulation finished!" << std::endl;

    // Save the log file
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
            "\ngen type: " << genTypeStr <<
            "\nspan: " << spanAngle * 180 / M_PI << 
            "\n\ncalculation time: " << simMsTime.count() << "ms" << std::endl;
        logStream.close();
    }

    return 0;
}