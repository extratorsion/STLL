#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

#include "base.hpp"
#include <algorithm>

__STLL_NAMESPACE_START__


template <typename Arg, typename Result>
struct unary_function {
    typedef Arg     argument_type;
    typedef Result  result_type;
};

template <typename Arg1, typename Arg2, typename Result>
struct binary_function {
    typedef Arg1    first_argument_type;
    typedef Arg2    second_argument_type;
    typedef Result  result_type;
};


template <typename Tp>
struct negate : public unary_function<Tp, Tp> {
    Tp operator()(const Tp& elem) const {
        return -elem;
    }
};

template <typename Tp>
struct logical_not : public unary_function<Tp, bool> {
    Tp operator()(const Tp& elem) const {
        return !elem;
    }
};

template <typename Tp>
struct logical_and : public  binary_function<Tp, Tp, bool> {
    bool operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 & elem2;;
    }
};

template <typename Tp>
struct logical_or : public  binary_function<Tp, Tp, bool> {
    bool operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 || elem2;;
    }
};

template <typename Tp>
struct less : public  binary_function<Tp, Tp, bool> {
    bool operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 < elem2;;
    }
};

template <typename Tp>
struct greater : public  binary_function<Tp, Tp, bool>  {
    bool operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 > elem2;;
    }
};

template <typename Tp>
struct equal_to : public  binary_function<Tp, Tp, bool>  {
    bool operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 == elem2;;
    }
};

template <typename Tp>
struct not_equal_to : public  binary_function<Tp, Tp, bool>  {
    bool operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 != elem2;;
    }
};

template <typename Tp>
struct less_equal : public  binary_function<Tp, Tp, bool> {
    bool operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 <= elem2;;
    }
};

template <typename Tp>
struct greater_equal : public  binary_function<Tp, Tp, bool>  {
    bool operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 >= elem2;
    }
};

template <typename Tp>
struct multiplies : public  binary_function<Tp, Tp, Tp>  {
    Tp operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 * elem2;
    }
};

template <typename Tp>
struct plus : public  binary_function<Tp, Tp, Tp>  {
    Tp operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 + elem2;
    }
};

template <typename Tp>
struct minus : public  binary_function<Tp, Tp, Tp>  {
    Tp operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 - elem2;
    }
};

template <typename Tp>
struct divides : public  binary_function<Tp, Tp, Tp>  {
    Tp operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 / elem2;
    }
};

template <typename Tp>
struct modulus : public  binary_function<Tp, Tp, Tp>  {
    Tp operator()(const Tp& elem1, const Tp& elem2) const {
        return elem1 % elem2;
    }
};

template <typename Tp>
struct identity : public unary_function<Tp, Tp> {
    const Tp& operator()(const Tp& x) const {
        return x;
    }
};

template <typename Pair>
struct selct1st : public unary_function<Pair, typename Pair::first_type> {
    const typename Pair::first_type& operator()(const Pair& x) {
        return x.first;
    }
};

template <typename Pair>
struct select2nd : public unary_function<Pair, typename Pair::second_type> {
    const typename Pair::second_type& operator()(const Pair& x) {
        return x.first;
    }
};

template <typename Arg1, typename Arg2>
struct project1st : public binary_function<Arg1, Arg2, Arg1> {
    Arg1 operator()(const Arg1& arg1, const Arg2&) const {
        return arg1;
    }
};

template <typename Arg1, typename Arg2>
struct project2nd : public binary_function<Arg1, Arg2, Arg1> {
    Arg1 operator()(const Arg1&, const Arg2& arg2) const {
        return arg2;
    }
};

/* indentity_element */
template <class Tp>
inline Tp indentity_element(const plus<Tp>&){ return Tp(0); }

template <class Tp>
inline Tp indentity_element(const multiplies<Tp>&) { return Tp(0); }


/* Function adapter */
template <typename Ret, typename... Args>
class ptr_fun_t {
protected:
   Ret(*ptr)(Args...) ;

public:
    ptr_fun_t(Ret(*ptr)(Args...))
        :ptr(ptr)
    {}

    Ret operator()(Args... args) const {
        return (*ptr)(args...);
    }
};

template <typename Ret, typename... Args>
inline ptr_fun_t<Ret, Args...> ptr_fun(Ret(*ptr)(Args...)) {
    return ptr_fun_t<Ret, Args...>(ptr);
}


template <typename Ret, typename Class, typename... Args>
class mem_fun_t {
protected:
   Ret(Class::*ptr)(Args...);

public:
    mem_fun_t(Ret(Class::*ptr)(Args...))
        :ptr(ptr)
    {}

    Ret operator()(Class* p, Args... args) const {
        return (p->*ptr)(args...);
    }
};

template <typename Ret, typename Class, typename... Args>
inline mem_fun_t<Ret, Class, Args...> mem_fun(Ret(Class::*ptr)(Args...)) {
    return mem_fun_t<Ret, Class>(ptr);
}

template <typename Ret, typename Class, typename... Args>
class mem_ref_fun_t {
protected:
   Ret(Class::*ptr)(Args...);

public:
    mem_ref_fun_t(Ret(Class::*ptr)(Args...))
        :ptr(ptr)
    {}

    Ret operator()(Class& p, Args... args) const {
        return (p.*ptr)(args...);
    }
};

template <typename Ret, typename Class, typename... Args>
inline mem_fun_t<Ret, Class, Args...> mem_fun_ref(Ret(Class::*ptr)(Args...)) {
    return mem_fun_ref<Ret, Class, Args...>(ptr);
}

/* Binder */
template <class Operation>
class binder1st : public unary_function<
        typename Operation::second_argument_type,
        typename Operation::result_type> {
protected:
    typename Operation::first_argument_type value;
    Operation operation;

public:
    binder1st(const Operation& op,
              const typename Operation::first_argument_type& arg)
        :operation(op), value(arg)
    {}

    typename Operation::result_type
    operator()(const typename Operation::second_argument_type& arg) const {
        return operation(value, arg);
    }
};

template <typename Operation, typename FirstArg>
inline binder1st<Operation> bind1st(const Operation& op, const FirstArg& arg) {
    return binder1st<Operation>(op, arg);
}

template <class Operation>
class binder2nd : public unary_function<
        typename Operation::second_argument_type,
        typename Operation::result_type> {
protected:
    typename Operation::first_argument_type value;
    Operation operation;

public:
    binder2nd(const Operation& op,
              const typename Operation::first_argument_type& arg)
        :operation(op), value(arg)
    {}

    typename Operation::result_type
    operator()(const typename Operation::second_argument_type& arg) const {
        return operation(arg, value);
    }
};

template <typename Operation, typename SecondArg>
inline binder1st<Operation> bind2nd(const Operation& op, const SecondArg& arg) {
    return binder1st<Operation>(op, arg);
}

__STLL_NAMESPACE_FINISH__


#endif // FUNCTOR_HPP
