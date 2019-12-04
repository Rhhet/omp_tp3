#ifndef TF_FUNC_H
#define TF_FUNC_H

#include "func.h"

grey_image_type *color_to_grey(const char *colorimg, const char *greyimg);
void emboss_pgm(const grey_image_type *gimg, const char *embimg);
void contrast_pgm(const grey_image_type *gimg, const char *contrastimg);



#endif