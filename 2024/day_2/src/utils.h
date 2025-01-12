#include <stdio.h>

#ifndef UTILS_H
#define UTILS_H

typedef struct {
  int *items;
  int capacity;
  int len;
} array;

typedef struct {
  array *items;
  int capacity;
  int len;
} nestedarray;

nestedarray parse_data(FILE *fp);
void append(array *a, int new_item);
void resize_array(array *a);
array new_array(int capacity);
void free_nested_array(nestedarray a);

#endif
