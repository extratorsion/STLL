#ifndef HASH_HPP
#define HASH_HPP

#include "base.hpp"

__STLL_NAMESPACE_START__

template <typename Tp, size_t size>
struct hash_type_length {
    typedef Tp              alter_type;
};

/* For user defined types,
 * the hash method is to return object's address.*/
template <typename Tp>
struct hash {
    size_t operator()(const Tp& obj) {
        if (sizeof(Tp) >= sizeof(size_t))
            return size_t(*(reinterpret_cast<const size_t*>(&obj)));
        else {
            using alter_type =
            typename hash_type_length<Tp, sizeof(Tp)>::alter_type;
            return hash<alter_type>()
                    (*(reinterpret_cast<const alter_type*>(&obj)));
        }
    }
};


template <typename Tp>
struct hash_type_length<Tp, 1> {
    typedef unsigned char alter_type;
};

template <typename Tp>
struct hash_type_length<Tp, 2> {
    typedef unsigned short alter_type;
};

template <typename Tp>
struct hash_type_length<Tp, 3> {
    typedef unsigned short alter_type;
};

template <typename Tp>
struct hash_type_length<Tp, 4> {
    typedef unsigned int alter_type;
};

template <typename Tp>
struct hash_type_length<Tp, 5> {
    typedef unsigned int alter_type;
};

template <typename Tp>
struct hash_type_length<Tp, 6> {
    typedef unsigned int alter_type;
};

template <typename Tp>
struct hash_type_length<Tp, 7> {
    typedef unsigned int alter_type;
};

template <typename Tp>
struct hash_type_length<Tp, 8> {
    typedef unsigned long long alter_type;
};

template <typename Tp>
struct hash<Tp*> {
    size_t operator()(const Tp* obj) {
        return size_t(obj);
    }
};


template <typename char_t>
inline size_t hash_string_wrapper(const char_t* s) {
    size_t hash_value = 0;
    for (; *s; ++s)
        hash_value += (5 * (hash_value) + size_t(*s));
    return hash_value;
}


template <>
struct hash<char*> {
    size_t operator()(const char* s) const {
        return hash_string_wrapper<char>(s);
    }
};
template <>
struct hash<const char*> {
    size_t operator()(const char* s) const {
        return hash_string_wrapper<char>(s);
    }
};


template <>
struct hash<wchar_t*> {
    size_t operator()(const wchar_t* s) const {
        return hash_string_wrapper<wchar_t>(s);
    }
};

template <>
struct hash<const wchar_t*> {
    size_t operator()(const wchar_t* s) const {
        return hash_string_wrapper<wchar_t>(s);
    }
};


template <>
struct hash<char> {
    size_t operator()(const char& x) const {
        return size_t(x);
    }
};

template <>
struct hash<signed char> {
    size_t operator()(const signed char& x) const {
        return size_t(x);
    }
};

template <>
struct hash<unsigned char> {
    size_t operator()(const unsigned char& x) const {
        return size_t(x);
    }
};

template <>
struct hash<wchar_t> {
    size_t operator()(const wchar_t& x) const {
        return size_t(x);
    }
};

template <>
struct hash<short> {
    size_t operator()(const short& x) const {
        return size_t(x);
    }
};

template <>
struct hash<unsigned short> {
    size_t operator()(const unsigned short& x) const {
        return size_t(x);
    }
};

template <>
struct hash<int> {
    size_t operator()(const int& x) const {
        return size_t(x);
    }
};

template <>
struct hash<unsigned int> {
    size_t operator()(const unsigned int& x) const {
        return size_t(x);
    }
};

template <>
struct hash<long> {
    size_t operator()(const long& x) const {
        return size_t(x);
    }
};

template <>
struct hash<unsigned long> {
    size_t operator()(const unsigned long& x) const {
        return size_t(x);
    }
};

template <>
struct hash<long long> {
    size_t operator()(const long long& x) const {
        return size_t(x);
    }
};

template <>
struct hash<unsigned long long> {
    size_t operator()(const unsigned long long& x) const {
        return size_t(x);
    }
};


__STLL_NAMESPACE_FINISH__

#endif // HASH_HPP
