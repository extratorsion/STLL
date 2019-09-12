#ifndef UTILS_HPP
#define UTILS_HPP

#include "base.hpp"
#include <iostream>

__STLL_NAMESPACE_START__

template <class Comparable>
const Comparable& max(const Comparable& elem1, const Comparable& elem2) {
    return elem1 > elem2 ? elem1 : elem2;
}

template <class Comparable>
const Comparable& min(const Comparable& elem1, const Comparable& elem2) {
    return elem1 < elem2 ? elem1 : elem2;
}

template <class Tp>
void swap(Tp& elem1, Tp& elem2) {
    Tp tmp = elem1;
    elem1 = elem2;
    elem2 = tmp;
}


/* print: Print args to screen,
 *        just like std::cout <<, but more easy to type
 */
void print() {}

template <typename Arg, typename... Args>
void print(const Arg& arg, const Args... args) {
    std::cout << arg;
    if (sizeof...(args) != 0)
        print(args...);
}

/* println: Print args to screen, and print std::endl at the end,
 *        just like std::cout <<, but more easy to type
 */
void println() {std::cout << std::endl;}
template <typename Arg, typename... Args>
void println(const Arg& arg, const Args... args) {
    if (sizeof...(args) == 0)
        std::cout << arg << std::endl;
    else {
        std::cout << arg;
        println(args...);
    }
}

/* all: Same as all in python3
 * return true if all element if true
 * else return false
 */
template <typename InputIterator>
bool all(InputIterator first, InputIterator last) {
    while (first != last) {
        if (!(*first))
            return false;
        ++first;
    }
    return true;
}

template <typename InputIterator, typename Pred>
bool all(InputIterator first, InputIterator last, const Pred& pred) {
    while (first != last) {
        if (!pred(*first))
            return false;
        ++first;
    }
    return true;
}

/* all: same as any in python3
 * return true if there have at least one element is true
 * else return false
 */
template <typename InputIterator>
bool any(InputIterator first, InputIterator last) {
    while (first != last) {
        if (*first)
            return true;
        ++first;
    }
    return false;
}

template <typename InputIterator, typename Pred>
bool any(InputIterator first, InputIterator last, const Pred& pred) {
    while (first != last) {
        if (pred(*first))
            return true;
        ++first;
    }
    return false;
}



__STLL_NAMESPACE_FINISH__

#endif // UTILS_HPP
