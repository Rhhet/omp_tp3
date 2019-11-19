#include "util.h"

#include <stdlib.h>

void extract_mat(const unsigned char *matrix, int width, int index, unsigned char *ext_mat) {
    for (int i = 0; i < EMB_CF_NB; i++) {
        if (i < 3) {            // first line
            ext_mat[i] = matrix[index - width + i - 1];
        } else if (i < 6) {     // second line
            ext_mat[i] = matrix[index + i - 4];
        } else {                // last line
            ext_mat[i] = matrix[index + width + i - 7];
        }
    }
}

unsigned char scalar_prod(const unsigned char *vec1, const int *vec2, int vlength) {
    int res = 0;
    for (int i = 0; i < vlength; i++) 
        res += vec1[i] * vec2[i];
    return (unsigned char)res;
}
