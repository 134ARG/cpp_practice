#ifndef SORT_H
#define SORT_H

#include <iostream>

void merge(int array[], int n, int array1[], int n1, int array2[], int n2)
{
    int i = 0, l = 0;
    for(int j = 0; j < n; j++)
        if((array1[l] < array2[i] || i >= n2) && l < n1) {
            array[j] = array1[l++];
        } else {
            array[j] = array2[i++];
        }
}

void sort(int array[], int n)
{
    if (n > 1) {
        int n1 = n / 2;
        int n2 = n - n1;
        int array1[n1];
        int array2[n2];

        std::copy(array, array + n1, array1);
        std::copy(array + n1, array + n, array2);

        sort(array1, n1);
        sort(array2, n2);
        merge(array, n, array1, n1, array2, n2);
    }


}

void printArray(int array[], int n)
{
   std::cout << "{ ";
   for (int i = 0; i < n; i++)
   {
      if (i > 0) std::cout << ", ";
      std::cout << array[i];
   }
   std::cout << " }" << std::endl;
}




#endif // SORT_H
