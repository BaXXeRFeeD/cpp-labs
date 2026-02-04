#include <iostream>
#include <cpr/cpr.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <conio.h>
#include "City.h"
#include "windows.h"

int main() {
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    Weather weath;

//    cpr::Response r = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city?name=Moscow"}, cpr::Header{{"X-Api-Key", "/hquTPVF7IdOdyLHL78dNA==EoT2VY8uJ5nzGcTL"}});

    std::ifstream config("config.json");
    nlohmann::json config_data = nlohmann::json::parse(config);
    config.close();
    int day_count, size, rate;
    City *array;
    if (config_data["names"] == nullptr) {
        std::cout << "Write city names\n";
        return 0;
    } else if (config_data["day_count"] == nullptr) {
        std::cout << "Write day count\n";
        return 0;
    } else if (config_data["rate"] == nullptr) {
        std::cout << "Write rate count (in hours)\n";
        return 0;
    } else {
        day_count = config_data["day_count"];
        rate = config_data["rate"];
        size = config_data["names"].size();
        array = new City[size];
        if (day_count < 0 || day_count > 16) {
            std::cout << "day_count may be in [0; 16]\n";
            return 0;
        } else if (size == 0) {
            std::cout << "Write city names\n";
            return 0;
        } else if (rate == 0) {
            std::cout << "Write rate count (in hours)\n";
            return 0;
        }
        for (int i = 0; i < size; i++) {
            array[i] = City(config_data["names"][i]);
        }
        for (int i = 0; i < size; i++) {
            if (!array[i].Parse())
                return 0;
        }
    }
    system("cls");
    weath.make_weather();
    array[0].Out(day_count, weath);

    int iter = 0;

    while (true) {
        if (_getch() == '=' && day_count < 16) {
            system("cls");
            day_count++;
            array[iter].Out(day_count, weath);
        } else if (_getch() == '-' && day_count > 0) {
            system("cls");
            day_count--;
            array[iter].Out(day_count, weath);
        } else if (_getch() == 'n') {
            iter++;
            if (iter == size)
                iter = 0;
            system("cls");
            array[iter].Out(day_count, weath);
        } else if (_getch() == 'p') {
            iter--;
            if (iter == -1)
                iter = size - 1;
            system("cls");
            array[iter].Out(day_count, weath);
        } else if (_getch() == 27) {
            for (int j = 0; j < size; j++) {
                array[j].Destroy();
                system("taskkill /IM lab10.exe & taskkill /IM cmd.exe");
            }
        }
    }
}