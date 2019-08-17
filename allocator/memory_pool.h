#include <exception>
#include <iostream>
#include <cstdlib>
#include <climits>
#include <new>


#define NO_MEMORY_HANDLE throw std::bad_alloc();
template <int inst>
class malloc_alloc_template {
private:
    static void* oom_malloc(size_t n) {
        for(;;) {
            if (malloc_alloc_template_oom_handler == nullptr) {
                NO_MEMORY_HANDLE
            }
            malloc_alloc_template_oom_handler();
            void* result = malloc(n);
            if (result)
                return result;
        }
    }

    static void* oom_realloc(void* p, size_t n) {
    for(;;) {
            if (malloc_alloc_template_oom_handler == nullptr) {
                NO_MEMORY_HANDLE
            }
            malloc_alloc_template_oom_handler();
            void* result = realloc(p, n);
            if (result)
                return result;
        }
    }

    static void (*malloc_alloc_template_oom_handler)();

public:
    static void* allocate(size_t n) {
        void* result = malloc(n);
        if (0 == result) 
            result = oom_malloc(n);
        return result;
    }

    static void* reallocate(void* p, size_t n) {
        void* result = realloc(p, n);
        if (result == 0)
            result = oom_realloc(p, n);
        return result;
    }

    static void deallocate(void* p, size_t n) {
        free(p);
    }

    static void set_malloc_hanlder(void(*malloc_hanlder)()) {
        malloc_alloc_template_oom_handler = malloc_hanlder;
    }
};

template<int inst>
void (*malloc_alloc_template<inst>::malloc_alloc_template_oom_handler)() = nullptr;


#define SGI_OOM_IMPLEMENT


enum {ALIGN = 8};
enum {MAX_BYTES = 128};
enum {NFREELIISTS = MAX_BYTES / ALIGN};

union obj {
    union obj* next;
    char client_data[0];
};

template <int inst>
class alloc_template {
private:
    inline static size_t round_up(size_t bytes) {
        return ((bytes + (ALIGN - 1)) & (~(ALIGN-1)));
    }

private:
    
private:
    static union obj* free_list[NFREELIISTS];
    static void* free_segment_start;
    static void* free_segment_finish;
    static size_t heap_size;

private:

    static size_t FREELIST_INDEX(size_t bytes) {
        return (bytes + ALIGN - 1) / ALIGN - 1;
    }

    // n % 8 == 0
    // Get more memory and insert them into free_list.
    static void* refill(size_t size) {
        size_t nobjs = 20;
        void* result = chunk_alloc(size, &nobjs);

        if (nobjs == 1)
            return result;

        size_t index = FREELIST_INDEX(size);

        union obj* current_block = (union obj*)((char*)result + size);
        union obj* next_block = nullptr;
        free_list[index] = current_block;
        
        for (size_t i = 1; i < nobjs; ++i) {
            if (i == nobjs -1)
                next_block = nullptr;
            else
                next_block = (union obj*)((char*)current_block + size);
            current_block->next = next_block;
        }

        return result;
    }

    // size % 8 == 0
    // Only alloc memory and do not construct free_list.
    static void* chunk_alloc(size_t size, size_t* p_nobjs) {
        size_t bytes_left = size_t(free_segment_finish)
                            - size_t(free_segment_start);
        size_t bytes_wanted = size * (*p_nobjs);

        if (bytes_left >= bytes_wanted) {
            void* result = free_segment_start;
            free_segment_start = (void*)(
                        size_t(free_segment_start)
                        + bytes_wanted
                    );    

            return result;
        } else if (bytes_left >= size) {
            (*p_nobjs) = bytes_left / size;
            void* result = free_segment_start;
            free_segment_start = (void*)(
                        size_t(free_segment_start) + 
                        size * (*p_nobjs)
                    );
            return result;
        } else {
            if (bytes_left > 0) { 
                // bytes_left still is a multiple of 8.
                size_t index = FREELIST_INDEX(bytes_left);
                ((union obj*)free_segment_start)->next = free_list[index];
                free_list[index] = (union obj*)free_segment_start;

                bytes_left = 0;
            }

            size_t bytes_total_alloc = (
                        (bytes_wanted << 1) + (heap_size >> 4)
                   );

            free_segment_start = malloc(bytes_total_alloc);
            if (nullptr == free_segment_start) {
                for (size_t i = size; i < MAX_BYTES; size += ALIGN) {
                    size_t index = FREELIST_INDEX(i); 
                    union obj* block = free_list[index];
                    free_list[index] = free_list[index]->next;

                    free_segment_start = (void*)block;
                    free_segment_finish = (void*)free_list[index];
                    return chunk_alloc(size, p_nobjs);
                }

                free_segment_finish = nullptr;

#ifdef SGI_OOM_IMPLEMENT

                free_segment_start = malloc_alloc_template<0>::allocate(
                    bytes_total_alloc
                );

#else
                void* one_obj = malloc(size);
                if (nullptr == one_obj) {
                    one_obj = malloc_alloc_template<0>::allocate(size);
                }
                *p_nobjs = 1;
                return one_obj;
#endif
            }
            heap_size += bytes_total_alloc;

            free_segment_finish = (void*)(
                (char*)free_segment_start + bytes_total_alloc
            );
            return chunk_alloc(size, p_nobjs);
        }
    }

public:
    static void* allocate(size_t size) {
        if (size > MAX_BYTES) {
            return malloc_alloc_template<0>::allocate(size);
        }

        size = round_up(size);
        size_t index = FREELIST_INDEX(size);
        union obj* list = free_list[index];
        union obj* result = nullptr;
        if (list == nullptr) {
            result = (union obj*)refill(size);
        } else {
            result = list;
            free_list[index] = list->next;
        }
        
        return result;
    }

    static void* reallocate(void* p, size_t old_size, size_t new_size) {
        deallocate(p, old_size);
        return allocate(new_size);
    }

    static void deallocate(void* p, size_t size) {
        if (size > MAX_BYTES) {
            malloc_alloc_template<0>::deallocate(p, size);
        }

        size = round_up(size);
        size_t index = FREELIST_INDEX(size);
        ((union obj*)p)->next = free_list[index];
        free_list[index] = (union obj*)p;
    }
};


template <int inst>
void* alloc_template<inst>::free_segment_start = nullptr;

template <int inst>
void* alloc_template<inst>::free_segment_finish = nullptr;

template <int inst>
size_t alloc_template<inst>::heap_size = 0;

template <int inst>
union obj* alloc_template<inst>::free_list[NFREELIISTS] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

using alloc = alloc_template<0>;
using maloc_alloc = malloc_alloc_template<0>;


template <class T, class Alloc=alloc>
class simple_alloc {
public:
    typedef T           value_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type; 

public:
    static T* allocate(size_t n) {
        return (0 == n ? (T*)nullptr : (T*)Alloc::allocate(n * sizeof(T)));
    }

    static T* allocate(void) {
        return (T*)Alloc::allocate(sizeof(T));
    }
    
    static void deallocate(T* p, size_t n) {
        if (p && n)
            Alloc::deallocate(p, sizeof(n * sizeof(T)));
    }

    static void deallocate(T* p) {
        if (p)
            Alloc::deallocate(p, sizeof(T));
    }
};
