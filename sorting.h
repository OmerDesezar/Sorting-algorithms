#ifndef __SORTING_H__
#define __SORTING_H__

#include <stddef.h> /* size_t */

typedef int (*cmp_func)(const void *, const void *);

void BubbleSort(int *arr, size_t size);

void SelectionSort(int *arr, size_t size);

void InsertionSort(int *arr, size_t size);

void HeapSort(int *arr, size_t size);

int CountingSort(int *arr, size_t size);

int RadixSort(int *arr, size_t size);

int MergeSort(int *arr_to_sort, size_t num_elements);

int BinarySearchIterative(int *find_in, size_t size, int find);

int BinarySearchRecursion(int *find_in, size_t size, int find);

void QSort(void *base, size_t num_elements, size_t size, cmp_func cmp);

#endif /* __SORTING_H__ */
