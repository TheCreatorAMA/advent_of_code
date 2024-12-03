#include <stdio.h>
#include <stdlib.h>

/*
PART 1:
-------
1. Input is going to be two lists:
                        -----
                        3   4
                        4   3
                        2   5
                        1   3
                        3   9
                        3   3
                        -----
2. Need to sort this list from smallest to largest
3. Subtract each pair of values in list and get the difference
        abs(a[0] - b[0]) = distance
4. Sum up list of distances

Assumptions:
------------
1. Both list are equal length
2. Distances are never negative

PART 2:
-------
1. Use the sorted locations list and pick a value from locationsA
2. Use value LocationsA[i] and see how many times it occurs in locationsB
3. Calculate similarity score by doing locationsA[i] * num_occur_locB
4. Add similarity score of locationsA[i] to running sum
*/

#define MAX_LINES 1001

void bubbleSort(int arr[], int n);
void readData(const char *filename, int locationsA[], int locationsB[],
              int *size);

int main() {
  int locationsA[MAX_LINES], locationsB[MAX_LINES];
  int diff, sum, size = 0;

  // START: Part 1
  readData("data.txt", locationsA, locationsB, &size);

  bubbleSort(locationsA, size);
  bubbleSort(locationsB, size);

  // printf("Sorted Locations: ");
  for (int i = 0; i < size; i++) {
    diff = abs(locationsA[i] - locationsB[i]);
    // printf("\nA[%d] = %d B[%d] = %d ---> diff = %d", i, locationsA[i], i,
    //        locationsB[i], diff);
    sum += diff;
  }

  printf("\nLocations sum: %d", sum);
  // END: Part 1
  // Result: 2742123

  // START: Part 2
  // PERF: We could make this faster by caching values that have already had
  // their similarity_score found (if the left list has duplicates)
  int cur_location, occurances, similarity_score, similarity_sum;

  for (int k = 0; k < size; k++) {
    cur_location = locationsA[k];
    occurances = 0;
    similarity_score = 0;
    for (int l = 0; l < size; l++) {
      if (cur_location == locationsB[l]) {
        occurances++;
      }
    }
    similarity_score = cur_location * occurances;
    // printf("\nCur Location: %d * occurances: %d = similarity_score: %d",
    //        cur_location, occurances, similarity_score);
    similarity_sum += similarity_score;
  }

  printf("\nSimilarity sum: %d\n", similarity_sum);
  // END: Part 2
  // Result: 21328497

  return 0;
}

void bubbleSort(int arr[], int n) {
  int i, j, temp;

  for (i = 0; i < n - 1; i++) {
    for (j = 0; j < n - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

void readData(const char *filename, int locationsA[], int locationsB[],
              int *size) {
  FILE *file;
  int index = 0;

  file = fopen("data.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }

  while (fscanf(file, "%d %d", &locationsA[index], &locationsB[index]) == 2) {
    index++;
    if (index >= MAX_LINES) {
      fprintf(stderr, "Error: Too many lines in file (max %d).\n", MAX_LINES);
      fclose(file);
      return;
    }
  }

  fclose(file);
  *size = index;
}
