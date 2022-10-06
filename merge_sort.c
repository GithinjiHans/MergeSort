#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int *IterSort(int *arr, int arr_len) {
  // we'll can use the quick sort algorithm
  int i = 0, j = arr_len - 1;
  int key = arr[0];
  if (arr_len > 1) {
    while (i < j) {
      for (; j > i; j--) {
        if (arr[j] < key) {
          arr[i] = arr[j];
          i++;
          break;
        }
      }
      for (; i < j; i++) {
        if (arr[i] > key) {
          arr[j] = arr[i];
          j--;
          break;
        }
      }
    }
    arr[i] = key;
    int arr1[i];
    int arr2[arr_len - i - 1];
    for (int k = 0; k < i; k++) {
      arr1[k] = arr[k];
    }
    for (int k = 0; k < arr_len - i - 1; k++) {
      arr2[k] = arr[k + i + 1];
    }
    int *arr1_sorted = IterSort(arr1, i);
    int *arr2_sorted = IterSort(arr2, arr_len - i - 1);
    for (int k = 0; k < i; k++) {
      arr[k] = arr1_sorted[k];
    }
    arr[i] = key;
    for (int k = 0; k < arr_len - i - 1; k++) {
      arr[k + i + 1] = arr2_sorted[k];
    }
  }
  return arr;
}

int *arrm;
// create a function to merge two sorted arrays
int *Merge(int *arr1, int arr1_len, int *arr2, int arr2_len) {
  int p = 0;
  int q = 0;
  int k = 0;
  int arr[arr1_len + arr2_len];
  while (p < arr1_len && q < arr2_len) {
    if (arr1[p] < arr2[q]) {
      arr[k++] = arr1[p++];
    } else {
      arr[k++] = arr2[q++];
    }
  }
  while (p < arr1_len) {
    arr[k++] = arr1[p++];
  }
  while (q < arr2_len) {
    arr[k++] = arr2[q++];
  }
  for (int i = 0; i < arr1_len + arr2_len; i++) {
    arrm[i] = arr[i];
  }
  return arrm;
}

int main(int argc, char *argv[]) {
  int *arr = (int *)malloc(sizeof(int));
  printf("%s", "Enter the array elements: ");
  int k = 0;
  // Input the array
  scanf("%d", &arr[0]);
  arr = (int *)realloc(arr, sizeof(int) * arr[0]);
  // create a new array to store the elementss to be sorted
  int *arr1 = (int *)malloc(sizeof(int) * arr[0]);
  for (int i = 1; i <= arr[0]; i++) {
    // input elements
    scanf("%d", &arr[i]);
    arr1[i - 1] = arr[i];
    k = i;
  }
  int sort_arr_len = k;
  int sort_arr_start = 1;
  int mid = sort_arr_len / 2;
  int *left_arr = (int *)malloc(sizeof(int) * mid);
  int *right_arr = (int *)malloc(sizeof(int) * (sort_arr_len - mid));
  for (int i = 0; i < mid; i++) {
    left_arr[i] = arr[i + 1];
  }
  for (int i = 0; i < (sort_arr_len - mid) + 1; i++) {
    right_arr[i] = arr[i + mid + 1];
  }
  // length of the left and right array
  int left_arr_len = mid;
  int right_arr_len = sort_arr_len - mid;
  int fd1[2];
  int fd2[2];
  // create the pipes
  pipe(fd1);
  pipe(fd2);
  // create the processes
  pid_t pid1 = fork();
  pid_t pid2 = fork();
  if (pid1 == 0) {
    close(fd1[0]);
    // child process
    // sort the left array
    int *left_arr_sorted = IterSort(left_arr, left_arr_len);
    // write
    write(fd1[1], left_arr_sorted, sizeof(int) * left_arr_len);
    close(fd1[1]);
  } else if (pid2 == 0) {
    close(fd2[0]);
    // child process
    int *right_arr_sorted = IterSort(right_arr, right_arr_len);
    // write the sorted array to the pipe
    write(fd2[1], right_arr_sorted, sizeof(int) * right_arr_len);
    close(fd2[1]);
  } else {
    close(fd1[1]);
    close(fd2[1]);
    // parent process
    // wait for the child processes to finish
    wait(NULL);
    wait(NULL);
    // read the sorted arrays from the pipes
    int *left_arr_sorted = (int *)malloc(sizeof(int) * left_arr_len);
    int *right_arr_sorted = (int *)malloc(sizeof(int) * right_arr_len);
    read(fd1[0], left_arr_sorted, sizeof(int) * left_arr_len);
    read(fd2[0], right_arr_sorted, sizeof(int) * right_arr_len);
    arrm = (int *)malloc(sizeof(int) * (left_arr_len + right_arr_len));
    // merge the two sorted arrays
    int *arr_sorted =
        Merge(left_arr_sorted, left_arr_len, right_arr_sorted, right_arr_len);
    printf("%s", "Sorted array: ");
    for (int i = 0; i < left_arr_len + right_arr_len; i++) {
      printf("%d ", arr_sorted[i]);
    }
    printf("%s", "\n");
    // free the memory
    free(arr_sorted);
  }
  // free the memory
  free(arr);
  free(arr1);
  free(left_arr);
  free(right_arr);
  return 0;
}
