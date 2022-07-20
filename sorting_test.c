#include <stdio.h>	/* printf */
#include <stdlib.h> /* random */
#include <time.h>	/* time */

#include "sorting.h"

#define ARR_SIZE 10000
#define ARR_RANGE 5000
#define TESTS_PER_SORT 10

typedef void (*sort_t)(int *, size_t);
typedef int (*sort_malloc_t)(int *, size_t);

static void TestAllFuncs();
static void TestBubble();
static void TestInsertion();
static void TestSelection();
static void TestHeap();
static void TestCounting();
static void TestRadix();
static void TestLibQSort();
static void TestBinarySearch();
static void TestMerge();
static void TestQSort();
static clock_t TestSort(sort_t);
static clock_t TestMallocSort(sort_malloc_t);
static int IsSorted(int *, size_t);
static void RandomizeArr(int *, size_t);
static int IntCompare(const void *, const void *);

int main()
{
	TestAllFuncs();
	return (0);
}

static void TestAllFuncs()
{
	srand(time(NULL));
	printf("~ Size of array sorted: %d ~\n", ARR_SIZE);
	printf("~ Number of test ran: %d ~\n", TESTS_PER_SORT);

	TestBubble();
	TestInsertion();
	TestSelection();
	TestHeap();
	TestCounting();
	TestRadix();
	TestMerge();
	TestQSort();
	TestLibQSort();
	TestBinarySearch();
}

static void TestBubble()
{
	clock_t avg_time = TestSort(BubbleSort);
	if (-1 != avg_time)
	{
		printf("BubbleSort working!                                  V\n");
		printf(">>>  BubbleSort avg time:    %ldms\n", avg_time);
	}
	else
	{
		printf("BubbleSort NOT working!                              X\n");
	}
}

static void TestInsertion()
{
	clock_t avg_time = TestSort(InsertionSort);
	if (-1 != avg_time)
	{
		printf("InsertionSort working!                               V\n");
		printf(">>>  InsertionSort avg time: %ldms\n", avg_time);
	}
	else
	{
		printf("InsertionSort NOT working!                           X\n");
	}
}

static void TestSelection()
{
	clock_t avg_time = TestSort(SelectionSort);
	if (-1 != avg_time)
	{
		printf("SelectionSort working!                               V\n");
		printf(">>>  SelectionSort avg time: %ldms\n", avg_time);
	}
	else
	{
		printf("SelectionSort NOT working!                           X\n");
	}
}

static void TestCounting()
{
	clock_t avg_time = TestMallocSort(CountingSort);
	if (-1 != avg_time)
	{
		printf("CountingSort working!                                V\n");
		printf(">>>  CountingSort avg time:  %ldms\n", avg_time);
	}
	else
	{
		printf("CountingSort NOT working!                            X\n");
	}
}

static void TestRadix()
{
	clock_t avg_time = TestMallocSort(RadixSort);
	if (-1 != avg_time)
	{
		printf("RadixSort working!                                   V\n");
		printf(">>>  RadixSort avg time:     %ldms\n", avg_time);
	}
	else
	{
		printf("RadixSort NOT working!                               X\n");
	}
}

static void TestHeap()
{
	clock_t avg_time = TestSort(HeapSort);
	if (-1 != avg_time)
	{
		printf("HeapSort working!                                    V\n");
		printf(">>>  HeapSort avg time:      %ldms\n", avg_time);
	}
	else
	{
		printf("HeapSort NOT working!                                X\n");
	}
}

static void TestBinarySearch()
{
	int arr[100] = {0};
	int to_find = 99;
	int to_not_find = 111;
	int found_index = 0;
	int not_found_index = 0;
	int i = 0;
	for (; i < 100; ++i)
	{
		arr[i] = i;
	}
	found_index = BinarySearchIterative(arr, 100, to_find);
	not_found_index = BinarySearchIterative(arr, 100, to_not_find);
	if (99 == found_index && -1 == not_found_index)
	{
		printf("BinarySearchIterative working!                       V\n");
	}
	else
	{
		printf("BinarySearchIterative NOT working!                   X\n");
	}
	found_index = BinarySearchRecursion(arr, 100, to_find);
	not_found_index = BinarySearchRecursion(arr, 100, to_not_find);
	if (99 == found_index && -1 == not_found_index)
	{
		printf("BinarySearchRecursion working!                       V\n");
	}
	else
	{
		printf("BinarySearchRecursion NOT working!                   X\n");
	}
}

static void TestMerge()
{
	clock_t avg_time = TestMallocSort(MergeSort);
	if (-1 != avg_time)
	{
		printf("MergeSort working!                                   V\n");
		printf(">>>  MergeSort avg time:     %ldms\n", avg_time);
	}
	else
	{
		printf("MergeSort NOT working!                               X\n");
	}
}

static void TestQSort()
{
	int i = 0;
	int is_sorted = 1;
	int arr[ARR_SIZE] = {0};
	clock_t start = 0;
	clock_t end = 0;
	clock_t total = 0;

	while (++i < TESTS_PER_SORT)
	{
		RandomizeArr(arr, ARR_SIZE);
		start = clock();
		QSort(arr, ARR_SIZE, sizeof(int), IntCompare);
		end = clock();
		total += (end - start) / (CLOCKS_PER_SEC / 1000);
		is_sorted = IsSorted(arr, ARR_SIZE);
	}
	if (is_sorted)
	{
		printf("QuickSort working!                                   V\n");
		printf(">>>  QuickSort avg time:     %ldms\n", (total / TESTS_PER_SORT));
	}
	else
	{
		printf("QuickSort NOT working!                               X\n");
	}
}

static void TestLibQSort()
{
	int i = 0;
	int arr[ARR_SIZE] = {0};
	clock_t start = 0;
	clock_t end = 0;
	clock_t total = 0;

	while (++i < TESTS_PER_SORT)
	{
		RandomizeArr(arr, ARR_SIZE);
		start = clock();
		qsort(arr, ARR_SIZE, sizeof(int), IntCompare);
		end = clock();
		total += (end - start) / (CLOCKS_PER_SEC / 1000);
	}
	printf("\n>>>  LibQSort avg time:      %ldms\n", (total / TESTS_PER_SORT));
}

static clock_t TestSort(sort_t sort)
{
	int i = 0;
	int is_sorted = 1;
	int arr[ARR_SIZE] = {0};
	clock_t start = 0;
	clock_t end = 0;
	clock_t total = 0;

	while (++i < TESTS_PER_SORT && is_sorted)
	{
		RandomizeArr(arr, ARR_SIZE);
		start = clock();
		sort(arr, ARR_SIZE);
		end = clock();
		total += (end - start) / (CLOCKS_PER_SEC / 1000);
		is_sorted = IsSorted(arr, ARR_SIZE);
	}

	if (is_sorted)
	{
		return (total / TESTS_PER_SORT);
	}
	else
	{
		return (-1);
	}
}

static clock_t TestMallocSort(sort_malloc_t sort)
{
	int i = 0;
	int is_sorted = 1;
	int arr[ARR_SIZE] = {0};
	clock_t start = 0;
	clock_t end = 0;
	clock_t total = 0;

	while (++i <= TESTS_PER_SORT && is_sorted)
	{
		RandomizeArr(arr, ARR_SIZE);
		start = clock();
		sort(arr, ARR_SIZE);
		end = clock();
		total += (end - start) / (CLOCKS_PER_SEC / 1000);
		is_sorted = IsSorted(arr, ARR_SIZE);
	}

	if (is_sorted)
	{
		return (total / TESTS_PER_SORT);
	}
	else
	{
		return (-1);
	}
}

static int IsSorted(int *arr, size_t size)
{
	size_t i = 0;
	int sorted_flag = 1;
	for (i = 0; i < size - 1; i++)
	{
		if (arr[i] > arr[i + 1])
		{
			sorted_flag = 0;
			break;
		}
	}
	return (sorted_flag);
}

static void RandomizeArr(int *arr, size_t size)
{
	size_t i = 0;
	for (i = 0; i < size; ++i)
	{
		arr[i] = rand() % ARR_RANGE;
	}
}

static int IntCompare(const void *num1, const void *num2)
{
	return (*(int *)num1 - *(int *)num2);
}
