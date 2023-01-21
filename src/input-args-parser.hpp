#ifndef __INPUT_ARGS_PARSER
#define __INPUT_ARGS_PARSER

#include <string>
#include <vector>

class InputArgsParser {
public:
    InputArgsParser(const int argc, const char *argv[]);
    const std::string getValue(const std::string &option) const;
    const std::string getValue(const std::string &option, const std::string &def) const;
    const std::vector<std::string> getList(const std::string &option) const;
    bool optionExists(const std::string &option) const;
private:
    std::vector<std::string> argv;
    std::vector<std::string>::const_iterator getOptionIter(const std::string &option) const;
};

#endif