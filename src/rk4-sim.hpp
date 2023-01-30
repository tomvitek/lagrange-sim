#ifndef __RK4_SIM
#define __RK4_SIM

#include <eigen3/Eigen/Eigen>
#include "sim-writer.hpp"

template <typename Derived> 
using SolverFunc = Derived (*)(double t, const Derived& x0);

/**
 * @brief Solver for system of differential equations with initial conditions
 * 
 * @tparam Which data class is used to represent data (e.g. Eigen::ArrayXXd for double or Eigen::ArrayXXf for float)
 */
template<typename Derived>
class RK4Solver {
public:
    /**
     * @brief Initiates a new RK4Solver instance
     * 
     * @param func Function representing system of differetial equations
     * @param writer SimWriter instance used to write the output of the simulation
     */
    RK4Solver(SolverFunc<Derived> func, SimWriter<Derived>& writer) : func(func), writer(writer) {
        
    }

    /**
     * @brief Solves the system of differential equations using initial conditions and saves the solution to the given file
     * 
     * @param t_from Time from which to start the simulation
     * @param t_step Time step
     * @param t_to Time of the end of the simulation
     * @param x0 Initial conditions (this will be fed as param to the func)
     * @param saveCount How many times during the simulation the state should be saved
     * @return Final state of the simulation
     */
    Derived solve(
        double t_from, 
        double t_step,
        double t_to, 
        Eigen::ArrayBase<Derived>& x0,
        uint32_t saveCount
    ) {
        Derived x = x0;
        this->writer.open();
        writer.write(0, x);

        if (writeStatus)
            std::cerr << "0%" << std::flush;
        const uint64_t iterationCount = (t_to - t_from) / t_step;
        const double saveTimeDelta = (t_to - t_from) / (saveCount - 1);
        double nextSaveTime = saveTimeDelta;
        uint64_t iteration = 0;

        while (iteration < iterationCount) {
            double t = iteration * t_step;
            
            x = this->step(t, t_step, x);
            iteration++;
            if (t > nextSaveTime) {
                writer.write(t, x);
                nextSaveTime += saveTimeDelta;

                if (this->writeStatus){
                    
                    std::cerr << "\r                 \r" << iteration * 1000 / iterationCount / 10.0 << "%" << std::flush;
                }
            }
        }

        this->writer.write(t_to, x);
        this->writer.close();
        std::cerr << std::endl;
        return x;
    }

    /**
     * @brief This is used to write output of the simulation in 'solve' method.
     * 
     */
    SimWriter<Derived>& writer;
    /**
     * @brief Function representing the system of differential equations.
     * 
     */
    SolverFunc<Derived> func;
    /**
     * @brief Set this to false if you don't want to see simulation percentage in cerr.
     */
    bool writeStatus = true;
protected:
    /**
     * @brief Performs one step of the simulation
     * 
     * @param t Time of the simulation
     * @param t_step Simulation time step
     * @param x State of the system (initial conditions) before the step
     * @return State of the system after the step
     */
    Derived step(double t, double t_step, Eigen::ArrayBase<Derived>& x) {
        Derived k1 = t_step * this->func(t, x);
        Derived k2 = t_step * this->func(t + t_step / 2, x + k1);
        Derived k3 = t_step * this->func(t + t_step / 2,  x + k2);
        Derived k4 = t_step * this->func(t + t_step, x + k3);    
        return x + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
    }
};

#endif