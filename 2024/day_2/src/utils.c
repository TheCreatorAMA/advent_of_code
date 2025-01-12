#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

array new_array(int capacity) {
  array new_array = {
      .capacity = capacity,
      .len = 0,
  };

  new_array.items = malloc(new_array.capacity * sizeof(int));
  if (new_array.items == NULL) {
    printf("Error occured when allocating memory for line buffer\n");
    return new_array;
  }

  return new_array;
}

nestedarray parse_data(FILE *fp) {
  nestedarray buff = {
      .capacity = 4,
      .len = 0,
  }; // The buffer we will return containing array of arrays
  buff.items = malloc(buff.capacity * sizeof(array));
  if (buff.items == NULL) {
    printf("error occured when allocating memory for line buffer\n");
    return buff;
  }

  char cur_line[1024];
  while (fgets(cur_line, sizeof(cur_line), fp) != NULL) {
    array line_buf = new_array(4);
    char *token = strtok(cur_line, " ");
    while (token != NULL) {
      int cur_num = atoi(token);
      append(&line_buf, cur_num);
      token = strtok(NULL, " ");
    }

    // Check current size of buff, if not enough space resize
    if (buff.len == buff.capacity) {
      buff.capacity *= 2;
      array *new_buf = realloc(buff.items, sizeof(array) * buff.capacity);
      if (new_buf == NULL) {
        printf("failed to re-allocate memory for nested buffer\n");
        free(buff.items);
      }
      buff.items = new_buf;
    }

    // make copy to put into buff
    array line_buff_cpy;
    memcpy(&line_buff_cpy, &line_buf, sizeof(array));

    buff.items[buff.len++] = line_buff_cpy;
  }

  return buff;
}

void append(array *a, int new_item) {
  if (a->len == a->capacity) {
    resize_array(a);
  }

  a->items[a->len++] = new_item;
}

void resize_array(array *a) {
  a->capacity *= 2;

  int *newbuff = realloc(a->items, sizeof(int) * a->capacity);
  if (newbuff == NULL) {
    printf("failed to reallocate memory when resizing array.\n");
    free(a->items);
  }

  a->items = newbuff;
}

void free_nested_array(nestedarray a) {
  for (int i = 0; i < a.len; i++) {
    free(a.items[i].items);
  }
  free(a.items);
}
