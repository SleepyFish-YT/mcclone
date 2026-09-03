//
// Created by SleepyFish on 02.09.2026.
// Project: mcclone
//

#ifndef MCCLONE_VEC4B_H
#define MCCLONE_VEC4B_H

/**
 * @author SleepyFish
 * @brief 4 byte vector class
 * @relatesalso signed char
 */
class Vec4b {

public:

    const signed char field_176117_a;
    const signed char field_176115_b;
    const signed char field_176116_c;
    const signed char field_176114_d;

    Vec4b(signed char x, signed char y, signed char z, signed char w) noexcept :
        field_176117_a(x),
        field_176115_b(y),
        field_176116_c(z),
        field_176114_d(w)
    {}

    Vec4b(const Vec4b& vec4b) noexcept :
        field_176117_a(vec4b.field_176117_a),
        field_176115_b(vec4b.field_176115_b),
        field_176116_c(vec4b.field_176116_c),
        field_176114_d(vec4b.field_176114_d)
    {}

    int hashCode() const noexcept {
        int i = this->field_176117_a;
        i = 31 * i + this->field_176115_b;
        i = 31 * i + this->field_176116_c;
        i = 31 * i + this->field_176114_d;
        return i;
    }

};

#endif //MCCLONE_VEC4B_H
