#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP
#include <cstddef>

#include "base.hpp"
#include "utility.hpp"

__STLL_NAMESPACE_START__


namespace
{

template <class Tp>
inline Tp* __allocate(size_t size, const Tp*) {
    Tp* result = static_cast<Tp*>(::operator new(size * sizeof(Tp)));
    return result;
}

template <class Tp>
inline void __deallocate(Tp* mem) {
    operator delete(static_cast<void*>(mem));
}

}

template <class Tp>
class allocator {
public:
    typedef Tp           value_type;
    typedef Tp*          pointer;
    typedef const Tp*    const_pointer;
    typedef Tp&          reference;
    typedef const Tp&    const_reference;
    typedef size_t       size_type;
    typedef ptrdiff_t    difference_type;

public:
    static pointer allocate(size_type size,
                     const void* ptr=static_cast<const void*>(nullptr)) {
        return __allocate(size, static_cast<const_pointer>(ptr));
    }

    static void deallocate(pointer ptr, size_type) {
        __deallocate(ptr);
    }

    static pointer address(const_reference elem) {
        return pointer(&elem);
    }

    static const_pointer const_address(const_reference elem) {
        return const_pointer(&elem);
    }

    static size_type max_size() {
        return max(size_type(1), size_type(~size_t(0) / sizeof (Tp)));
    }
};

__STLL_NAMESPACE_FINISH__

#endif // ALLOCATpOR_HPP
