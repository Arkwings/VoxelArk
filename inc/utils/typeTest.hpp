#pragma once

template<typename T, typename U>
struct is_same: std::false_type {};

template<typename T>
struct is_same<T, T>: std::true_type {};

template<typename T, typename U>
constexpr bool eqTypes() { return is_same<T, U>::value; }