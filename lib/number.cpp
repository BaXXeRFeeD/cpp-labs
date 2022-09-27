#include "number.h"

uint2022_t from_uint(uint32_t i) {
    uint2022_t uint2022_t;
    for (int t = 0; t < 68; t++)
        uint2022_t.digits[t] = 0;
    if (i == 0) {
        uint2022_t.digits[0] = 1000000000;
        uint2022_t.countdigits = 1;
    } else {
        int j = 0;
        while (i > 0) {
            uint2022_t.digits[j] = i % 1000000000 + 1000000000;
            j++;
            i /= 1000000000;
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
        uint2022_t.digits[count] = stoi(buffstring.substr(i - 8, 9)) + 1000000000;
        count++;
    }
    uint2022_t.digits[count] =
            stoi(buffstring.substr(0, buffstring.size() - 1 - (uint2022_t.countdigits - 1) * 9 + 1)) + 1000000000;
    return uint2022_t;
}

uint2022_t operator+(const uint2022_t &hs1, const uint2022_t &hs2) {
    uint2022_t uint2022_t;
    struct uint2022_t lhs = hs1;
    struct uint2022_t rhs = hs2;
    for (int i = 0; i < 68; i++)
        uint2022_t.digits[i] = 0;
    for (int i = fmin(lhs.countdigits, rhs.countdigits); i < fmax(lhs.countdigits, rhs.countdigits); i++) {
        if (lhs.countdigits > rhs.countdigits)
            rhs.digits[i] = 1000000000;
        else
            lhs.digits[i] = 1000000000;
    }
    for (int i = 0; i < fmax(lhs.countdigits, rhs.countdigits); i++) {
        uint2022_t.digits[i] = lhs.digits[i] - 1000000000 + rhs.digits[i] - 1000000000 + uint2022_t.zapas;
        if (uint2022_t.digits[i] >= 1000000000)
            uint2022_t.zapas = 1;
        else {
            uint2022_t.digits[i] += 1000000000;
            uint2022_t.zapas = 0;
        }
    }
    if (uint2022_t.zapas) {
        uint2022_t.digits[fmax(lhs.countdigits, rhs.countdigits)] += uint2022_t.zapas;
        uint2022_t.zapas = 0;
        uint2022_t.countdigits = fmax(lhs.countdigits, rhs.countdigits) + 1;
    } else
        uint2022_t.countdigits = fmax(lhs.countdigits, rhs.countdigits);
    return uint2022_t;
}

uint2022_t operator-(const uint2022_t &lhs, const uint2022_t &rhs) {
    uint2022_t uint2022_t;
    if (lhs == rhs) {
        uint2022_t.digits[0] = 1000000000;
        uint2022_t.countdigits = 1;
    } else {
        struct uint2022_t lhs1 = lhs;
        struct uint2022_t rhs1 = rhs;
        uint2022_t.countdigits = lhs1.countdigits;
        for (int i = 0; i < 68; i++)
            uint2022_t.digits[i] = 0;
        for (int i = rhs1.countdigits; i < lhs1.countdigits; i++)
            rhs1.digits[i] = 1000000000;
        for (int i = 0; i < lhs1.countdigits; i++) {
            if (lhs1.digits[i] >= rhs1.digits[i] + uint2022_t.zapas) {
                uint2022_t.digits[i] = (lhs1.digits[i] - uint2022_t.zapas - rhs1.digits[i]) + 1000000000;
                uint2022_t.zapas = 0;
            } else {
                uint2022_t.digits[i] = 2000000000 - rhs1.digits[i] - uint2022_t.zapas + lhs1.digits[i];
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
    int count = 0;
    if ((lhs.digits[0] == 1000000000 && lhs.countdigits == 1) ||
        (rhs.digits[0] == 1000000000 && rhs.countdigits == 1)) {
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
                mult = (uint64_t) (lhs.digits[i] - 1000000000) * (uint64_t) (rhs.digits[j] - 1000000000);
                uint2022_t.digits[i + j] += mult % 1000000000;
                if (uint2022_t.digits[i + j] >= 1000000000) {
                    if (uint2022_t.digits[i + j + 1] == 0)
                        count++;
                    uint2022_t.digits[i + j + 1]++;
                    uint2022_t.digits[i + j] -= 1000000000;
                }
                mult /= 1000000000;
                if (mult != 0) {
                    if (uint2022_t.digits[i + j + 1] == 0)
                        count++;
                    uint2022_t.digits[i + j + 1] += mult % 1000000000;
                    if (uint2022_t.digits[i + j + 1] >= 1000000000) {
                        if (uint2022_t.digits[i + j + 2] == 0)
                            count++;
                        uint2022_t.digits[i + j + 2]++;
                        uint2022_t.digits[i + j + 1] -= 1000000000;
                    }
                }
            }
        }
    }
    for (int i = 0; i < count; i++)
        uint2022_t.digits[i] += 1000000000;
    uint2022_t.countdigits = count;
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
    if (lhs.countdigits != rhs.countdigits)
        return true;
    for (int i = 0; i < lhs.countdigits; i++)
        if (lhs.digits[i] != rhs.digits[i])
            return true;
    return false;
}

std::ostream &operator<<(std::ostream &stream, const uint2022_t &value) {
    if (value.str) {
        stream << "Undefined Behavior";
    } else {
        struct uint2022_t proverka = from_string(
                "48156091677115868480078692270323562563127432271414226341441788416392587332230643768902423100952675139440175832691636710"
                "6052034484602375642882110959089521812209947069992139877256008949136579813164413834190131240610432508865633901300457687591589"
                "6321903255827106838867819739516957333842785448961317408670542466925730316291502478820826826477731689044263368148553678106934"
                "6754746178079707116356715945292806889290699278717813583995934722350764724084592467095871617327975075134165154129579253728839"
                "3481542519773223140547524361834615428274169543954961376881442030303829940191406452725012875774576546969913778507874304");
        proverka = proverka - value;
        if(proverka.str)
            stream << "Undefined Behavior";
            else {
                stream << value.digits[value.countdigits - 1] % 1000000000;
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
