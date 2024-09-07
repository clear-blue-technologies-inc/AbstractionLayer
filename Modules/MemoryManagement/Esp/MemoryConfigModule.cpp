#include "esp_heap_caps.h"

//Going off the strategies listed here: https://docs.espressif.com/projects/esp-idf/en/v5.1.1/esp32/api-reference/kconfig.html#config-spiram-malloc-alwaysinternal
//https://docs.espressif.com/projects/esp-idf/en/v5.1.1/esp32/api-reference/system/mem_alloc.html
static void *allocate(size_t size) {
    constexpr size_t spiramThreshold = 8196;

    if (size <= spiramThreshold) {
        if (nullptr == heap_caps_malloc(size, MALLOC_CAP_8BIT)) {
            return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
        }
    }
    else {
        if (nullptr == heap_caps_malloc(size, MALLOC_CAP_SPIRAM)) {
            return heap_caps_malloc(size, MALLOC_CAP_8BIT);
        }
    }
}

void *operator new(size_t size) {
    return allocate(size);
}

void *operator new[](size_t size) {
    return allocate(size);
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