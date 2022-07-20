/*=========================== LIBRARIES & MACROS ============================*/

#include <stdlib.h> /* calloc */

#include "sorting.h"

#define SUCCESS 0
#define ERROR -1
#define NUM_OF_DIGITS 10

/*============================== DECLARATIONS ===============================*/

static int FindMin(int *, size_t);
static int FindMax(int *, size_t);
static void SwapInts(int *, int *);
static int MergeArr(int *, int, int);
static void CopyArr(int *, int *, size_t);
static void CountToIndex(int *, size_t);
static int MergeSortRec(int *, int, int);
static int BinarySearchRec(int, int, int, int *);
static int CountingSortDigits(int *, size_t, int);
static void SwapElements(char *, size_t, int, int);
static int partition(void *, size_t, int, int, cmp_func);
static void QSortRec(void *, size_t, int, int, cmp_func);
static void HeapifyDownRec(int *, size_t, size_t);

/*=========================== FUNCTION DEFINITION ===========================*/

/* reviewed by Tzach */
void BubbleSort(int *arr, size_t size)
{
	size_t i = 0;
	size_t j = 0;
	for (i = 0; i < size - 1; ++i)
	{
		for (j = 0; j < size - i - 1; ++j)
		{
			if (arr[j] > arr[j + 1])
			{
				SwapInts(&arr[j], &arr[j + 1]);
			}
		}
	}
}

void SelectionSort(int *arr, size_t size)
{
	size_t i = 0;
	size_t j = 0;
	size_t min_index = 0;
	for (i = 0; i < size - 1; ++i)
	{
		min_index = i;
		for (j = i + 1; j < size; ++j)
		{
			if (arr[j] < arr[min_index])
			{
				min_index = j;
			}
		}
		SwapInts(&arr[min_index], &arr[i]);
	}
}

void InsertionSort(int *arr, size_t size)
{
	size_t i = 0;
	int j = 0;
	int key = 0;
	for (i = 1; i < size; ++i)
	{
		key = arr[i];
		j = i - 1;
		while (arr[j] > key && 0 <= j)
		{
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
	}
}

int CountingSort(int *arr, size_t size)
{
	size_t i = 0;
	int *output_arr = NULL;
	int *count_arr = NULL;
	int max = FindMax(arr, size);
	int min = FindMin(arr, size);
	size_t range = max - min + 1;

	output_arr = (int *)calloc(size, sizeof(int));
	if (NULL == output_arr)
	{
		return (-1);
	}
	count_arr = (int *)calloc(range, sizeof(int));
	if (NULL == count_arr)
	{
		return (-1);
	}

	for (i = 0; i < size; ++i)
	{
		++count_arr[arr[i] - min];
	}

	CountToIndex(count_arr, range);

	for (i = 0; i < size; ++i)
	{
		output_arr[count_arr[arr[i] - min]] = arr[i];
		++count_arr[arr[i] - min];
	}

	CopyArr(arr, output_arr, size);

	free(output_arr);
	free(count_arr);

	return (0);
}

int RadixSort(int *arr, size_t size)
{
	int base = 1;
	int rtn = 0;
	int max = FindMax(arr, size);

	while (0 < max / base && 0 == rtn)
	{
		rtn = CountingSortDigits(arr, size, base);
		base *= 10;
	}

	return (rtn);
}

void HeapSort(int *arr, size_t size)
{
	int i = (size / 2) - 1;
	for (; i >= 0; --i)
	{
		HeapifyDownRec(arr, i, size);
	}
	i = size - 1;
	for (; i >= 0; --i)
	{
		SwapInts(&arr[0], &arr[i]);
		HeapifyDownRec(arr, 0, i);
	}
}

int BinarySearchIterative(int *find_in, size_t size, int find)
{
	int start = 0, middle = 0;
	int end = size - 1;
	do
	{
		middle = (start + end) / 2;
		(find_in[middle] > find) ? (end = (middle - 1)) : (start = (middle + 1));
	} while (find_in[middle] != find && (start <= end));

	return ((find_in[middle] == find) ? middle : (ERROR));
}

int BinarySearchRecursion(int *find_in, size_t size, int find)
{
	return (BinarySearchRec(0, size - 1, find, find_in));
}

int MergeSort(int *arr_to_sort, size_t size)
{
	return (MergeSortRec(arr_to_sort, 0, size - 1));
}

void QSort(void *base, size_t num_elements, size_t element_size, cmp_func cmp)
{
	QSortRec(base, element_size, 0, num_elements - 1, cmp);
}

static int CountingSortDigits(int *arr, size_t size, int index)
{
	size_t i = 0;
	int count_arr[NUM_OF_DIGITS] = {0};
	int *output_arr = (int *)calloc(size, sizeof(int));

	if (NULL == output_arr)
	{
		return (-1);
	}

	for (i = 0; i < size; ++i)
	{
		++count_arr[(arr[i] / index) % 10];
	}

	CountToIndex(count_arr, NUM_OF_DIGITS);

	for (i = 0; i < size; ++i)
	{
		output_arr[count_arr[(arr[i] / index) % 10]] = arr[i];
		++count_arr[(arr[i] / index) % 10];
	}

	CopyArr(arr, output_arr, size);

	free(output_arr);

	return (0);
}

static void CountToIndex(int *arr, size_t num)
{
	size_t i = 1;
	for (; i < num; ++i)
	{
		arr[i] += arr[i - 1];
	}
	for (i = (num - 1); i > 0; --i)
	{
		arr[i] = arr[i - 1];
	}
	arr[0] = 0;
}

static int BinarySearchRec(int start, int end, int find, int *arr)
{
	int middle = (start + end) / 2;
	return (
		(end < start) ? (ERROR) : (arr[middle] == find) ? (middle)
							  : (arr[middle] > find)	? (BinarySearchRec(start, middle - 1, find, arr))
														: (BinarySearchRec(middle + 1, end, find, arr)));
}

static int MergeSortRec(int *arr, int start, int end)
{
	if (start < end)
	{
		int middle = (start + end) / 2;
		return (
			(MergeSortRec(arr, start, middle) == ERROR) ? ERROR : (MergeSortRec(arr, middle + 1, end) == ERROR) ? ERROR
																												: (MergeArr(arr, start, end)));
	}
	return (SUCCESS);
}

static int MergeArr(int *arr, int start, int end)
{
	int i = 0, j = 0, k = start;
	int middle = (start + end) / 2;
	int left_size = middle - start + 1;
	int right_size = end - middle;
	int *left_arr = (int *)malloc(sizeof(int) * left_size);
	int *right_arr = NULL;
	if (NULL == left_arr)
	{
		return (ERROR);
	}
	right_arr = (int *)malloc(sizeof(int) * right_size);
	if (NULL == right_arr)
	{
		free(left_arr);
		return (ERROR);
	}

	CopyArr(left_arr, (arr + start), left_size);
	CopyArr(right_arr, (arr + middle + 1), right_size);

	while (i < left_size && j < right_size)
	{
		if (left_arr[i] <= right_arr[j])
		{
			arr[k] = left_arr[i];
			++i;
		}
		else
		{
			arr[k] = right_arr[j];
			++j;
		}
		++k;
	}

	while (i < left_size)
	{
		arr[k] = left_arr[i];
		++i;
		++k;
	}

	while (j < right_size)
	{
		arr[k] = right_arr[j];
		++j;
		++k;
	}
	free(left_arr);
	free(right_arr);
	return (SUCCESS);
}

static void QSortRec(void *arr, size_t element_size, int start, int end, cmp_func cmp)
{
	if (start < end)
	{
		int index = partition(arr, element_size, start, end, cmp);
		QSortRec(arr, element_size, start, index - 1, cmp);
		QSortRec(arr, element_size, index + 1, end, cmp);
	}
}

static int partition(void *arr, size_t element_size, int start, int end, cmp_func cmp_func)
{
	int i = start, j = start;
	char *arr_runner = (char *)arr;
	for (; j < end; ++j)
	{
		if (0 >= cmp_func(&arr_runner[j * element_size], &arr_runner[end * element_size]))
		{
			SwapElements(arr_runner, element_size, (i * element_size), (j * element_size));
			++i;
		}
	}
	SwapElements(arr_runner, element_size, (i * element_size), (end * element_size));
	return (i);
}

static void SwapElements(char *arr, size_t element_size, int a, int b)
{
	size_t i = 0;
	char temp = ' ';
	for (; i < element_size; ++i)
	{
		temp = arr[a + i];
		arr[a + i] = arr[b + i];
		arr[b + i] = temp;
	}
}

static void CopyArr(int *dest_arr, int *src_arr, size_t num)
{
	size_t i = 0;
	for (; i < num; ++i)
	{
		dest_arr[i] = src_arr[i];
	}
}

static int FindMax(int *arr, size_t size)
{
	size_t i = 1;
	int max = arr[0];
	for (i = 1; i < size; ++i)
	{
		if (arr[i] > max)
		{
			max = arr[i];
		}
	}
	return (max);
}

static int FindMin(int *arr, size_t size)
{
	size_t i = 1;
	int min = arr[0];
	for (i = 1; i < size; ++i)
	{
		if (arr[i] < min)
		{
			min = arr[i];
		}
	}
	return (min);
}

static void SwapInts(int *num1, int *num2)
{
	int temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}

static void HeapifyDownRec(int *arr, size_t index, size_t size)
{
	size_t l_child_index = (2 * index) + 1;
	size_t r_child_index = (2 * index) + 2;
	size_t biggest = index;
	if (l_child_index < size && arr[l_child_index] > arr[biggest])
	{
		biggest = l_child_index;
	}
	if (r_child_index < size && arr[r_child_index] > arr[biggest])
	{
		biggest = r_child_index;
	}
	if (biggest != index)
	{
		SwapInts(&arr[index], &arr[biggest]);
		HeapifyDownRec(arr, biggest, size);
	}
}