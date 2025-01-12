#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void print_nested_array(nestedarray a);
int part_one(nestedarray data);
int part_two(nestedarray data);
int check_if_uniform(array a);
int check_diff(array a);
array copy_exclude(array a, int exclude_index);

int main(void) {
  FILE *fp = fopen("./data.txt", "r");
  if (fp == NULL) {
    printf("error opening data file\n");
    return 1;
  }

  nestedarray data = parse_data(fp);
  if (data.items == NULL) {
    printf("error occured loading data\n");
    return 1;
  }

  // print_nested_array(data);
  int p1_result = part_one(data);
  printf("result for part 1: %d\n", p1_result);

  int p2_result = part_two(data);
  printf("result for part 2: %d\n", p2_result);

  free_nested_array(data);
  return 0;
}

void print_nested_array(nestedarray a) {
  printf("[\n");
  for (int i = 0; i < a.len; i++) {
    printf("   [ ");
    for (int j = 0; j < a.items[i].len; j++) {
      printf("%d ", a.items[i].items[j]);
    }
    printf("]\n");
  }
  printf("]\n");
}

int part_one(nestedarray data) {
  int count = 0;
  for (int i = 0; i < data.len; i++) {
    if (check_if_uniform(data.items[i]) && check_diff(data.items[i])) {
      count++;
    }
  }

  return count;
}

// Brute force lol
int part_two(nestedarray data) {
  int count = 0;
  array check_again_array;
  for (int i = 0; i < data.len; i++) {
    if (check_if_uniform(data.items[i]) && check_diff(data.items[i])) {
      count++;
    } else {
      // Check every combination of array.....
      // Got stuck on logic checks I could do in check_if_uniform
      for (int j = 0; j < data.items[i].len; j++) {
        check_again_array = copy_exclude(data.items[i], j);
        if (check_if_uniform(check_again_array) &&
            check_diff(check_again_array)) {
          count++;
          break;
        }
      }
    }
  }

  return count;
}

// Check if all values are increasing or decreasing
int check_if_uniform(array a) {
  // First see if pattern is increasing or decreasing
  // below makes sure there is at least two elements
  if (a.len < 2) {
    return 0;
  }

  char uniform_trend; // 'i' for increasing, 'd' for decreasing
  int diff = a.items[0] - a.items[1];

  if (diff == 0) {
    return 0;
  } else if (diff > 0) {
    uniform_trend = 'd';
  } else {
    uniform_trend = 'i';
  }

  for (int i = 0; i < a.len - 1; i++) {
    diff = a.items[i] - a.items[i + 1];
    if ((uniform_trend == 'd' && diff < 0) ||
        (uniform_trend == 'i' && diff > 0)) {
      return 0;
    }
  }

  return 1;
}

// Check each adjacent element to make sure they differ
// by at least one and at most three.
int check_diff(array a) {
  int diff;

  for (int i = 0; i < a.len - 1; i++) {
    diff = abs(a.items[i] - a.items[i + 1]);

    if (diff < 1 || diff > 3) {
      return 0;
    }
  }

  return 1;
}

array copy_exclude(array a, int exclude_index) {
  array new_array = {
      .capacity = a.capacity,
      .len = a.len - 1,
  };

  int new_items[a.len - 1];
  int new_items_index = 0;
  for (int i = 0; i < a.len; i++) {
    if (i != exclude_index) {
      new_items[new_items_index++] = a.items[i];
    }
  }

  new_array.items = new_items;

  return new_array;
}
