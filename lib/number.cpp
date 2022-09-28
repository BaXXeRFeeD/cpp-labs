#include "number.h"

uint2022_t from_uint(uint32_t i) {
    uint2022_t uint2022_t;
    for (int t = 0; t < 68; t++)
        uint2022_t.digits[t] = 0;
    if (i == 0) {
        uint2022_t.digits[0] = miliard;
        uint2022_t.countdigits = 1;
    } else {
        int j = 0;
        while (i > 0) {
            uint2022_t.digits[j] = i % miliard + miliard;
            j++;
            i /= miliard;
        }
        uint2022_t.countdigits = j;
    }
    return uint2022_t;
}

uint2022_t from_string(const char *buff) {
    uint2022_t uint2022_t;
    for (int i = 0; i < 68; i++)
        uint2022_t.digits[i] = 0;
    int count = 0;
    std::string buffstring = buff;
    uint2022_t.countdigits = buffstring.length() / 9;
    if (buffstring.size() % 9 != 0)
        uint2022_t.countdigits++;
    for (int i = buffstring.size() - 1; i > buffstring.size() - 1 - (uint2022_t.countdigits - 1) * 9; i -= 9) {
        uint2022_t.digits[count] = stoi(buffstring.substr(i - 8, 9)) + miliard;
        count++;
    }
    uint2022_t.digits[count] =
            stoi(buffstring.substr(0, buffstring.size() - 1 - (uint2022_t.countdigits - 1) * 9 + 1)) + miliard;
    return uint2022_t;
}

uint2022_t operator+(const uint2022_t &hs1, const uint2022_t &hs2) {
    uint2022_t uint2022_t;
    uint2022_t.str = hs1.str + hs2.str;
    struct uint2022_t lhs = hs1;
    struct uint2022_t rhs = hs2;
    for (int i = 0; i < 68; i++)
        uint2022_t.digits[i] = 0;
    for (int i = fmin(lhs.countdigits, rhs.countdigits); i < fmax(lhs.countdigits, rhs.countdigits); i++) {
        if (lhs.countdigits > rhs.countdigits)
            rhs.digits[i] = miliard;
        else
            lhs.digits[i] = miliard;
    }
    for (int i = 0; i < fmax(lhs.countdigits, rhs.countdigits); i++) {
        uint2022_t.digits[i] = lhs.digits[i] - miliard + rhs.digits[i] - miliard + uint2022_t.zapas;
        if (uint2022_t.digits[i] >= miliard)
            uint2022_t.zapas = 1;
        else {
            uint2022_t.digits[i] += miliard;
            uint2022_t.zapas = 0;
        }
    }
    if (uint2022_t.zapas) {
        uint2022_t.digits[fmax(lhs.countdigits, rhs.countdigits)] += uint2022_t.zapas;
        uint2022_t.zapas = 0;
        uint2022_t.countdigits = fmax(lhs.countdigits, rhs.countdigits) + 1;
    } else
        uint2022_t.countdigits = fmax(lhs.countdigits, rhs.countdigits);
    if (!lhs.flag && !rhs.flag) {
        struct uint2022_t checker;
        uint2022_t.flag = 1;
        checker = from_string(MAXIMUM);
        checker = checker - uint2022_t;
        if (checker.str)
            uint2022_t.str = 1;
        uint2022_t.flag = 0;
    }
    return uint2022_t;
}

uint2022_t operator-(const uint2022_t &lhs, const uint2022_t &rhs) {
    uint2022_t uint2022_t;
    uint2022_t.str = lhs.str + rhs.str;
    if (lhs == rhs) {
        uint2022_t.digits[0] = miliard;
        uint2022_t.countdigits = 1;
    } else {
        struct uint2022_t lhs1 = lhs;
        struct uint2022_t rhs1 = rhs;
        uint2022_t.countdigits = lhs1.countdigits;
        for (int i = 0; i < 68; i++)
            uint2022_t.digits[i] = 0;
        for (int i = rhs1.countdigits; i < lhs1.countdigits; i++)
            rhs1.digits[i] = miliard;
        for (int i = 0; i < lhs1.countdigits; i++) {
            if (lhs1.digits[i] >= rhs1.digits[i] + uint2022_t.zapas) {
                uint2022_t.digits[i] = (lhs1.digits[i] - uint2022_t.zapas - rhs1.digits[i]) + miliard;
                uint2022_t.zapas = 0;
            } else {
                uint2022_t.digits[i] = 2 * miliard - rhs1.digits[i] - uint2022_t.zapas + lhs1.digits[i];
                uint2022_t.zapas = 1;
            }
        }
        if (uint2022_t + rhs1 != lhs1)
            uint2022_t.str = 1;
    }
    return uint2022_t;
}

uint2022_t operator*(const uint2022_t &lhs, const uint2022_t &rhs) {
    uint2022_t uint2022_t;
    uint2022_t.str = lhs.str + rhs.str;
    int count = 0;
    if ((lhs.digits[0] == miliard && lhs.countdigits == 1) ||
        (rhs.digits[0] == miliard && rhs.countdigits == 1)) {
        uint2022_t.digits[0] = 0;
        count = 1;
    } else {
        for (int i = 0; i < 68; i++)
            uint2022_t.digits[i] = 0;
        uint64_t mult;
        for (int i = 0; i < lhs.countdigits; i++) {
            for (int j = 0; j < rhs.countdigits; j++) {
                if (uint2022_t.digits[i + j] == 0)
                    count++;
                mult = (uint64_t)(lhs.digits[i] - miliard) * (uint64_t)(rhs.digits[j] - miliard);
                uint2022_t.digits[i + j] += mult % miliard;
                if (uint2022_t.digits[i + j] >= miliard) {
                    if (uint2022_t.digits[i + j + 1] == 0)
                        count++;
                    uint2022_t.digits[i + j + 1]++;
                    uint2022_t.digits[i + j] -= miliard;
                }
                mult /= miliard;
                if (mult != 0) {
                    if (uint2022_t.digits[i + j + 1] == 0)
                        count++;
                    uint2022_t.digits[i + j + 1] += mult % miliard;
                    if (uint2022_t.digits[i + j + 1] >= miliard) {
                        if (uint2022_t.digits[i + j + 2] == 0)
                            count++;
                        uint2022_t.digits[i + j + 2]++;
                        uint2022_t.digits[i + j + 1] -= miliard;
                    }
                }
            }
        }
    }
    for (int i = 0; i < count; i++)
        uint2022_t.digits[i] += miliard;
    uint2022_t.countdigits = count;
    if (!lhs.flag && !rhs.flag) {
        struct uint2022_t checker;
        uint2022_t.flag = 1;
        checker = from_string(MAXIMUM);
        checker = checker - uint2022_t;
        if (checker.str)
            uint2022_t.str = 1;
        uint2022_t.flag = 0;
    }
    return uint2022_t;
}

uint2022_t operator/(const uint2022_t &lhs, const uint2022_t &rhs) {
    return uint2022_t();
}

bool operator==(const uint2022_t &lhs, const uint2022_t &rhs) {
    if (lhs.countdigits != rhs.countdigits)
        return false;
    for (int i = 0; i < lhs.countdigits; i++)
        if (lhs.digits[i] != rhs.digits[i])
            return false;
    return true;
}

bool operator!=(const uint2022_t &lhs, const uint2022_t &rhs) {
    if (lhs == rhs)
        return false;
    return true;
}

std::ostream &operator<<(std::ostream &stream, uint2022_t value) {
    if (value.str)
        stream << "Undefined Behavior";
    else {
        uint2022_t checker = from_string(MAXIMUM);
        value.flag = 1;
        checker = checker - value;
        value.flag = 0;
        if (checker.str)
            stream << "Undefined Behavior";
        else {
            stream << value.digits[value.countdigits - 1] % miliard;
            for (int i = value.countdigits - 2; i >= 0; i--) {
                std::string stringvalue = std::to_string(value.digits[i]);
                for (int j = 1; j < 10; j++) {
                    stream << stringvalue[j];
                }
            }
        }
    }
    return stream;
}
