#include <iostream>
#include <climits>
#include <cstdlib>
#include <memory>
#include <new>

template<class T>
inline T* _allocate(size_t size, T*) {
    std::set_new_handler(nullptr);
    T* tmp = (T*)(::operator new(size * sizeof(T)));

    if (nullptr == tmp) {
        std::cerr << "Out of memory." << std::endl;
        exit(1);
    }
    return tmp;
}

template<class T>
inline void _deallocate(T* buffer) {
    ::operator delete(buffer);
}

template<class T1, class T2>
inline void _construct(T1* ptr, const T2& value) {
    new(ptr)T1(value);
}

template<class T>
inline void _destroy(T* ptr) {
    ptr->~T();
}

template <class T>
class allocator {
public:
    typedef T           value_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type; 
    

    pointer allocate(size_type n, const void* hint=nullptr) {
        return _allocate((difference_type)n, (T*)nullptr);
    }

    void deallocate(pointer p, size_type n) {
        _deallocate(p);
    }

    void construct(pointer p, size_type n) {
        _construct(p, n);
    }

    void destroy(pointer p) {
        _destroy(p);
    }

    pointer address(reference x) {
        return (pointer)&x;
    }

    const_pointer const_address(const_reference x) {
        return (const_pointer)&x;
    }

    size_type max_size() const {
        return size_type(UINT_MAX/sizeof(T));
    }

};


int main()
{
    return 0;
}
