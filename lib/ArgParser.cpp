#include <vector>
#include "ArgParser.h"

ArgumentParser::ArgParser::ArgParser(std::string str) {
    name_parser = str;
}

bool ArgumentParser::ArgParser::Parse(std::vector<std::string> vector1) {
    for (int i = 1; i < vector1.size(); i++) {
        int j = 0;
        while (j < vector1.size()) {
            if (vector1[i][j] == '=') {
                break;
            }
            j++;
        }
        if(j == vector1.size() - 1 && vector1[i][j] != '=')
            j = 0;
        if (vector1[i][1] == '-')
            parsers.push_back(std::make_pair(vector1[i].substr(2, j - 2), vector1[i].substr(j + 1, vector1[i].size() - j - 1)));
        else
            parsers.push_back(std::make_pair(vector1[i].substr(1, j - 1), vector1[i].substr(j + 1, vector1[i].size() - j - 1)));
        if(vector1[i][vector1[i].size() - 1] == '=')
            return false;
    }
    for(int i = 0; i < arguments.size(); i++){
        bool success = false;
        for(int j = 0; j < parsers.size(); j++){
            if(arguments[i].first == parsers[j].first){
                arguments[i].second = parsers[j].second;
                success = true;
                break;
            }
        }
        if(!success)
            return false;
    }
    return true;
}

void ArgumentParser::ArgParser::AddStringArgument(const char *str) {
    arguments.push_back(std::make_pair(str, ""));
}

std::string ArgumentParser::ArgParser::GetStringValue(const char *str) {
    bool success = false;
    for(int i = 0; i < arguments.size(); i++){
        if(arguments[i].first == str){
            success = true;
            break;
            return arguments[i].second;
        }
    }
    if(!success)
        return "Error";
}

void ArgumentParser::ArgParser::AddStringArgument(const char *str1, const char *str2) {

}
