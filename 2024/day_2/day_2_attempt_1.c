#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

  FILE *file = fopen("data.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  char buffer[255];
  char *token;
  int cur_num;
  int prev_num;
  int is_safe = 1; // 0 for false and 1 for true
  int safe_sum = 0;
  int is_increasing = 0; // -1 for decreasing, 1 for increasing
  int cur_count = 0; // Using this to check if we are on first number of loop
  int diff;

  // Parse file line by line
  while (fgets(buffer, sizeof(buffer), file) !=
         NULL) { // fgets() returns NULL when the end of the file is reached

    // Resetting variables
    is_safe = 1;
    is_increasing = 0;
    cur_count = 0;

    printf("current line: %s", buffer);
    token = strtok(buffer, " ");
    while (token != NULL) {
      cur_num = atoi(token);

      if (cur_count == 0) {
        prev_num = cur_num;
        cur_count++;
        token = strtok(NULL, " "); // Iterate to next part of string
        continue;
      }

      diff = prev_num - cur_num;

      if (is_increasing == 0) {
        if (diff == 0) {
          is_safe = 0;
          break;
        } else if (diff > 0) {
          is_increasing = -1;
        } else {
          is_increasing = 1;
        }
      }

      if (diff == 0) {
        is_safe = 0;
        break;
      }

      if (diff > 0 && is_increasing != -1) {
        is_safe = 0;
        break;
      }

      if (diff < 0 && is_increasing != 1) {
        is_safe = 0;
        break;
      }

      if (abs(diff) < 1 || abs(diff) > 3) {
        is_safe = 0;
        break;
      }

      prev_num = cur_num;
      token = strtok(NULL, " "); // Iterate to next part of string
    }

    // if is_safe is == 1, previous loop deemed it to be safe
    if (is_safe == 1) {
      safe_sum++;
    }
  }

  printf("Sum of safe levels: %d\n", safe_sum);
  fclose(file);

  return 0;
}
