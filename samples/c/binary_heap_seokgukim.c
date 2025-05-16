#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For memcpy

// Begin of heap implementation
#define DEFINE_HEAP(type, name_prefix) \
    typedef struct { \
        type *data; \
        size_t size; \
        size_t capacity; \
        int (*compare)(const type *a, const type *b); \
    } name_prefix##_heap_t;
#define HEAP_INIT(heap, initial_capacity, compare_func) \
    do { \
        (heap)->data = (void *)malloc((initial_capacity) * sizeof(*(heap)->data)); \
        if (!(heap)->data) { \
            perror("Failed to allocate memory for heap data"); \
            exit(EXIT_FAILURE); \
        } \
        (heap)->size = 0; \
        (heap)->capacity = (initial_capacity); \
        (heap)->compare = (compare_func); \
    } while (0)
#define HEAP_PARENT(i) (((i) - 1) / 2)
#define HEAP_LEFT_CHILD(i) (2 * (i) + 1)
#define HEAP_RIGHT_CHILD(i) (2 * (i) + 2)
#define HEAP_SWAP(type, a, b) \
    do { \
        type temp = *(a); \
        *(a) = *(b); \
        *(b) = temp; \
    } while (0)
#define HEAP_HEAPIFY_UP(heap, index, type) \
    do { \
        size_t current_idx = (index); \
        while (current_idx > 0 && (heap)->compare(&(heap)->data[current_idx], &(heap)->data[HEAP_PARENT(current_idx)]) < 0) { \
            HEAP_SWAP(type, &(heap)->data[current_idx], &(heap)->data[HEAP_PARENT(current_idx)]); \
            current_idx = HEAP_PARENT(current_idx); \
        } \
    } while (0)
#define HEAP_HEAPIFY_DOWN(heap, index, type) \
    do { \
        size_t current_idx = (index); \
        size_t min_idx = current_idx; \
        while (1) { \
            size_t left_idx = HEAP_LEFT_CHILD(current_idx); \
            size_t right_idx = HEAP_RIGHT_CHILD(current_idx); \
            if (left_idx < (heap)->size && (heap)->compare(&(heap)->data[left_idx], &(heap)->data[min_idx]) < 0) { \
                min_idx = left_idx; \
            } \
            if (right_idx < (heap)->size && (heap)->compare(&(heap)->data[right_idx], &(heap)->data[min_idx]) < 0) { \
                min_idx = right_idx; \
            } \
            if (min_idx == current_idx) { \
                break; \
            } \
            HEAP_SWAP(type, &(heap)->data[current_idx], &(heap)->data[min_idx]); \
            current_idx = min_idx; \
        } \
    } while (0)
#define HEAP_INSERT(heap, value, type) \
    do { \
        if ((heap)->size == (heap)->capacity) { \
            (heap)->capacity = ((heap)->capacity == 0) ? 1 : (heap)->capacity * 2; \
            (heap)->data = (void *)realloc((heap)->data, (heap)->capacity * sizeof(type)); \
            if (!(heap)->data) { \
                perror("Failed to reallocate memory for heap data"); \
                exit(EXIT_FAILURE); \
            } \
        } \
        (heap)->data[(heap)->size] = (value); \
        (heap)->size++; \
        HEAP_HEAPIFY_UP(heap, (heap)->size - 1, type); \
    } while (0)
#define HEAP_POP(heap, result_ptr, type) \
    do { \
        if ((heap)->size == 0) { \
            perror("Heap is empty, cannot extract min\n"); \
        } else { \
            *(result_ptr) = (heap)->data[0]; \
            (heap)->data[0] = (heap)->data[(heap)->size - 1]; \
            (heap)->size--; \
            if ((heap)->size > 0) { \
                HEAP_HEAPIFY_DOWN(heap, 0, type); \
            } \
        } \
    } while (0)
#define HEAP_PEEK(heap, result_ptr) \
    do { \
        if ((heap)->size == 0) { \
            perror("Heap is empty, cannot peek min\n"); \
        } else { \
            *(result_ptr) = (heap)->data[0]; \
        } \
    } while (0)
#define HEAP_SIZE(heap) ((heap)->size)
#define HEAP_IS_EMPTY(heap) ((heap)->size == 0)
#define HEAP_FREE(heap) \
    do { \
        if ((heap)->data) { \
            free((heap)->data); \
            (heap)->data = NULL; \
        } \
        (heap)->size = 0; \
        (heap)->capacity = 0; \
    } while (0)
#define HEAP_SORT(heap, type) \
    do { \
        for (size_t i = (heap)->size / 2; i != SIZE_MAX; i--) { \
            HEAP_HEAPIFY_DOWN(heap, i, type); \
        } \
    } while (0)
// End of heap implementation

// Example Usage:
// 1. Define the heap type for integers
DEFINE_HEAP(int, int);

// 2. Define a comparison function for integers (for a min-heap)
int compare_int(const int *a, const int *b) {
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

// Example for a struct
typedef struct {
    int id;
    float value;
} MyStruct;

DEFINE_HEAP(MyStruct, mystruct);

int compare_mystruct_by_value(const MyStruct *a, const MyStruct *b) {
    if (a->value < b->value) return -1;
    if (a->value > b->value) return 1;
    return 0;
}


int main() {
    // Integer Heap Example
    printf("--- Integer Min-Heap Example ---\n");
    int_heap_t my_int_heap;
    HEAP_INIT(&my_int_heap, 10, compare_int);

    HEAP_INSERT(&my_int_heap, 10, int);
    HEAP_INSERT(&my_int_heap, 4, int);
    HEAP_INSERT(&my_int_heap, 15, int);
    HEAP_INSERT(&my_int_heap, 20, int);
    HEAP_INSERT(&my_int_heap, 0, int);
    HEAP_INSERT(&my_int_heap, 3, int);

    printf("Heap size: %zu\n", HEAP_SIZE(&my_int_heap));

    int min_val;
    if (!HEAP_IS_EMPTY(&my_int_heap)) {
        HEAP_PEEK(&my_int_heap, &min_val);
        printf("Peek min: %d\n", min_val);
    }

    printf("Extracting elements:\n");
    while (!HEAP_IS_EMPTY(&my_int_heap)) {
        HEAP_POP(&my_int_heap, &min_val, int);
        printf("%d ", min_val);
    }
    printf("\n");
    printf("Heap size after extraction: %zu\n", HEAP_SIZE(&my_int_heap));
    HEAP_FREE(&my_int_heap);

    // MyStruct Heap Example
    printf("\n--- MyStruct Min-Heap (by value) Example ---\n");
    mystruct_heap_t my_struct_heap;
    HEAP_INIT(&my_struct_heap, 5, compare_mystruct_by_value);

    MyStruct s1 = {1, 10.5f};
    MyStruct s2 = {2, 3.2f};
    MyStruct s3 = {3, 15.0f};
    MyStruct s4 = {4, 0.5f};

    HEAP_INSERT(&my_struct_heap, s1, MyStruct);
    HEAP_INSERT(&my_struct_heap, s2, MyStruct);
    HEAP_INSERT(&my_struct_heap, s3, MyStruct);
    HEAP_INSERT(&my_struct_heap, s4, MyStruct);

    printf("Struct Heap size: %zu\n", HEAP_SIZE(&my_struct_heap));

    MyStruct min_struct;
    if (!HEAP_IS_EMPTY(&my_struct_heap)) {
        HEAP_PEEK(&my_struct_heap, &min_struct);
        printf("Peek min struct: id=%d, value=%.2f\n", min_struct.id, min_struct.value);
    }

    printf("Extracting struct elements (by value):\n");
    while (!HEAP_IS_EMPTY(&my_struct_heap)) {
        HEAP_POP(&my_struct_heap, &min_struct, MyStruct);
        printf("id=%d, value=%.2f | ", min_struct.id, min_struct.value);
    }
    printf("\n");
    printf("Struct Heap size after extraction: %zu\n", HEAP_SIZE(&my_struct_heap));
    HEAP_FREE(&my_struct_heap);

    return 0;
}