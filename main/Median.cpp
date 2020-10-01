#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <vector>

using namespace std;

double QuickSortOnce(double a[], int low, int high) {
    double pivot = a[low];
    int i = low, j = high;
    while (i < j) {

        while (a[j] >= pivot && i < j) {
            j--;
        }

        a[i] = a[j];

        while (a[i] <= pivot && i < j) {
            i++;
        }

        a[j] = a[i];
    }
    a[i] = pivot;

    return i;
}

void QuickSort(double a[], int low, int high) {
    if (low >= high) {
        return;
    }

    double pivot = QuickSortOnce(a, low, high);

    QuickSort(a, low, pivot - 1);

    QuickSort(a, pivot + 1, high);
}

double EvaluateMedian(double a[], int n) {
    QuickSort(a, 0, n - 1);

    if (n % 2 != 0) {
        return a[n / 2];
    } else {
        return (a[n / 2] + a[n / 2 - 1]) / 2;
    }
}

