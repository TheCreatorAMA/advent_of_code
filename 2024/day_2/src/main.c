#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void print_nested_array(nestedarray a);
int part_one(nestedarray data);
int check_if_uniform(array a);
int check_diff(array a);

int main(void) {
  FILE *fp = fopen("/home/alex/Code/advent_of_code/2024/day_2/data.txt", "r");
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
    if (uniform_trend == 'd' && diff < 0) {
      return 0;
    }

    if (uniform_trend == 'i' && diff > 0) {
      return 0;
    }
  }

  return 1;
}

// Check each adjacent element to make sure they differ
// by at least one and at most three
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
