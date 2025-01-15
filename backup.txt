#include <errno.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Advent of Code 2024 Day 3 Part 1
 */

int main(void) {
  FILE *fp;

  fp = fopen("./data.txt", "r");
  if (fp == NULL) {
    perror("error opening file");
    printf("Error code: %d\n", errno);
    return 1;
  }

  const char *pattern = "mul\\(([[:digit:]]{1,3}),([[:digit:]]{1,3})\\)";
  regex_t rgx;

  int success = regcomp(&rgx, pattern, REG_EXTENDED);
  if (success == 0) {
    printf("RegEx compiled successfully\n");
  } else {
    printf("RegEx failed to compile\n");
    return 1;
  }

  long int running_sum = 0;
  char s[8096]; // Buffer to read lines from file into, should be big enough to
                // hold the longest line in your file.

  // In my case I have a file containing lines I want to use my regex on
  while (fgets(s, sizeof(s), fp) != NULL) {
    printf("\n<----- Line to search for matches in ----->: \n%s\n", s);

    int max_matches =
        200; // How many matches we estimate to find in our input string.
             // You can check to see if you are done finding matches using the
             // return value of regexec. Just ballpark this number.
    int max_groups = 3; // This should be set to the max number of groups in
                        // your regex (a group is anything in '()' in your
                        // regex) in my case I have 3. One group for matching
                        // the full pattern im looking for 'mul(x,y)', two other
                        // groups to match 'x' and 'y' in 'mul(x,y)'

    regmatch_t
        groups[max_groups]; // Array to hold the match information for each
                            // group. Each group match is a struct with two
                            // fields rm_so and rm_eo which are the start and
                            // end indices of the match in the string.
    char *cur_pos =
        s; // Pointer to keep track of the current position in the string

    printf("--- Matches ---\n");
    int num_matches = 0;
    for (int m = 0; m < max_matches; m++) {
      // regexec returns 0 if a match is found, REG_NOMATCH if no match is
      // found. Break no more matches are found. We use are cur_pos in the
      // string to find the next match.
      if (regexec(&rgx, cur_pos, max_groups, groups, 0) == REG_NOMATCH) {
        break;
      }

      int offset = 0; // to keep track of the offset of the match
      int x_int;      // to store the value of x before converting it
      int y_int;      // to store the value of y before converting it
      for (int i = 0; i < max_groups; i++) {

        // We find the difference between the start and end index of the match
        // to get the number of characters in the match
        unsigned int characters_in_match = groups[i].rm_eo - groups[i].rm_so;
        // Create a string to hold the match

        switch (i) {
        case 0: { // Case to handler first matching group (e.g. any match that
                  // fits the pattern 'mul(x,y)')
          offset =
              groups[i]
                  .rm_eo; // Set the offset to the first groups end
                          // index. We do this because the first group (e.g.
                          // 'mul(x,y)') is what actually matches the whole
                          // pattern we are looking for. The sub groups are what
                          // matches the 'x' and 'y' within our whole match.
          break;
        }
        case 1: { // Case to handle second matching group (e.g. any match that
                  // is the 'x' in the pattern 'mul(x,y)')
          char x[characters_in_match + 1];
          // Copy the matched subsection found by our current group of the
          // primary string we are searching into the result string
          strncpy(x, cur_pos + groups[i].rm_so, characters_in_match);
          x_int = atoi(x);
          break;
        }
        case 2: { // Case to handle third matching group (e.g. any match that
                  // is the 'y' in the pattern 'mul(x,y)')
          char y[characters_in_match + 1];
          // Copy the matched subsection found by our current group of the
          // primary string we are searching into the result string
          strncpy(y, cur_pos + groups[i].rm_so, characters_in_match);
          y_int = atoi(y);
          break;
        }
        default: {
          break;
        }
        }
      }
      // Update our current position in the string to the end of the last match.
      // Now when we call regexec again it will start searching from the end of
      // the last match.
      cur_pos +=
          offset; // Set current position in string to end index of last match
      num_matches++;

      // Specific to my use case, multiply x & y then add it to the running sum
      running_sum += (x_int * y_int);
    }
    printf("\nNumber of matches: %d\n", num_matches);
  }

  printf("Final Sum: %ld\n", running_sum);
  // Cleanup
  fclose(fp);
  regfree(&rgx);

  return 0;
}
