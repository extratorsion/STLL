#ifndef BASE_HPP
#define BASE_HPP

/*
 * STLL: STL Learning.
 * All the file in this project are aim to learn and implement the essence of
 * STL(standard template library). It maybe premature, but I think it's more
 * easy to understanding as a STL learning matrial.
 * It's still under developing, if you have found any bug, you can send email to
 * me <extratorsion@outlook.com>.
 *
 * About copyright
 * Free to copy, free to use.
 */

#include <cstddef>

#define DEBUG

#ifdef DEBUG
#include <iostream>
#define STR(X) #X
#define $(expr) std::cout << STR(expr) << ": " << (expr) << std::endl;
#endif

#define STLL_NAMESPACE stll

#define __STLL_NAMESPACE_START__ namespace STLL_NAMESPACE {

#define __STLL_NAMESPACE_FINISH__ }

#define __ANONYMOUS_NAMESPACE_START namespace {
#define __ANONYMOUS_NAMESPACE_FINISH }



#endif // BASE_HPP
