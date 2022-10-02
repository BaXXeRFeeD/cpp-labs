#include "number.h"

uint2022_t from_uint(uint32_t i) {
    uint2022_t uint2022_t;
    for (int t = 0; t < digitssize; t++) {
        uint2022_t.digits[t] = 0;
    }
    if (i == 0) {
        uint2022_t.countdigits = 1;
    } else {
        int j = 0;
        while (i > 0) {
            uint2022_t.digits[j] = i % additionaldigit;
            j++;
            i /= additionaldigit;
        }
        uint2022_t.countdigits = j;
    }
    return uint2022_t;
}

uint2022_t from_string(const char *buff) {
    uint2022_t uint2022;
    for (int i = 0; i < digitssize; i++) {
        uint2022.digits[i] = 0;
    }
    int count = 0;
    std::string buffstring = buff;
    uint2022.countdigits = buffstring.length() / 9;
    for (int i = 0; i < buffstring.size(); i++) {
        if (buffstring[i] < '0' || buffstring[i] > '9') {
            uint2022.str = 0;
            break;
        }
    }
    if (uint2022.str) {
        if (buffstring.length() > MAXIMUMSIZE) {
            uint2022.str = 0;
        } else {
            if (buffstring.size() % 9 != 0) {
                uint2022.countdigits++;
            }
            for (int i = buffstring.size() - 1; i > buffstring.size() - 1 - (uint2022.countdigits - 1) * 9; i -= 9) {
                uint2022.digits[count] = stoi(buffstring.substr(i - 8, 9));
                count++;
            }
            uint2022.digits[count] =
                    stoi(buffstring.substr(0, buffstring.size() - 1 - (uint2022.countdigits - 1) * 9 + 1));
            bool tr = false;
            for (int i = 0; i < buffstring.size() && uint2022.str; i++) {
                if (buffstring[i] != MAXIMUM[i]) {
                    tr = true;
                    break;
                }
            }
            if (tr && uint2022.str) {
                uint2022_t checker = from_string(MAXIMUM);
                uint2022.flag = 1;
                checker = checker - uint2022;
                uint2022.flag = 0;
                uint2022.str = checker.str;
            }
        }
    }
    return uint2022;
}

uint2022_t operator+(const uint2022_t &hs1, const uint2022_t &hs2) {
    uint2022_t uint2022;
    uint2022.str = hs1.str * hs2.str;
    if (uint2022.str) {
        uint2022_t lhs = hs1;
        uint2022_t rhs = hs2;
        for (int i = 0; i < digitssize; i++) {
            uint2022.digits[i] = 0;
        }
        for (int i = 0; i < fmax(lhs.countdigits, rhs.countdigits); i++) {
            uint2022.digits[i] =
                    lhs.digits[i] + rhs.digits[i] + uint2022.digits[i];
            if (uint2022.digits[i] >= additionaldigit) {
                uint2022.digits[i + 1] = 1;
                uint2022.digits[i] -= additionaldigit;
            }
        }
        if (uint2022.digits[fmax(lhs.countdigits, rhs.countdigits)] == 0) {
            uint2022.countdigits = fmax(lhs.countdigits, rhs.countdigits);
        } else {
            uint2022.countdigits = fmax(lhs.countdigits, rhs.countdigits) + 1;
        }
        if (!lhs.flag && !rhs.flag) {
            uint2022_t checker;
            uint2022.flag = 1;
            checker = from_string(MAXIMUM);
            checker = checker - uint2022;
            uint2022.str = checker.str;
            uint2022.flag = 0;
        }
    }
    return uint2022;
}

uint2022_t operator-(const uint2022_t &lhs, const uint2022_t &rhs) {
    uint2022_t uint2022;
    uint2022.str = lhs.str * rhs.str;
    if (uint2022.str) {
        if (lhs == rhs) {
            uint2022.digits[0] = 0;
            uint2022.countdigits = 1;
        } else {
            uint2022_t lhs1 = lhs;
            uint2022_t rhs1 = rhs;
            uint2022.countdigits = lhs1.countdigits;
            for (int i = 0; i < digitssize; i++) {
                uint2022.digits[i] = 0;
            }
            for (int i = 0; i < lhs1.countdigits; i++) {
                if (lhs1.digits[i] >= rhs1.digits[i] + uint2022.digits[i]) {
                    uint2022.digits[i] =
                            (lhs1.digits[i] - uint2022.digits[i] - rhs1.digits[i]) ;
                } else {
                    uint2022.digits[i] =
                            additionaldigit - rhs1.digits[i] - uint2022.digits[i] + lhs1.digits[i];
                    uint2022.digits[i + 1] = 1;
                }
            }
            if (uint2022 + rhs1 != lhs1) {
                uint2022.str = 0;
            }
        }
    }
    return uint2022;
}

uint2022_t operator*(const uint2022_t &lhs, const uint2022_t &rhs) {
    uint2022_t uint2022;
    uint2022.str = lhs.str * rhs.str;
    if (uint2022.str) {
        int count = 0;
        if ((lhs.digits[0] == 0 && lhs.countdigits == 1) ||
            (rhs.digits[0] == 0 && rhs.countdigits == 1)) {
            uint2022.digits[0] = 0;
            count = 1;
        } else {
            for (int i = 0; i < digitssize; i++) {
                uint2022.digits[i] = -1;
            }
            uint64_t mult;
            for (int i = 0; i < lhs.countdigits && uint2022.str; i++) {
                for (int j = 0; j < rhs.countdigits; j++) {
                    if (uint2022.digits[digitssize - 1] != -1) {
                        uint2022.str = 0;
                        break;
                    }
                    if (uint2022.digits[i + j] == -1) {
                        uint2022.digits[i + j]++;
                        count++;
                    }
                    mult = (uint64_t) lhs.digits[i] * (uint64_t) rhs.digits[j];
                    uint2022.digits[i + j] += mult % additionaldigit;
                    if (uint2022.digits[i + j] >= additionaldigit) {
                        if (uint2022.digits[i + j + 1] == -1) {
                            uint2022.digits[i + j + 1]++;
                            count++;
                        }
                        uint2022.digits[i + j + 1]++;
                        uint2022.digits[i + j] -= additionaldigit;
                    }
                    mult /= additionaldigit;
                    if (mult != 0) {
                        if (uint2022.digits[i + j + 1] == -1) {
                            uint2022.digits[i + j + 1]++;
                            count++;
                        }
                        uint2022.digits[i + j + 1] += mult % additionaldigit;
                        if (uint2022.digits[i + j + 1] >= additionaldigit) {
                            if (uint2022.digits[i + j + 2] == -1) {
                                uint2022.digits[i + j + 2]++;
                                count++;
                            }
                            uint2022.digits[i + j + 2]++;
                            uint2022.digits[i + j + 1] -= additionaldigit;
                        }
                    }
                }
            }
        }
        if (uint2022.str) {
            uint2022.countdigits = count;
            if (!lhs.flag && !rhs.flag) {
                uint2022_t checker;
                uint2022.flag = 1;
                checker = from_string(MAXIMUM);
                checker = checker - uint2022;
                uint2022.str = checker.str;
                uint2022.flag = 0;
            }
        }
    }
    return uint2022;
}

uint2022_t operator/(const uint2022_t &lhs, const uint2022_t &rhs) {
    return uint2022_t();
}

bool operator==(const uint2022_t &lhs, const uint2022_t &rhs) {
    if (lhs.str && rhs.str) {
        if (lhs.countdigits != rhs.countdigits) {
            return false;
        }
        for (int i = 0; i < lhs.countdigits; i++) {
            if (lhs.digits[i] != rhs.digits[i]) {
                return false;
            }
        }
        return true;
    }
    std::cout << "Undefined Behavior\n";
    return false;
}

bool operator!=(const uint2022_t &lhs, const uint2022_t &rhs) {
    if (!lhs.str || !rhs.str) {
        std::cout << "Undefined Behavior\n";
        return false;
    }
    return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &stream, uint2022_t value) {
    if (value.str) {
        for(int i = 0; i < value.countdigits; i++){
            value.digits[i] += additionaldigit;
        }
        stream << value.digits[value.countdigits - 1] % additionaldigit;
        for (int i = value.countdigits - 2; i >= 0; i--) {
            std::string stringvalue = std::to_string(value.digits[i]);
            for (int j = 1; j < 10; j++) {
                stream << stringvalue[j];
            }
        }
        return stream;
    }
    stream << "Undefined Behavior\n";
    return stream;
}