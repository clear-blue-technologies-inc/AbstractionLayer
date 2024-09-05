#include "esp_heap_caps.h"

//TODO: I think this needs a bit more thought. I feel like I don't know enough about the memory
//layout to be confident that these calls are the best choice. Maybe could do something like choose
//where to allocate based on the size.
void *operator new(size_t size) {
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
}

void *operator new[](size_t size) {
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
}

void operator delete(void *ptr) {
    heap_caps_free(ptr);
}
void operator delete(void *ptr, unsigned int) {
    heap_caps_free(ptr);
}

void operator delete[](void *ptr) {
    heap_caps_free(ptr);
}
void operator delete [](void *ptr, unsigned int) {
    heap_caps_free(ptr);
}