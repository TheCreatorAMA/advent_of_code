#include "utils.h"
#include <stdio.h>

int main(void) {
  FILE *fp = fopen("/home/alika/Code/advent_of_code/2024/day_2/data.txt", "r");
  if (fp == NULL) {
    printf("error opening data file\n");
    return 1;
  }

  nestedarray data = parse_data(fp);
  if (data.items == NULL) {
    printf("error occured loading data\n");
    return 1;
  }

  printf("[\n");
  for (int i = 0; i < data.len; i++) {
    printf("   [ ");
    for (int j = 0; j < data.items[i].len; j++) {
      printf("%d ", data.items[i].items[j]);
    }
    printf("]\n");
  }
  printf("]\n");

  free_nested_array(data);
  return 0;
}
