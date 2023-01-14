#ifndef __LAGRANGE_SIM
#define __LAGRANGE_SIM

#include <eigen3/Eigen/Eigen>

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

Eigen::ArrayXXd lagrangeFunc(double t, const Eigen::Ref<Eigen::ArrayXXd>& data);

void simulateSatellite(const Eigen::Ref<Eigen::Vector2d>& pos, const Eigen::Ref<Eigen::Vector2d>& vel, SimParams& params);

#endif