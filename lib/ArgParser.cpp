#include "ArgParser.h"

void ArgumentParser::Argument::Default(const bool &flag) {
    def = true;
    this->flag = flag;
}

void ArgumentParser::Help::Default(const bool &flag) {
    def = true;
    help_default = true;
    this->flag = true;
}

void ArgumentParser::Argument::Default(const char *str) {
    def = true;
    parametrs.push_back(str);
}

void ArgumentParser::Argument::StoreValue(std::string &val) {
    store_str = &val;
}

void ArgumentParser::Argument::StoreValue(bool &flag) {
    store_flag = &flag;
}

void ArgumentParser::Help::StoreValue(bool &flag) {
    store_flag = &flag;
}

ArgumentParser::Argument &ArgumentParser::Argument::MultiValue() {
    return *this;
}

ArgumentParser::Argument &ArgumentParser::Argument::MultiValue(int count) {
    min_count = count;
    return *this;
}

ArgumentParser::Help &ArgumentParser::Help::MultiValue(int count) {
    min_count = count;
    help_multi = true;
    return *this;
}

void ArgumentParser::Argument::StoreValues(std::vector<int> &int_values) {
    store_ints = &int_values;
}

ArgumentParser::ArgParser::ArgParser(const char *str) {
    name_parser = str;
}

bool ArgumentParser::ArgParser::Parsing(std::vector<std::string> vector1){
    std::vector<std::pair<std::string, std::string>> parsers;
    std::vector<std::string> positionals;
    std::vector<std::string> flags;
    for (int i = 1; i < vector1.size(); i++) {
        int j = 0;
        while (j < vector1[i].size()) {
            if (vector1[i][j] == '=') {
                break;
            }
            j++;
        }
        if (vector1[i][0] != '-') {
            positionals.push_back(vector1[i]);
        } else if (vector1[i][1] == '-') {
            if (j == vector1[i].size()) {
                flags.push_back(vector1[i].substr(2, vector1[i].size() - 2));
            } else

                parsers.push_back(
                        std::make_pair(vector1[i].substr(2, j - 2),
                                       vector1[i].substr(j + 1, vector1[i].size() - j - 1)));
        } else {
            if (j == vector1[i].size()) {
                for (int t = 1; t < vector1[i].size(); t++)
                    flags.push_back(std::string(1, vector1[i][t]));
            } else
                parsers.push_back(
                        std::make_pair(vector1[i].substr(1, j - 1),
                                       vector1[i].substr(j + 1, vector1[i].size() - j - 1)));
        }
    }
    for (int i = 0; i < arguments.size(); i++) {
        bool success = false;
        for (int j = 0; j < parsers.size(); j++) {
            if (arguments[i].long_name == parsers[j].first || arguments[i].short_name == parsers[j].first) {
                arguments[i].parametrs.push_back(parsers[j].second);
                arguments[i].value = parsers[j].second;
                success = true;
            }
        }
        if (arguments[i].is_flag) {
            for (int j = 0; j < flags.size(); j++) {
                for (int t = 0; t < flags.size(); t++)
                    if (flags[t] == arguments[i].short_name || flags[t] == arguments[i].long_name) {
                        success = true;
                        arguments[i].flag = true;
                        break;
                    }
            }
        }
        if (arguments[i].postitional) {
            for (int j = 0; j < positionals.size(); j++) {

                arguments[i].parametrs.push_back(positionals.front());
                positionals.erase(positionals.begin());
            }
            success = true;
        }
        if (arguments[i].parametrs.size() < arguments[i].min_count)
            return false;
        if (!success && !arguments[i].def)
            return false;
        if (arguments[i].store_ints != nullptr) {
            for (int t = 0; t < arguments[i].parametrs.size(); t++)
                arguments[i].store_ints->push_back(stoi(arguments[i].parametrs[t]));
            for (int t = 0; t < positionals.size(); t++)
                arguments[i].store_ints->push_back(stoi(positionals[t]));
        }
        if (arguments[i].store_str != nullptr)
            *arguments[i].store_str = arguments[i].parametrs[0];
        if (arguments[i].store_flag != nullptr)
            *arguments[i].store_flag = arguments[i].flag;
    }
    for (int i = 0; i < helpers.size(); i++) {
        bool success = false;
        for (int j = 0; j < flags.size(); j++) {
            if (helpers[i].long_name == flags[j] || helpers[i].short_name == flags[j]) {
                helpers[i].flag = true;
                success = true;
                break;
            }
        }
        if (helpers[i].store_flag != nullptr)
            *helpers[i].store_flag = helpers[i].flag;
        if (!success && !helpers[i].def)
            return false;
        if (helpers[i].info.size() < helpers[i].min_count)
            return false;
    }
    return true;
}

bool ArgumentParser::ArgParser::Parse(int argc, char **argv) {
    std::vector<std::string> vector1;
    for(int i = 0; i < argc; i++)
        vector1.push_back(argv[i]);
    return Parsing(vector1);
}

bool ArgumentParser::ArgParser::Parse(std::vector<std::string> vector1) {
    return Parsing(vector1);
}

ArgumentParser::Argument &ArgumentParser::ArgParser::AddStringArgument(const char *str) {
    Argument arg;
    arg.long_name = str;
    arguments.push_back(arg);
    return arguments.back();
}

std::string ArgumentParser::ArgParser::GetStringValue(const char *str) {
    std::string string;
    for (int i = 0; i < arguments.size(); i++) {
        if (arguments[i].long_name == str || "" + arguments[i].short_name == str) {
            string = arguments[i].parametrs[0];
        }
    }
    return string;
}

ArgumentParser::Argument &ArgumentParser::ArgParser::AddStringArgument(char str1, const char *str2) {
    Argument arg;
    arg.long_name = str2;
    arg.short_name = std::string(1, str1);
    arguments.push_back(arg);
    return arguments.back();
}

ArgumentParser::Argument &ArgumentParser::ArgParser::AddIntArgument(const char *str) {
    Argument arg;
    arg.long_name = str;
    arguments.push_back(arg);
    return arguments.back();
}

int ArgumentParser::ArgParser::GetIntValue(const char *str) {
    int num;
    for (int i = 0; i < arguments.size(); i++) {
        if (arguments[i].long_name == str || "" + arguments[i].short_name == str) {
            num = stoi(arguments[i].parametrs[0]);
        }
    }
    return num;
}

int ArgumentParser::ArgParser::GetIntValue(const char *str, int number) {
    int num;
    for (int i = 0; i < arguments.size(); i++) {
        if (arguments[i].long_name == str || "" + arguments[i].short_name == str) {
            num = stoi(arguments[i].parametrs[number]);
        }
    }
    return num;
}

ArgumentParser::Argument &ArgumentParser::ArgParser::AddIntArgument(char str1, const char *str2) {
    Argument arg;
    arg.long_name = str2;
    arg.short_name = str1;
    arguments.push_back(arg);
    return arguments.back();
}

ArgumentParser::Argument &ArgumentParser::ArgParser::AddFlag(char str1, const char *str2) {
    Argument arg;
    arg.long_name = str2;
    arg.short_name = str1;
    arg.is_flag = true;
    arguments.push_back(arg);
    return arguments.back();
}

bool ArgumentParser::ArgParser::GetFlag(const char *str) {
    bool flag;
    for (int i = 0; i < arguments.size(); i++) {
        if ((arguments[i].long_name == str || "" + arguments[i].short_name == str) && arguments[i].is_flag) {
            flag = arguments[i].flag;
            break;
        }
    }
    return flag;
}

ArgumentParser::Argument &ArgumentParser::Argument::Positional() {
    postitional = true;
    return *this;
}

void ArgumentParser::ArgParser::AddHelp(char str1, const char *str2, const char *information) {
    class Help helper;
    helper.short_name = std::string(1, str1);
    helper.long_name = str2;
    helper.help_help = true;
    helper.info.push_back(information);
    helpers.push_back(helper);
}

bool ArgumentParser::ArgParser::Help() {
    if (helpers.empty())
        return false;
    return true;
}

ArgumentParser::Help &ArgumentParser::ArgParser::AddStringArgument(char str1, const char *str2, const char *str3) {
    class Help helper;
    helper.def = true;
    helper.short_name = std::string(1, str1);
    helper.long_name = str2;
    helper.info.push_back(str3);
    helper.help_string = true;
    helpers.push_back(helper);
    return helpers.back();
}

ArgumentParser::Help &ArgumentParser::ArgParser::AddFlag(char str1, const char *str2, const char *str3) {
    class Help helper;
    helper.def = true;
    helper.short_name = std::string(1, str1);
    helper.long_name = str2;
    helper.info.push_back(str3);
    helpers.push_back(helper);
    return helpers.back();
}

ArgumentParser::Help &ArgumentParser::ArgParser::AddFlag(const char *str2, const char *str3) {
    class Help helper;
    helper.def = true;
    helper.long_name = str2;
    helper.info.push_back(str3);
    helpers.push_back(helper);
    return helpers.back();
}

ArgumentParser::Help &ArgumentParser::ArgParser::AddIntArgument(const char *str1, const char *str2) {
    class Help helper;
    helper.def = true;
    helper.long_name = str1;
    helper.info.push_back(str2);
    helper.help_int = true;
    helpers.push_back(helper);
    return helpers.back();
}

std::string ArgumentParser::ArgParser::HelpDescription() {
    std::string out = name_parser + "\n";
    for (int i = 0; i < helpers.size(); i++) {
        if (helpers[i].help_help) {
            out += helpers[i].info.back();
            out += "\n\n";
        }
        else {
            if (helpers[i].short_name != "")
                out += "-" + helpers[i].short_name + ",  --" + helpers[i].long_name;
            else
                out += "     --" + helpers[i].long_name;
            if (helpers[i].help_string)
                out += "=<string>,";
            else if (helpers[i].help_int)
                out += "=<int>,";
            else out += ",";
            out += "  " + helpers[i].info.back();
            if (helpers[i].help_multi) {
                out += " [repeated, min args = ";
                out += std::to_string(helpers[i].min_count);
                out +=  "]\n";
            }
            else if (helpers[i].help_default)
                out += " [default = true]\n";
            else out += "\n";
        }
    }
    out += "\n";
    for(int i = 0; i < helpers.size(); i++)
        if(helpers[i].help_help){
            out += "-" + helpers[i].short_name + ",  --" + helpers[i].long_name;
            out += " Display this help and exit\n";
        }
    return out;
}
