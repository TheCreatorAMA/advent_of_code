#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  size_t length;
  size_t capacity;
  int *items; // NOTE: This is a void pointer, it really is just
} IntArray;   // memory location but we dont know whats there.

typedef struct {
  size_t length;
  size_t capacity;
  void *items;
} NestedArray;

IntArray *createDynamicIntArray(int initialCapacity);
void append(IntArray *arr, int value);
void freeArray(IntArray *arr);

int main(void) {
  const int INIT_ARR_SIZE = 5;

  FILE *file = fopen("./data.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  char buffer[255];

  // NOTE: Parse file line by line
  while (
      fgets(buffer, sizeof(buffer), file) !=
      NULL) { // NTOE: fgets() returns NULL when the end of the file is reached

    IntArray *arr = createDynamicIntArray(INIT_ARR_SIZE);
    char *token;
    int cur_num;

    token = strtok(buffer, " ");
    while (token != NULL) {
      cur_num = atoi(token);
      append(arr, cur_num);
      token = strtok(NULL, " "); // Iterate to next part of string
    }

    // NOTE: Print all loaded arrays
    // printf("Loaded array: ");
    // for (int i = 0; i < arr->length; i++) {
    //   if (i == arr->length - 1) {
    //     printf("%d\n", arr->items[i]);
    //   } else {
    //     printf("%d ", arr->items[i]);
    //   }
    // }

    freeArray(arr);
  }

  fclose(file);

  return 0;
}

IntArray *createDynamicIntArray(int initialCapacity) {
  IntArray *arr = (IntArray *)malloc(sizeof(IntArray));
  if (arr == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }

  arr->items = (int *)malloc(initialCapacity * sizeof(int));
  if (arr->items == NULL) {
    printf("Memory allocation failed\n");
    exit(1);
  }

  arr->length = 0;
  arr->capacity = initialCapacity;

  return arr;
}

void append(IntArray *arr, int value) {
  if (arr->length == arr->capacity) {
    // NOTE: Resize array
    int newCapacity = arr->capacity * 2;
    int *newArray = (int *)realloc(arr->items, newCapacity * sizeof(int));
    if (newArray == NULL) {
      printf("Failed to resize array, memory allocation failed\n");
      exit(1);
    }
    arr->items = newArray;
    arr->capacity = newCapacity;
  }

  arr->items[arr->length] = value;
  arr->length++;
}

void freeArray(IntArray *arr) {
  free(arr->items);
  free(arr);
}
