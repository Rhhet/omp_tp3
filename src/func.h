#ifndef FUNC_H
#define FUNC_H

// number of threads
#define NB_THD 16


typedef struct color_pixel_struct {
    unsigned char r, g, b;
} color_pixel_type;

typedef struct color_image_struct {
    int width, height;
    color_pixel_type *pixels;
} color_image_type;

typedef struct grey_image_struct {
    int width, height;
    unsigned char *pixels;
} grey_image_type;



color_image_type *loadColorImage(const char *filename);
grey_image_type *createGreyImage(int width, int height);
void saveGreyImage(const char *filename, grey_image_type *image);
grey_image_type *loadGreyImage(const char *filename);
void clean_clrimg(color_image_type *clrimg);  
void clean_gimg(grey_image_type *gimg);

#endif