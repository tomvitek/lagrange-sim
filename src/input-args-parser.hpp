#ifndef __INPUT_ARGS_PARSER
#define __INPUT_ARGS_PARSER

#include <string>
#include <vector>

/**
 * @brief Parses arguments from the command line and helps sorting them out.
 * 
 */
class InputArgsParser {
public:
    /**
     * @brief Construct a new Input Args Parser object
     * 
     * @param argc First argument of `main` function
     * @param argv Second argument of `main` function
     */
    InputArgsParser(const int argc, const char *argv[]);
    /**
     * @brief Get the value after the option
     * 
     * For example, when parameters are "--save --output file1", this returns "file1" for option "--output"
     * 
     * When the option doesn't exist, a std::runtime_error is thrown
     * @param option Option to search for
     * @return Value of the option
     */
    const std::string getValue(const std::string &option) const;
    /**
     * @brief Get the value after the option
     * 
     * For example, when parameters are "--save --output file1 input_file", this returns "file1" for option "--output"
     * 
     * When the arguments don't contain the option, the `def` value is returned.
     * @param option Option to search for
     * @param def Default value to return
     * @return Value of the option
     */
    const std::string getValue(const std::string &option, const std::string &def) const;
    /**
     * @brief Same as `getValue(option)`, but returns a list of items after the option until one doesn't start with '-' symbol
     * 
     * For example, when parameters are "--save --input input_file1 input_file2 -o output_file", this returns {input_file1, input_file2}
     * 
     * When the option doesn't exist, a std::runtime_error is thrown
     * @param option Option to search for
     * @return List (vector) from the option
     */
    const std::vector<std::string> getList(const std::string &option) const;
    /**
     * @brief Returns true if the arguments contain the option string
     * 
     * @param option Option string to search for
     */
    bool optionExists(const std::string &option) const;
private:
    std::vector<std::string> argv;
    /**
     * @brief Returns `argv` iterator starting at the first value of the option
     * 
     * @param option Option string to search for
     */
    std::vector<std::string>::const_iterator getOptionIter(const std::string &option) const;
};

#endif