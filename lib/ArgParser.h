#pragma once

#include <string>
#include <vector>

namespace ArgumentParser {

class ArgParser {
    std::vector<std::pair<std::string, std::string>> parsers;
    std::vector<std::pair<std::string, std::vector<std::string>>> arguments;
    std::string name_parser;
    class Options{

    };
    // Your Implementation here!
public:
    ArgParser();

    ArgParser(std::string str);

    bool Parse(std::vector<std::string> vector1);

    void AddStringArgument(const char *str);

    std::string GetStringValue(const char *str);

    void AddStringArgument(const char *str1, const char *str2);
};

} // namespace ArgumentParser