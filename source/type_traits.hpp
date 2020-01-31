#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

#include "base.hpp"

__STLL_NAMESPACE_START__
/*
 * true_type:   POD like, the object of true_type should have all data in stack.
 *              It's default constructor, defaut destructor... is trivial.
 * false_type:  Type which have data in heap, constructor or destructor function
 *              must be invoked when object was created or destroyed.
 */
struct true_type{};
struct false_type{};


/*For all types programmer defined, it's false_type by default.*/
template <class Type>
struct type_traits {
    typedef true_type       this_dumpy_member_must_be_first;
    typedef false_type      has_trivial_default_constructor;
    typedef false_type      has_trivial_copy_constructor;
    typedef false_type      has_trivial_assignment_operator;
    typedef false_type      has_trivial_destructor;
    typedef false_type      is_POD_type; // POD: plain old type.
};

template<class Type>
struct type_traits<Type*> {
    typedef true_type      has_trivial_default_constructor;
    typedef true_type      has_trivial_copy_constructor;
    typedef true_type      has_trivial_assignment_operator;
    typedef true_type      has_trivial_destructor;
    typedef true_type      is_POD_type;
};

/* For all builtin types, it's true_type by default. */
template <>
struct type_traits<char> {
    typedef true_type      has_trivial_default_constructor;
    typedef true_type      has_trivial_copy_constructor;
    typedef true_type      has_trivial_assignment_operator;
    typedef true_type      has_trivial_destructor;
    typedef true_type      is_POD_type;
};

template <>
struct type_traits<signed char> {
    typedef true_type      has_trivial_default_constructor;
    typedef true_type      has_trivial_copy_constructor;
    typedef true_type      has_trivial_assignment_operator;
    typedef true_type      has_trivial_destructor;
    typedef true_type      is_POD_type;
};


template <>
struct type_traits<unsigned char> {
    typedef true_type      has_trivial_default_constructor;
    typedef true_type      has_trivial_copy_constructor;
    typedef true_type      has_trivial_assignment_operator;
    typedef true_type      has_trivial_destructor;
    typedef true_type      is_POD_type;
};

template <>
struct type_traits<short> {
    typedef true_type      has_trivial_default_constructor;
    typedef true_type      has_trivial_copy_constructor;
    typedef true_type      has_trivial_assignment_operator;
    typedef true_type      has_trivial_destructor;
    typedef true_type      is_POD_type;
};

template <>
struct type_traits<unsigned short> {
    typedef true_type      has_trivial_default_constructor;
    typedef true_type      has_trivial_copy_constructor;
    typedef true_type      has_trivial_assignment_operator;
    typedef true_type      has_trivial_destructor;
    typedef true_type      is_POD_type;
};



template <>
struct type_traits<int> {
    typedef true_type      has_trivial_default_constructor;
    typedef true_type      has_trivial_copy_constructor;
    typedef true_type      has_trivial_assignment_operator;
    typedef true_type      has_trivial_destructor;
    typedef true_type      is_POD_type;
};

template <>
struct type_traits<unsigned int> {
    typedef true_type      has_trivial_default_constructor;
    typedef true_type      has_trivial_copy_constructor;
    typedef true_type      has_trivial_assignment_operator;
    typedef true_type      has_trivial_destructor;
    typedef true_type      is_POD_type;
};

template <>
struct type_traits<long> {
    typedef true_type      has_trivial_default_constructor;
    typedef true_type      has_trivial_copy_constructor;
    typedef true_type      has_trivial_assignment_operator;
    typedef true_type      has_trivial_destructor;
    typedef true_type      is_POD_type;
};

template <>
struct type_traits<unsigned long> {
    typedef true_type      has_trivial_default_constructor;
    typedef true_type      has_trivial_copy_constructor;
    typedef true_type      has_trivial_assignment_operator;
    typedef true_type      has_trivial_destructor;
    typedef true_type      is_POD_type;
};

template <>
struct type_traits<long long> {
    typedef true_type      has_trivial_default_constructor;
    typedef true_type      has_trivial_copy_constructor;
    typedef true_type      has_trivial_assignment_operator;
    typedef true_type      has_trivial_destructor;
    typedef true_type      is_POD_type;
};

template <>
struct type_traits<unsigned long long> {
    typedef true_type      has_trivial_default_constructor;
    typedef true_type      has_trivial_copy_constructor;
    typedef true_type      has_trivial_assignment_operator;
    typedef true_type      has_trivial_destructor;
    typedef true_type      is_POD_type;
};

template <>
struct type_traits<float> {
    typedef true_type      has_trivial_default_constructor;
    typedef true_type      has_trivial_copy_constructor;
    typedef true_type      has_trivial_assignment_operator;
    typedef true_type      has_trivial_destructor;
    typedef true_type      is_POD_type;
};

template <>
struct type_traits<double> {
    typedef true_type      has_trivial_default_constructor;
    typedef true_type      has_trivial_copy_constructor;
    typedef true_type      has_trivial_assignment_operator;
    typedef true_type      has_trivial_destructor;
    typedef true_type      is_POD_type;
};

template <>
struct type_traits<long double> {
    typedef true_type      has_trivial_default_constructor;
    typedef true_type      has_trivial_copy_constructor;
    typedef true_type      has_trivial_assignment_operator;
    typedef true_type      has_trivial_destructor;
    typedef true_type      is_POD_type;
};


template <class Tp>
struct type_identity {
    typedef Tp       raw_type;
};

template <class Tp>
struct type_identity<Tp&> {
    typedef Tp       raw_type;
};

template <class Tp>
struct type_identity<const Tp&> {
    typedef Tp       raw_type;
};



template <class Tp> 
struct remove_reference { using type = Tp; };

template <class Tp>
struct remove_reference<Tp&> { using type = Tp; };

template <class Tp>
struct remove_reference<Tp&&> { using type = Tp; };


__STLL_NAMESPACE_FINISH__


#endif // TYPE_TRAITS_HPP
