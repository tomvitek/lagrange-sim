#ifndef __SIM_WRITER
#define __SIM_WRITER

#include <eigen3/Eigen/Eigen>
#include <fstream>

/**
 * @brief Represents a class used to write output of the simulation to the specified csv file.
 * 
 * @tparam Which class is used for system data storage (e.g. Eigen::ArrayXXd for double or Eigen::ArrayXXf for float)
 */
template<typename Derived>
class SimWriter {
public:
    /**
     * @brief Construct a new Sim Writer object.
     * 
     * Note that you also need to specify the output path before using this, otherwise
     * an exception will be thrown.
     * 
     */
    SimWriter() {
        this->outputPath = "";
    }

    /**
     * @brief Construct a new Sim Writer object
     * 
     * @param outputPath Path to the file where the simulation will be saved. Can be changed later.
     */
    SimWriter(const char *outputPath) {
        this->outputPath = outputPath;
    }

    /**
     * @brief Set the Output Path
     */
    void setOutputPath(const char *outputPath) {
        this->outputPath = outputPath;
    }

    /**
     * @brief Opens the file with the given outputPath and prepares for writing simulation output.
     * 
     */
    void open() {
        if (this->outputPath == "")
            throw std::runtime_error("SimWriter needs outputPath to write to the file, but it wasn't provided.");

        this->file.open(this->outputPath);

        if (! this->file.good())
            throw std::runtime_error("Failed to write to file: \"" + this->outputPath + '\"');
    }

    /**
     * @brief Writes one simulation step to the opened file. 
     * 
     * @param t Time of the simulation (this will be in the first column)
     * @param data Simulation data (state of the system)
     */
    void write(double t, const Eigen::ArrayBase<Derived>& data) {
        file << t;

        for (size_t row_i = 0; row_i < data.rows(); ++row_i) {
            for (size_t col_i = 0; col_i < data.cols(); ++col_i) {
                file << ',' << data(row_i, col_i);
            }
        }
        file << '\n';
    }

    /**
     * @brief Flushes the stream and closes the output file. If the file is already closed, does nothing.
     * 
     */
    void close() {
        if (this->file.is_open())
            this->file.close();
    }

    std::string outputPath;
private:
    std::ofstream file;
};

#endif