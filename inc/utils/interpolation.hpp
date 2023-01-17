#pragma once

template<typename T>
inline T linear_interpolation(const T& a, const T& b, const T& t) {
    return a + (b - a) * t;
}

template<typename T>
inline T quadratic_interpolation(const T& min, const T& max, const T& t) {
    return min + (max - min) * (-(t * t) * 4 + t * 4);
}

template<typename T>
inline T quadratic_interpolation_by_val(const T& min, const T& max, const T& val) {
    T val_fract = (val - min) / (max - min);
    return min + (max - min) * (-(val_fract * val_fract) * 4 + val_fract * 4);
}