#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace ArgumentParser {

    class Help {
    public:
        std::string long_name = "";
        std::string short_name = "";
        std::vector<std::string> info;
        bool flag = false;
        int min_count = 0;
        bool def = false;

        bool* store_flag;

        bool help_string = false;
        bool help_default = false;
        bool help_multi = false;
        bool help_int = false;
        bool help_help = false;

        Help& MultiValue(int count);

        void Default(const bool &flag);

        void StoreValue(bool &flag);
    };

    class Argument {
    public:
        std::string long_name;
        std::string short_name;
        std::vector<std::string> parametrs;
        std::string value;
        int min_count = 0;
        bool is_flag = false;
        bool flag = false;
        bool def = false;
        bool postitional = false;

        bool *store_flag = nullptr;
        std::vector<int> *store_ints = nullptr;
        std::string *store_str = nullptr;

        void Default(const bool &flag);

        void Default(const char *str);

        void StoreValue(std::string &val);

        void StoreValue(bool &flag);

        Argument &MultiValue();

        Argument &MultiValue(int count);

        void StoreValues(std::vector<int> &int_values);

        Argument &Positional();
    };

    class ArgParser {

        std::string name_parser;
        std::vector<std::string> storevalues;
        std::vector<Argument> arguments;
        std::vector<Help> helpers;
        // Your Implementation here!
    public:
        ArgParser();

        ArgParser(const char *str);

        bool Parse(std::vector<std::string> vector1);

        bool Parse(int argc, char **argv);

        bool Parsing(std::vector<std::string> vector1);

        ArgumentParser::Argument &AddStringArgument(const char *str);

        std::string GetStringValue(const char *str);

        ArgumentParser::Argument &AddStringArgument(char str1, const char *str2);

        ArgumentParser::Argument &AddIntArgument(const char *str);

        int GetIntValue(const char *str);

        int GetIntValue(const char *str, int number);

        ArgumentParser::Argument &AddIntArgument(char str1, const char *str2);

        ArgumentParser::Argument &AddFlag(char str1, const char *str2);

        bool GetFlag(const char *str);

        void AddHelp(char i, const char *string, const char *string1);

        bool Help();

        ArgumentParser::Help &AddStringArgument(char str1, const char *str2, const char *str3);

        ArgumentParser::Help &AddFlag(char str1, const char *str2, const char *str3);

        ArgumentParser::Help &AddFlag(const char *str2, const char *str3);

        ArgumentParser::Help &AddIntArgument(const char *str1, const char *str2);

        std::string HelpDescription();
    };

} // namespace ArgumentParser