// merge sort by seokgukim
#include <iostream>

void merge_sort(int *arr, int s, int e) {
    if (s + 1 == e) return;
    
    int m = s + e >> 1;
    merge_sort(arr, s, m);
    merge_sort(arr, m, e);
    
    int idx = 0, idx_f = s, idx_b = m;
    int *tmp = new int[e - s];
    while (idx < e - s) {
        if (idx_f >= m || (idx_b < e && arr[idx_f] > arr[idx_b])) tmp[idx++] = arr[idx_b++];
        else tmp[idx++] = arr[idx_f++];
    }

    for (int i = s; i < e; i++) {
        arr[i] = tmp[i - s];
    }

    free(tmp);
}

// int main() {
//     int arr[9] = {6,1,34,6,1,23,55,43,32};
//     merge_sort(arr, 0, 9);

//     for (int i = 0; i < 9; i++) std::cout << arr[i] << " ";
// }