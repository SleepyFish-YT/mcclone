//
// Created by SleepyFish on 03.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_MATHHELPER_H
#define MCCLONE_MATHHELPER_H

#include "Vec3i.h"
#include "MathUtils.h"

#include <array>
#include <cmath>
#include <numbers>
#include <random>
#include <string>
#include <span>
#include <bit>
#include <stdexcept>

/**
 * @author SleepyFish
 */
class MathHelper {

private:

    static inline const float radToIndex     = MathUtils::roundToFloat(651.8986469044033);

    static inline const double field_181163_d = std::bit_cast<double>(4805340802404319232LL);

    static inline const std::array<int,    32>    multiplyDeBruijnBitPosition = {
            0,1,28,2,29,14,24,3,30,22,20,15,25,17,4,8,31,27,13,23,21,19,16,7,26,12,18,6,11,5,10,9
    };

    static std::array<float, 65536> buildSinTable() {
        std::array<float, 65536> table{};
        for (int i = 0; i < 65536; ++i) {
            table[i] = static_cast<float>(std::sin(static_cast<double>(i) * std::numbers::pi * 2.0 / 65536.0));
        }
        return table;
    }

    static std::array<float, 4096> buildSinTableFast() {
        std::array<float, 4096> table{};
        for (int i = 0; i < 4096; ++i) {
            table[i] = MathUtils::roundToFloat(std::sin(static_cast<double>(i) * std::numbers::pi * 2.0 / 4096.0));
        }
        return table;
    }

    static std::array<double, 257> buildField181164e() {
        std::array<double, 257> table{};
        for (int i = 0; i < 257; ++i) {
            const double d0 = static_cast<double>(i) / 256.0;
            table[i] = std::asin(d0);
        }
        return table;
    }

    static std::array<double, 257> buildField181165f() {
        std::array<double, 257> table{};
        for (int i = 0; i < 257; ++i) {
            const double d0 = static_cast<double>(i) / 256.0;
            table[i] = std::cos(std::asin(d0));
        }
        return table;
    }

    static bool isPowerOfTwo(int value) {
        return value != 0 && (value & (value - 1)) == 0;
    }

    static int calculateLogBaseTwoDeBruijn(int value) {
        value = isPowerOfTwo(value) ? value : roundUpToPowerOfTwo(value);
        return multiplyDeBruijnBitPosition[static_cast<int>(static_cast<long long>(value) * 125613361LL >> 27) & 31];
    }

    static double func_181161_i(double value) {
        const double d0 = 0.5 * value;
        long long i = std::bit_cast<long long>(value);
        i = 6910469410427058090LL - (i >> 1);
        value = std::bit_cast<double>(i);
        value = value * (1.5 - d0 * value * value);
        return value;
    }

    static inline const std::array<float,  65536> SIN_TABLE       = buildSinTable();
    static inline const std::array<float,  4096>  SIN_TABLE_FAST  = buildSinTableFast();
    static inline const std::array<double, 257>   field_181164_e  = buildField181164e();
    static inline const std::array<double, 257>   field_181165_f  = buildField181165f();

public:

    static inline bool fastMath = false;

    static inline const float PI      = MathUtils::roundToFloat(std::numbers::pi);
    static inline const float PId2    = MathUtils::roundToFloat(std::numbers::pi / 2.0);
    static inline const float deg2Rad = MathUtils::roundToFloat(0.017453292519943295);

    static float sin(float value) {
        return fastMath
               ? SIN_TABLE_FAST[static_cast<int>(value * radToIndex) & 4095]
               : SIN_TABLE[static_cast<int>(value * 10430.378f) & 65535];
    }

    static float cos(float value) {
        return fastMath
               ? SIN_TABLE_FAST[static_cast<int>(value * radToIndex + 1024.0f) & 4095]
               : SIN_TABLE[static_cast<int>(value * 10430.378f + 16384.0f) & 65535];
    }

    static float sqrt_float(float value) {
        return static_cast<float>(std::sqrt(value));
    }

    static float sqrt_double(double value) {
        return static_cast<float>(std::sqrt(value));
    }

    static int floor_float(float value) {
        const int i = static_cast<int>(value);
        return value < static_cast<float>(i) ? i - 1 : i;
    }

    static int truncateDoubleToInt(double value) {
        return static_cast<int>(value + 1024.0) - 1024;
    }

    static int floor_double(double value) {
        const int i = static_cast<int>(value);
        return value < static_cast<double>(i) ? i - 1 : i;
    }

    static long long floor_double_long(double value) {
        const long long i = static_cast<long long>(value);
        return value < static_cast<double>(i) ? i - 1LL : i;
    }

    static int func_154353_e(double value) {
        return static_cast<int>(value >= 0.0 ? value : -value + 1.0);
    }

    static float abs(float value) {
        return value >= 0.0f ? value : -value;
    }

    static int abs_int(int value) {
        return value >= 0 ? value : -value;
    }

    static int ceiling_float_int(float value) {
        const int i = static_cast<int>(value);
        return value > static_cast<float>(i) ? i + 1 : i;
    }

    static int ceiling_double_int(double value) {
        const int i = static_cast<int>(value);
        return value > static_cast<double>(i) ? i + 1 : i;
    }

    static int clamp_int(int num, int min, int max) {
        return num < min ? min : (num > max ? max : num);
    }

    static float clamp_float(float num, float min, float max) {
        return num < min ? min : (num > max ? max : num);
    }

    static double clamp_double(double num, double min, double max) {
        return num < min ? min : (num > max ? max : num);
    }

    static double denormalizeClamp(double lowerBnd, double upperBnd, double slide) {
        return slide < 0.0 ? lowerBnd : (slide > 1.0 ? upperBnd : lowerBnd + (upperBnd - lowerBnd) * slide);
    }

    static double abs_max(double a, double b) {
        if (a < 0.0) a = -a;
        if (b < 0.0) b = -b;
        return a > b ? a : b;
    }

    static int bucketInt(int a, int b) {
        return a < 0 ? -((-a - 1) / b) - 1 : a / b;
    }

    static int getRandomIntegerInRange(std::mt19937& rand, int min, int max) {
        return min >= max ? min : std::uniform_int_distribution<int>(min, max)(rand);
    }

    static float randomFloatClamp(std::mt19937& rand, float min, float max) {
        return min >= max ? min : std::uniform_real_distribution<float>(min, max)(rand);
    }

    static double getRandomDoubleInRange(std::mt19937& rand, double min, double max) {
        return min >= max ? min : std::uniform_real_distribution<double>(min, max)(rand);
    }

    static double average(std::span<const long long> values) {
        long long sum = 0LL;
        for (const long long v : values) sum += v;
        return static_cast<double>(sum) / static_cast<double>(values.size());
    }

    static bool epsilonEquals(float a, float b) {
        return abs(b - a) < 1.0E-5f;
    }

    static int normalizeAngle(int value, int mod) {
        return (value % mod + mod) % mod;
    }

    static float wrapAngleTo180_float(float value) {
        value = std::fmod(value, 360.0f);
        if (value >= 180.0f)  value -= 360.0f;
        if (value < -180.0f) value += 360.0f;
        return value;
    }

    static double wrapAngleTo180_double(double value) {
        value = std::fmod(value, 360.0);
        if (value >= 180.0)  value -= 360.0;
        if (value < -180.0) value += 360.0;
        return value;
    }

    static int parseIntWithDefault(const std::string& str, int defaultVal) {
        try { return std::stoi(str); }
        catch (...) { return defaultVal; }
    }

    static int parseIntWithDefaultAndMax(const std::string& str, int defaultVal, int max) {
        return std::max(max, parseIntWithDefault(str, defaultVal));
    }

    static double parseDoubleWithDefault(const std::string& str, double defaultVal) {
        try { return std::stod(str); }
        catch (...) { return defaultVal; }
    }

    static double parseDoubleWithDefaultAndMax(const std::string& str, double defaultVal, double max) {
        return std::max(max, parseDoubleWithDefault(str, defaultVal));
    }

    static int roundUpToPowerOfTwo(int value) {
        int i = value - 1;
        i |= i >> 1;
        i |= i >> 2;
        i |= i >> 4;
        i |= i >> 8;
        i |= i >> 16;
        return i + 1;
    }

    static int calculateLogBaseTwo(int value) {
        return calculateLogBaseTwoDeBruijn(value) - (isPowerOfTwo(value) ? 0 : 1);
    }

    static int roundUp(int value, int step) {
        if (step == 0) return 0;
        if (value == 0) return step;
        if (value < 0) step *= -1;
        const int i = value % step;
        return i == 0 ? value : value + step - i;
    }

    static int func_180183_b(float r, float g, float b) {
        return func_180181_b(floor_float(r * 255.0f), floor_float(g * 255.0f), floor_float(b * 255.0f));
    }

    static int func_180181_b(int r, int g, int b) {
        return (r << 16) | (g << 8) | b;
    }

    static int func_180188_d(int a, int b) {
        const int i  = (a & 0xFF0000) >> 16;
        const int j  = (b & 0xFF0000) >> 16;
        const int k  = (a & 0xFF00) >> 8;
        const int l  = (b & 0xFF00) >> 8;
        const int i1 = (a & 0xFF);
        const int j1 = (b & 0xFF);
        const int k1 = static_cast<int>(static_cast<float>(i) * static_cast<float>(j) / 255.0f);
        const int l1 = static_cast<int>(static_cast<float>(k) * static_cast<float>(l) / 255.0f);
        const int i2 = static_cast<int>(static_cast<float>(i1) * static_cast<float>(j1) / 255.0f);
        return (a & 0xFF000000) | (k1 << 16) | (l1 << 8) | i2;
    }

    static double func_181162_h(double value) {
        return value - std::floor(value);
    }

    static long long getPositionRandom(const Vec3i& pos) {
        return getCoordinateRandom(pos.x, pos.y, pos.z);
    }

    static long long getCoordinateRandom(int x, int y, int z) {
        long long i = (static_cast<long long>(x) * 3129871LL) ^ (static_cast<long long>(z) * 116129781LL) ^ static_cast<long long>(y);
        i = i * i * 42317861LL + i * 11LL;
        return i;
    }

    static double func_181160_c(double a, double b, double c) {
        return (a - b) / (c - b);
    }

    static double atan2(double y, double x) {
        const double d0 = x * x + y * y;
        if (std::isnan(d0)) return std::numeric_limits<double>::quiet_NaN();

        const bool flagY = y < 0.0; if (flagY) y = -y;
        const bool flagX = x < 0.0; if (flagX) x = -x;
        const bool flag2 = y > x;
        if (flag2) { const double tmp = x; x = y; y = tmp; }

        const double d9 = func_181161_i(d0);
        x *= d9;
        y *= d9;

        const double d2 = field_181163_d + y;
        const int i = static_cast<int>(std::bit_cast<long long>(d2));
        const double d3 = field_181164_e[i];
        const double d4 = field_181165_f[i];
        const double d5 = d2 - field_181163_d;
        const double d6 = y * d4 - x * d5;
        const double d7 = (6.0 + d6 * d6) * d6 * 0.16666666666666666;
        double d8 = d3 + d7;

        if (flag2) d8 = (std::numbers::pi / 2.0) - d8;
        if (flagX)  d8 = std::numbers::pi - d8;
        if (flagY)  d8 = -d8;

        return d8;
    }

    static int hsvToRGB(float h, float s, float v) {
        const int i     = static_cast<int>(h * 6.0f) % 6;
        const float f   = h * 6.0f - static_cast<float>(i);
        const float f1  = v * (1.0f - s);
        const float f2  = v * (1.0f - f * s);
        const float f3  = v * (1.0f - (1.0f - f) * s);

        float r, g, b;
        switch (i) {
            case 0: r = v;  g = f3; b = f1; break;
            case 1: r = f2; g = v;  b = f1; break;
            case 2: r = f1; g = v;  b = f3; break;
            case 3: r = f1; g = f2; b = v;  break;
            case 4: r = f3; g = f1; b = v;  break;
            case 5: r = v;  g = f1; b = f2; break;
            default: throw std::runtime_error("Something went wrong when converting from HSV to RGB.");
        }

        return clamp_int(static_cast<int>(r * 255.0f), 0, 255) << 16
               | clamp_int(static_cast<int>(g * 255.0f), 0, 255) << 8
               | clamp_int(static_cast<int>(b * 255.0f), 0, 255);
    }

};



#endif //MCCLONE_MATHHELPER_H
