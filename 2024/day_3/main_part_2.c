#include <errno.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Advent of Code 2024 Day 3 Part 2
 * 83595109 <- Solution! My issue was treating each line as its own
 * run, not the entire file as the input. The case I was missing was
 * if the last command of the previous line was a "don't" it wouldnt
 * carry over to the next line.
 */

int main(void) {
  FILE *fp;

  fp = fopen("./data.txt", "r");
  if (fp == NULL) {
    perror("error opening file");
    printf("Error code: %d\n", errno);
    return 1;
  }

  const char *pattern = "(do\\(\\))|(don't\\(\\))|mul\\(([[:digit:]]{1,3}),(["
                        "[:digit:]]{1,3})\\)";
  regex_t rgx;

  int success = regcomp(&rgx, pattern, REG_EXTENDED);
  if (success == 0) {
    printf("RegEx compiled successfully\n");
  } else {
    printf("RegEx failed to compile\n");
    return 1;
  }

  int running_sum = 0;
  int run_command = 1;
  char s[8096];

  while (fgets(s, sizeof(s), fp) != NULL) {
    printf("\n<----- Line to search for matches in ----->: \n%s\n", s);

    int max_matches = 200;
    int max_groups = 5;

    regmatch_t groups[max_groups];
    char *cur_pos = s;

    printf("--- Matches ---\n");
    int num_matches = 0;
    int offset = 0;
    int x_int = -1;
    int y_int = -1;
    for (int m = 0; m < max_matches; m++) {
      if (regexec(&rgx, cur_pos, max_groups, groups, 0) == REG_NOMATCH) {
        break;
      }

      for (int i = 0; i < max_groups; i++) {

        if (groups[i].rm_so == -1 || groups[i].rm_eo == -1) {
          continue;
        }

        int characters_in_match = groups[i].rm_eo - groups[i].rm_so;

        switch (i) {
        case 0: {
          offset = groups[i].rm_eo;
          break;
        }
        case 1: {
          run_command = 1;
          break;
        }
        case 2: {
          run_command = 0;
          break;
        }
        case 3: {
          char x[characters_in_match + 1];
          strncpy(x, cur_pos + groups[i].rm_so, characters_in_match);
          x[characters_in_match] = '\0';
          x_int = atoi(x);
          break;
        }
        case 4: {
          char y[characters_in_match + 1];
          strncpy(y, cur_pos + groups[i].rm_so, characters_in_match);
          y[characters_in_match] = '\0';
          y_int = atoi(y);
          break;
        }
        default: {
          break;
        }
        }

        if (run_command && x_int != -1 && y_int != -1 && i == 4) {
          printf(" multiplying %d * %d since command is %d", x_int, y_int,
                 run_command);

          running_sum += (x_int * y_int);
        }
      }
      printf("\n");
      cur_pos += offset;
      num_matches++;
    }
    printf("\nNumber of matches: %d\n", num_matches);
  }

  printf("Final Sum: %d\n", running_sum);
  // Cleanup
  fclose(fp);
  regfree(&rgx);

  return 0;
}
