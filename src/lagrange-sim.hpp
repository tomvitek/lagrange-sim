#ifndef __LAGRANGE_SIM
#define __LAGRANGE_SIM

#include <eigen3/Eigen/Eigen>

struct TestBody {
    Eigen::Vector2d pos;
    Eigen::Vector2d vel;
};

/**
 * @brief Simulation parameters
 * 
 */
struct SimParams {
    /**
     * @brief Simulation output file
     * 
     */
    const char *outputFile;
    /**
     * @brief Start time of the simulation
     * 
     */
    double time_from;
    /**
     * @brief End time of the simulation
     * 
     */
    double time_to;
    /**
     * @brief Simulation time step
     * 
     */
    double time_step;
    /**
     * @brief Save count (how many points will be simulation save, to avoid huge output files)
     * 
     */
    size_t saveCount;
};

/**
 * @brief Computes derivatives of the speed and position for the given system.
 * 
 * This function can be used by RK4Solver. It takes data in form of 2dimensional array,
 * where each row corresponds to one simulated body, with the vector elements:
 * 
 * 0 - x position,
 * 1 - y position,
 * 2 - x speed,
 * 3 - y speed
 * 
 * @param t Time (in seconds)
 * @param data Data array
 * @return Eigen::ArrayXXd Derivatives
 */
Eigen::ArrayXXd lagrangeFunc(double t, const const Eigen::ArrayXXd& data);

/**
 * @brief Simulates one satellite (body) in the system given by lagrangeFunc (Earth and Moon). Basically equivalent of
 * calling `simulateSatellites` with only one body.
 * 
 * @param pos Initial body position
 * @param vel Initial body velocity
 * @param params Simulation parameters
 */
void simulateSatellite(const Eigen::Ref<Eigen::Vector2d>& pos, const Eigen::Ref<Eigen::Vector2d>& vel, SimParams& params);

/**
 * @brief Simulates system of multiple bodies in the system given by lagrangeFunc (Earth and Moon)
 * 
 * @param bodies List of initial positions and velocities
 * @param params Simulation parameters
 */
void simulateSatellites(std::vector<TestBody>& bodies, SimParams& params);

#endif