#include "City.h"

void weather_out(std::string weath, char code, int iter, HANDLE &cons) {
    if (code == 0) {
        SetConsoleTextAttribute(cons, 14);
        std::cout << weath.substr(iter, 15);
        SetConsoleTextAttribute(cons, 15);
    } else if (code == 1) {
        SetConsoleTextAttribute(cons, 15);
        std::cout << weath.substr(iter, 15);
    } else if (code == 2) {
        SetConsoleTextAttribute(cons, 15);
        std::cout << weath.substr(iter, 15);
    } else if (code == 3) {
        if (iter == 0) {
            SetConsoleTextAttribute(cons, 14);
            std::cout << weath.substr(iter, 15);
            SetConsoleTextAttribute(cons, 15);
        } else if (iter == 15) {
            SetConsoleTextAttribute(cons, 14);
            std::cout << weath.substr(iter, 15);
            SetConsoleTextAttribute(cons, 15);
        } else if (iter == 30) {
            SetConsoleTextAttribute(cons, 14);
            std::cout << weath.substr(iter, 7);
            SetConsoleTextAttribute(cons, 15);
            std::cout << weath.substr(iter + 7, 8);
        } else if (iter == 45) {
            SetConsoleTextAttribute(cons, 15);
            std::cout << weath.substr(iter, 15);
        } else {
            SetConsoleTextAttribute(cons, 14);
            std::cout << weath.substr(iter, 6);
            SetConsoleTextAttribute(cons, 15);
            std::cout << weath.substr(iter + 6, 9);
        }
    } else if (code == 4) {
        if (iter == 0) {
            SetConsoleTextAttribute(cons, 14);
            std::cout << weath.substr(iter, 15);
            SetConsoleTextAttribute(cons, 15);
        } else if (iter == 15) {
            SetConsoleTextAttribute(cons, 14);
            std::cout << weath.substr(iter, 7);
            SetConsoleTextAttribute(cons, 15);
            std::cout << weath.substr(iter + 7, 8);
        } else if (iter == 30) {
            SetConsoleTextAttribute(cons, 15);
            std::cout << weath.substr(iter, 15);
        } else if (iter == 45) {
            SetConsoleTextAttribute(cons, 14);
            std::cout << weath.substr(iter, 6);
            SetConsoleTextAttribute(cons, 15);
            std::cout << weath.substr(iter + 6, 9);
        } else {
            SetConsoleTextAttribute(cons, 3);
            std::cout << weath.substr(iter, 15);
            SetConsoleTextAttribute(cons, 15);
        }
    } else {
        if (iter == 0 || iter == 15 || iter == 30 || iter == 45) {
            SetConsoleTextAttribute(cons, 15);
            std::cout << weath.substr(iter, 15);
        } else {
            SetConsoleTextAttribute(cons, 3);
            std::cout << weath.substr(iter, 15);
            SetConsoleTextAttribute(cons, 15);
        }
    }
}

bool City::Parse() {
    std::ofstream("time.json");
    std::string str = "https://api.api-ninjas.com/v1/city?name=";
    str += name;
    cpr::Response r = cpr::Get(cpr::Url{str}, cpr::Header{{"X-Api-Key", "/hquTPVF7IdOdyLHL78dNA==EoT2VY8uJ5nzGcTL"}});
    std::fstream file("time.json");
    file << "{\"city\":";
    if(r.text == "{\"city\":}")
        return 0;
    file << r.text;
    file.close();
    file.open("time.json", std::ios::app | std::ios::ate);
    file << "}";
    file.close();

    file.open("time.json");
    nlohmann::json city_info = nlohmann::json::parse(file);

    latitude = city_info["city"][0]["latitude"].dump();
    longitude = city_info["city"][0]["longitude"].dump();


    file.close();
    std::remove("time.json");


    std::string str_w = "https://api.open-meteo.com/v1/forecast?latitude=";
    str_w += latitude;
    str_w += "&longitude=";
    str_w += longitude;

    time_t t;
    time(&t);
    tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H");
    std::string date1 = oss.str();

    t += 1296000;
    tm = *std::localtime(&t);

    oss.str("");
    oss.clear();
    oss << std::put_time(&tm, "%Y-%m-%d %H");
    std::string date2 = oss.str();

    int hour_now = stoi(date1.substr(date1.size() - 2, 2));
//    std::cout << hour_now;

//    std::cout << date1 << " " << date2;

    str_w += "&hourly=temperature_2m,relativehumidity_2m,precipitation_probability,precipitation,weathercode,windspeed_10m,winddirection_10m&forecast_days=1&start_date=";
    str_w += date1.substr(0, date1.size() - 3);
    str_w += "&end_date=";
    str_w += date2.substr(0, date2.size() - 3);
    cpr::Response weather = cpr::Get(cpr::Url{str_w});
    std::ofstream("time.json");
    file.open("time.json");
    file << weather.text;
    file.close();
    file.open("time.json");

    city_info = nlohmann::json::parse(file);

    std::string *humidity_now = new std::string[1];
    humidity_now[0] = city_info["hourly"]["relativehumidity_2m"][hour_now].dump();
    std::string *temperature_now = new std::string[1];
    temperature_now[0] = city_info["hourly"]["temperature_2m"][hour_now].dump();

    std::string *prob_now = new std::string[1];
    prob_now[0] = city_info["hourly"]["precipitation"][hour_now].dump();
    std::pair<std::string, char> *weather_code_now = new std::pair<std::string, char>[1];
    weather_code_now[0] = std::make_pair(city_info["hourly"]["weathercode"][hour_now].dump(), 0);

    std::string *wind_now = new std::string[1];
    std::string *degree_now = new std::string[1];
    wind_now[0] = city_info["hourly"]["windspeed_10m"][hour_now].dump();
    degree_now[0] = city_info["hourly"]["winddirection_10m"][hour_now].dump();

    if (weather_code_now[0].first == "0") {
        weather_code_now[0].second = 0;
        weather_code_now[0].first = "ßñíî           ";
    } else if (weather_code_now[0].first == "1" || weather_code_now[0].first == "2") {
        if (weather_code_now[0].first == "1")
            weather_code_now[0].first = "Ïğåèìóù. ÿñíî  ";
        else
            weather_code_now[0].first = "Ïåğåì. îáëà÷í. ";
        weather_code_now[0].second = 3;
    } else if (weather_code_now[0].first == "3") {
        weather_code_now[0].first = "Îáëà÷íî        ";
        weather_code_now[0].second = 1;
    } else if (weather_code_now[0].first == "45" || weather_code_now[0].first == "48") {
        if (weather_code_now[0].first == "45")
            weather_code_now[0].first = "Òóìàí          ";
        else
            weather_code_now[0].first = "Èçìîğîçü       ";
        weather_code_now[0].second = 2;
    } else if (weather_code_now[0].first == "51" || weather_code_now[0].first == "53" ||
               weather_code_now[0].first == "55" ||
               weather_code_now[0].first == "56" || weather_code_now[0].first == "57") {
        if (weather_code_now[0].first == "56" || weather_code_now[0].first == "57")
            weather_code_now[0].first = "Ëåäÿííàÿ ìîğîñü";
        else
            weather_code_now[0].first = "Ìîğîñü         ";
        weather_code_now[0].second = 4;
    } else if (weather_code_now[0].first == "61" || weather_code_now[0].first == "63" ||
               weather_code_now[0].first == "65" ||
               weather_code_now[0].first == "66" || weather_code_now[0].first == "67") {
        if (weather_code_now[0].first == "66" || weather_code_now[0].first == "67")
            weather_code_now[0].first = "Ëåäÿííîé äîæäü ";
        else
            weather_code_now[0].first = "Äîæäü          ";
        weather_code_now[0].second = 5;
    } else if (weather_code_now[0].first == "71" || weather_code_now[0].first == "73" ||
               weather_code_now[0].first == "75" ||
               weather_code_now[0].first == "77") {
        if (weather_code_now[0].first == "77")
            weather_code_now[0].first = "Ñíåæíûå çåğíà  ";
        else
            weather_code_now[0].first = "Ñíåãîïàä       ";
        weather_code_now[0].second = 6;
    } else if (weather_code_now[0].first == "81" || weather_code_now[0].first == "82" ||
               weather_code_now[0].first == "80" ||
               weather_code_now[0].first == "85" || weather_code_now[0].first == "86" ||
               weather_code_now[0].first == "95") {
        if (weather_code_now[0].first == "95")
            weather_code_now[0].first = "Ãğîçà";
        else if (weather_code_now[0].first == "85" || weather_code_now[0].first == "86")
            weather_code_now[0].first = "Ñíåã           ";
        else
            weather_code_now[0].first = "Ëèâíåâûé äîæäü ";
        weather_code_now[0].second = 7;
    }

    addDay(Day(date1.substr(0, date1.size() - 3), weather_code_now, temperature_now, wind_now, degree_now, humidity_now,
               prob_now), 0);

    for (int i = 1; i < 17; i++) {
        std::pair<std::string, char> *weather_code = new std::pair<std::string, char>[4];
        std::string *temperature = new std::string[4];
        std::string *wind = new std::string[4];
        std::string *degree = new std::string[4];
        std::string *humidity = new std::string[4];
        std::string *prob = new std::string[4];
        for (int j = 0; j < 4; j++) {
            temperature[j] = city_info["hourly"]["temperature_2m"][i * 24 + j * 6 - 1].dump();
            humidity[j] = city_info["hourly"]["relativehumidity_2m"][i * 24 + j * 6 - 1].dump();
            prob[j] = city_info["hourly"]["precipitation"][i * 24 + j * 6 - 1].dump();
            weather_code[j].first = city_info["hourly"]["weathercode"][i * 24 + j * 6 - 1].dump();
            wind[j] = city_info["hourly"]["windspeed_10m"][i * 24 + j * 6 - 1].dump();
            degree[j] = city_info["hourly"]["winddirection_10m"][i * 24 + j * 6 - 1].dump();
            if (weather_code[j].first == "0") {
                weather_code[j].second = 0;
                weather_code[j].first = "ßñíî           ";
            } else if (weather_code[j].first == "1" || weather_code[j].first == "2") {
                if (weather_code[j].first == "1")
                    weather_code[j].first = "Ïğåèìóù. ÿñíî  ";
                else
                    weather_code[j].first = "Ïåğåì. îáëà÷í. ";
                weather_code[j].second = 3;
            } else if (weather_code[j].first == "3") {
                weather_code[j].first = "Îáëà÷íî        ";
                weather_code[j].second = 1;
            } else if (weather_code[j].first == "45" || weather_code[j].first == "48") {
                if (weather_code[j].first == "45")
                    weather_code[j].first = "Òóìàí          ";
                else
                    weather_code[j].first = "Èçìîğîçü       ";
                weather_code[j].second = 2;
            } else if (weather_code[j].first == "51" || weather_code[j].first == "53" ||
                       weather_code[j].first == "55" ||
                       weather_code[j].first == "56" || weather_code[j].first == "57") {
                if (weather_code[j].first == "56" || weather_code[j].first == "57")
                    weather_code[j].first = "Ëåäÿííàÿ ìîğîñü";
                else
                    weather_code[j].first = "Ìîğîñü         ";
                weather_code[j].second = 4;
            } else if (weather_code[j].first == "61" || weather_code[j].first == "63" ||
                       weather_code[j].first == "65" ||
                       weather_code[j].first == "66" || weather_code[j].first == "67") {
                if (weather_code[j].first == "66" || weather_code[j].first == "67")
                    weather_code[j].first = "Ëåäÿííîé äîæäü ";
                else
                    weather_code[j].first = "Äîæäü          ";
                weather_code[j].second = 5;
            } else if (weather_code[j].first == "71" || weather_code[j].first == "73" ||
                       weather_code[j].first == "75" ||
                       weather_code[j].first == "77") {
                if (weather_code[j].first == "77")
                    weather_code[j].first = "Ñíåæíûå çåğíà  ";
                else
                    weather_code[j].first = "Ñíåãîïàä       ";
                weather_code[j].second = 6;
            } else if (weather_code[j].first == "81" || weather_code[j].first == "82" ||
                       weather_code[j].first == "80" ||
                       weather_code[j].first == "85" || weather_code[j].first == "86" ||
                       weather_code[j].first == "95") {
                if (weather_code[j].first == "95")
                    weather_code[j].first = "Ãğîçà";
                else if (weather_code[j].first == "85" || weather_code[j].first == "86")
                    weather_code[j].first = "Ñíåã           ";
                else
                    weather_code[j].first = "Ëèâíåâûé äîæäü ";
                weather_code[j].second = 7;
            }
            wind[j] = city_info["hourly"]["windspeed_10m"][i * 5 + j].dump();
            degree[j] = city_info["hourly"]["winddirection_10m"][i * 5 + j].dump();
            time_t t_cur;
            time(&t_cur);
            t_cur += 86400 * i;
            struct tm tm_cur = *std::localtime(&t_cur);

            std::ostringstream oss_cur;
            oss_cur << std::put_time(&tm_cur, "%Y-%m-%d");
            std::string date_cur = oss_cur.str();
            addDay(Day(date_cur, weather_code, temperature, wind, degree, humidity, prob), i);
        }
    }
    remove("time.json");
    return 1;
}


void City::Out(int day_count, Weather weather) {
    std::cout << "\n";
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    HANDLE cons = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(cons, 15);

    std::cout << "Ïğîãíîç ïîãîäû: " + name + '\n' + '\n';

    Day day = getDay(0);

    std::string weath = weather.weather[day.weather_code[0].second];

    int wind_color = 15;

    if (stoi(day.wind[0]) < 10)
        wind_color = 10;
    else if (stoi(day.wind[0]) >= 10 && stoi(day.wind[0]) < 20)
        wind_color = 14;
    else if (stoi(day.wind[0]) >= 20)
        wind_color = 12;

    weather_out(weath, day.weather_code[0].second, 0, cons);
    std::cout << day.weather_code[0].first << "\n";
    weather_out(weath, day.weather_code[0].second, 15, cons);
    std::cout << day.temperature[0] << " °C\n";
    weather_out(weath, day.weather_code[0].second, 30, cons);
    if (day.degree[0] < "22" || day.degree[0] >= "337") {
        SetConsoleTextAttribute(cons, 15);
        std::cout << "Ñ ";
        SetConsoleTextAttribute(cons, wind_color);
        std::cout << day.wind[0];
        SetConsoleTextAttribute(cons, 15);
        std::cout << " êì/÷\n";
    } else if (day.degree[0] >= "22" && day.degree[0] < "67") {
        SetConsoleTextAttribute(cons, 15);
        std::cout << "ÑÂ ";
        SetConsoleTextAttribute(cons, wind_color);
        std::cout << day.wind[0];
        SetConsoleTextAttribute(cons, 15);
        std::cout << " êì/÷\n";
    } else if (day.degree[0] >= "67" && day.degree[0] < "122") {
        SetConsoleTextAttribute(cons, 15);
        std::cout << "Â ";
        SetConsoleTextAttribute(cons, wind_color);
        std::cout << day.wind[0];
        SetConsoleTextAttribute(cons, 15);
        std::cout << " êì/÷\n";
        SetConsoleTextAttribute(cons, 15);
    } else if (day.degree[0] >= "122" && day.degree[0] < "157") {
        SetConsoleTextAttribute(cons, 15);
        std::cout << "ŞÂ ";
        SetConsoleTextAttribute(cons, wind_color);
        std::cout << day.wind[0];
        SetConsoleTextAttribute(cons, 15);
        std::cout << " êì/÷\n";
    } else if (day.degree[0] >= "157" && day.degree[0] < "202") {
        SetConsoleTextAttribute(cons, 15);
        std::cout << "Ş ";
        SetConsoleTextAttribute(cons, wind_color);
        std::cout << day.wind[0];
        SetConsoleTextAttribute(cons, 15);
        std::cout << " êì/÷\n";
    } else if (day.degree[0] >= "202" && day.degree[0] < "247") {
        SetConsoleTextAttribute(cons, 15);
        std::cout << "ŞÇ ";
        SetConsoleTextAttribute(cons, wind_color);
        std::cout << day.wind[0];
        SetConsoleTextAttribute(cons, 15);
        std::cout << " êì/÷\n";
    } else if (day.degree[0] >= "247" && day.degree[0] < "292") {
        SetConsoleTextAttribute(cons, 15);
        std::cout << "Ç ";
        SetConsoleTextAttribute(cons, wind_color);
        std::cout << day.wind[0];
        SetConsoleTextAttribute(cons, 15);
        std::cout << " êì/÷\n";
    } else if (day.degree[0] >= "292" && day.degree[0] < "337") {
        SetConsoleTextAttribute(cons, 15);
        std::cout << "ÑÇ ";
        SetConsoleTextAttribute(cons, wind_color);
        std::cout << day.wind[0];
        SetConsoleTextAttribute(cons, 15);
        std::cout << " êì/÷\n";
    }
    weather_out(weath, day.weather_code[0].second, 45, cons);
    std::cout << day.prob[0] << " ìì" << "\n";
    weather_out(weath, day.weather_code[0].second, 60, cons);
    std::cout << day.humidity[0] << " %" << "\n";

    for (int i = 1; i < day_count + 1; i++) {
        Day day_cur = getDay(i);
        std::cout
                << "                                                       ———————————————                                                       \n";
        std::cout
                << "———————————————————————————————————————————————————————| " + day_cur.date +
                   "  |———————————————————————————————————————————————————————\n";
        std::cout
                << "|             Óòğî             |             Äåíü      ———————————————       Âå÷åğ           |             Íî÷ü             |\n";
        std::cout
                << "——————————————————————————————————————————————————————————————|——————————————————————————————————————————————————————————————\n";
        std::string weather_cur[4];
        for (int j = 0; j < 4; j++) {
            weather_cur[j] = weather.weather[day_cur.weather_code[j].second];
        }
        std::cout << "|";
        for (int j = 0; j < 4; j++) {
            weather_out(weather_cur[j], day_cur.weather_code[j].second, 0, cons);
            std::cout << day_cur.weather_code[j].first << "|";
        }
        std::cout << "\n|";
        for (int j = 0; j < 4; j++) {
            weather_out(weather_cur[j], day_cur.weather_code[j].second, 15, cons);
            std::string temp = day_cur.temperature[j] + " °C";
            int size = 15 - temp.size();
            for (int k = 0; k < size; k++)
                temp += " ";
            std::cout << temp << "|";
        }
        std::cout << "\n|";
        for (int j = 0; j < 4; j++) {
            if (stoi(day_cur.wind[j]) < 10)
                wind_color = 10;
            else if (stoi(day_cur.wind[j]) >= 10 && stoi(day_cur.wind[j]) < 20)
                wind_color = 14;
            else if (stoi(day_cur.wind[j]) >= 20)
                wind_color = 12;

            int counter = 0;
            weather_out(weather_cur[j], day_cur.weather_code[j].second, 30, cons);
            if (day_cur.degree[j] < "22" || day_cur.degree[j] >= "337") {
                SetConsoleTextAttribute(cons, 15);
                std::cout << "Ñ ";
                counter += 2;
                SetConsoleTextAttribute(cons, wind_color);
                std::cout << day_cur.wind[j];
                counter += day_cur.wind[j].size();
                SetConsoleTextAttribute(cons, 15);
                std::cout << " êì/÷";
            } else if (day_cur.degree[j] >= "22" && day_cur.degree[j] < "67") {
                SetConsoleTextAttribute(cons, 15);
                std::cout << "ÑÂ ";
                counter += 3;
                SetConsoleTextAttribute(cons, wind_color);
                std::cout << day_cur.wind[j];
                counter += day_cur.wind[j].size();
                SetConsoleTextAttribute(cons, 15);
                std::cout << " êì/÷";
            } else if (day_cur.degree[j] >= "67" && day_cur.degree[j] < "122") {
                SetConsoleTextAttribute(cons, 15);
                std::cout << "Â ";
                counter += 2;
                SetConsoleTextAttribute(cons, wind_color);
                std::cout << day_cur.wind[j];
                counter += day_cur.wind[j].size();
                SetConsoleTextAttribute(cons, 15);
                std::cout << " êì/÷";
                SetConsoleTextAttribute(cons, 15);
            } else if (day_cur.degree[j] >= "122" && day_cur.degree[j] < "157") {
                SetConsoleTextAttribute(cons, 15);
                std::cout << "ŞÂ ";
                counter += 3;
                SetConsoleTextAttribute(cons, wind_color);
                std::cout << day_cur.wind[j];
                counter += day_cur.wind[j].size();
                SetConsoleTextAttribute(cons, 15);
                std::cout << " êì/÷";
            } else if (day_cur.degree[j] >= "157" && day_cur.degree[j] < "202") {
                SetConsoleTextAttribute(cons, 15);
                std::cout << "Ş ";
                counter += 2;
                SetConsoleTextAttribute(cons, wind_color);
                std::cout << day_cur.wind[j];
                counter += day_cur.wind[j].size();
                SetConsoleTextAttribute(cons, 15);
                std::cout << " êì/÷";
            } else if (day_cur.degree[j] >= "202" && day_cur.degree[j] < "247") {
                SetConsoleTextAttribute(cons, 15);
                std::cout << "ŞÇ ";
                counter += 3;
                SetConsoleTextAttribute(cons, wind_color);
                std::cout << day_cur.wind[j];
                counter += day_cur.wind[j].size();
                SetConsoleTextAttribute(cons, 15);
                std::cout << " êì/÷";
            } else if (day_cur.degree[j] >= "247" && day_cur.degree[j] < "292") {
                SetConsoleTextAttribute(cons, 15);
                std::cout << "Ç ";
                counter += 2;
                SetConsoleTextAttribute(cons, wind_color);
                std::cout << day_cur.wind[j];
                counter += day_cur.wind[j].size();
                SetConsoleTextAttribute(cons, 15);
                std::cout << " êì/÷";
            } else if (day_cur.degree[j] >= "292" && day_cur.degree[j] < "337") {
                SetConsoleTextAttribute(cons, 15);
                std::cout << "ÑÇ ";
                counter += 3;
                SetConsoleTextAttribute(cons, wind_color);
                std::cout << day_cur.wind[j];
                counter += day_cur.wind[j].size();
                SetConsoleTextAttribute(cons, 15);
                std::cout << " êì/÷";
            }
            for (int k = 0; k < 15 - 5 - counter; k++)
                std::cout << " ";
            std::cout << "|";
        }
        std::cout << "\n|";
        for (int j = 0; j < 4; j++) {
            weather_out(weather_cur[j], day_cur.weather_code[j].second, 45, cons);
            std::cout << day_cur.prob[j] << " ìì";
            for (int k = 0; k < 15 - day_cur.prob[j].size() - 3; k++)
                std::cout << " ";
            std::cout << "|";
        }
        std::cout << "\n|";

        for (int j = 0; j < 4; j++) {
            weather_out(weather_cur[j], day_cur.weather_code[j].second, 60, cons);
            std::cout << day_cur.humidity[j] << " %";
            for (int k = 0; k < 15 - day_cur.humidity[j].size() - 2; k++)
                std::cout << " ";
            std::cout << "|";
        }
        std::cout
                << "\n———————————————————————————————————————————————————————————————"
                   "——————————————————————————————————————————————————————————————\n";
    }
}

void City::Destroy() {
    for (int i = 0; i < 17; i++) {
        delete[] days[i].temperature;
        delete[] days[i].degree;
        delete[] days[i].humidity;
        delete[] days[i].prob;
        delete[] days[i].wind;
        delete[] days[i].weather_code;
    }
    delete[] days;
}