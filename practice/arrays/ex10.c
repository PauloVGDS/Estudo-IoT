// Crie dois arrays de 5 elementos cada e gere um terceiro array com a soma elemento a elemento dos dois anteriores
#include <stdio.h>

int main() {
    int nums1[5] = {14, 87, 33, 5, 62};
    int nums2[5] = {76, 21, 49, 90, 3};
    int sum[5];

    for (int i = 0; i < 5; i++) {
        sum[i] = nums1[i] + nums2[i];
    }

    for (int i = 0; i < 5; i++) {
        printf("%d ", sum[i]);
    }

    return 0;
}