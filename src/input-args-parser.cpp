#include "input-args-parser.hpp"
#include <algorithm>
#include <stdexcept>

InputArgsParser::InputArgsParser(const int argc, const char *argv[]) {
    for (size_t i = 0; i < argc; ++i) {
        this->argv.push_back(argv[i]);
    }
}

const std::string InputArgsParser::getValue(const std::string &option) const {
    auto optionIter = this->getOptionIter(option);

    return *(optionIter + 1);
}

const std::string InputArgsParser::getValue(const std::string &option, const std::string &def) const {
    try {
        return this->getValue(option);
    }
    catch (std::runtime_error e) {
        return def;
    }
}

const std::vector<std::string> InputArgsParser::getList(const std::string &option) const {
    auto optionIter = this->getOptionIter(option);

    auto nextOptionIter = std::find_if(optionIter + 1, this->argv.end(), [](std::string arg) { return arg[0] == '-'; });
    return std::vector(optionIter + 1, nextOptionIter);
}

bool InputArgsParser::optionExists(const std::string &option) const {
    return std::find(this->argv.begin(), this->argv.end(), option) != this->argv.end();
}

std::vector<std::string>::const_iterator InputArgsParser::getOptionIter(const std::string &option) const {
    auto optionIter = std::find(this->argv.begin(), this->argv.end(), option);
    if (optionIter == this->argv.end())
        throw std::runtime_error("No option \"" + option + "\" given");

    return optionIter;
}