#ifndef UTIL_H
#define UTIL_H

#define EMB_CF_NB 9  // nb of emboss coeffs


void extract_mat(const unsigned char *matrix, int width, int index, unsigned char *ext_mat);
unsigned char scalar_prod(const unsigned char *vec1, const int *vec2, int vlength);



#endif