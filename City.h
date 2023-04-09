#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <cstdio>
#include "Weather.h"

struct Day {
    std::string* temperature;
    std::string* degree;
    std::string* humidity;
    std::string* prob;
    std::string* wind;
    std::pair<std::string, char>* weather_code;
    std::string date;

    Day() {}

    Day(std::string date, std::pair<std::string, char> *weatherCode, std::string *temperature, std::string *wind, std::string *degree, std::string *humidity, std::string *prob) : date(date), weather_code(
            weatherCode), temperature(temperature), wind(wind), degree(degree), humidity(humidity), prob(prob) {}

};

class City {
    std::string name;
    std::string latitude;
    std::string longitude;
public:
    Day* days = new Day[17];
    void addDay(Day day, int index) {
        days[index] = day;
    }

    Day getDay(int index){
        return days[index];
    }

    City() {}

    City(std::string name) : name(name) {}

    bool Parse();

    void Out(int day_count, Weather weather);

    void Destroy();
};
