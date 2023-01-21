#include <iostream>
#include <eigen3/Eigen/Eigen>
#include "../lagrange-sim.hpp"
#include "../phys-constants.h"
#include "../input-args-parser.hpp"

#define CMD_GEN_TYPE_RINGS "rings"
#define CMD_GEN_TYPE_SPAN "span"

enum TestBodyGenType {
    RINGS,
    SPAN
};

struct TestBodyGenParams {
    double rFromRel;
    double rToRel;
    size_t countInRing;
    size_t ringCount;
    TestBodyGenType genType;
    double spanAngle;
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
                const double angle = p.spanAngle * angle_i / p.countInRing - p.spanAngle / 2;
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


    std::cerr << "Generating test bodies..." << std::endl;

    const TestBodyGenParams genParams {
        .rFromRel = rFromRel,
        .rToRel = rToRel,
        .countInRing = countInRing,
        .ringCount  = ringCount,
        .genType = genType,
        .spanAngle = spanAngle,
    };
    std::vector<TestBody> bodies = generateTestBodies(genParams);
    
    std::cerr << "Starting simulation..." << std::endl;
    SimParams simParams {
        .outputFile = outputFile.c_str(),
        .time_from = 0,
        .time_to = totalTime,
        .time_step = timeStep,
        .saveCount = saveCount
    };
    simulateSatellites(bodies, simParams);
    std::cerr << "Simulation finished!" << std::endl;

    return 0;
}