#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <omp.h>

#include "func.h"

/**
 * These functions use fscanf/fprintf that have a lock 
 * => cannot paralellize (at least, it seems to be difficult to do so...)
 */


/**********************************************************************/

color_image_type *loadColorImage(const char *filename) {
    int width, height, max_value;
    char format[8];
    color_image_type *image;
    FILE *f = fopen(filename, "rb");

    if (!f) {
        fprintf(stderr, "Cannot open file %s...\n", filename);
        exit(-1);
    }
    fscanf(f, "%s\n", format);
    assert((format[0] == 'P' && format[1] == '3')); // check P3 format

    while (fgetc(f) == '#') {                      // commentaire
        while (fgetc(f) != '\n')
            ; // aller jusqu'a la fin de la ligne
    }
    fseek(f, -1, SEEK_CUR);
    fscanf(f, "%d %d\n", &width, &height);
    fscanf(f, "%d\n", &max_value);

    image = malloc(sizeof(color_image_type));
    assert(image != NULL);
    image->width = width;
    image->height = height;
    image->pixels = malloc(width * height * sizeof(color_pixel_type));
    assert(image->pixels != NULL);

    double start = omp_get_wtime();
    for (int i = 0; i < width * height; i++) {
        int r, g, b;
        fscanf(f, "%d %d %d", &r, &g, &b);
        image->pixels[i].r = (unsigned char)r;
        image->pixels[i].g = (unsigned char)g;
        image->pixels[i].b = (unsigned char)b;
    }
    double end = omp_get_wtime();
    printf("load color img  time=%.10f\n", end - start);

    fclose(f);
    return image;
}

/**********************************************************************/

/** Added -- should be used when the color image has already been
 * transformed into a grey one instead of using the color_to_grey
 * function along with loadColorImage 
 */
grey_image_type *loadGreyImage(const char *filename) {
    int width, height, max_value;
    char format[8];
    grey_image_type *image;
    FILE *f = fopen(filename, "rb");

    if (!f) {
        fprintf(stderr, "Cannot open file %s...\n", filename);
        exit(-1);
    }
    fscanf(f, "%s\n", format);
    assert((format[0] == 'P' && format[1] == '2')); // check P2 format

    while (fgetc(f) == '#') {                      // commentaire
        while (fgetc(f) != '\n')
            ; // aller jusqu'a la fin de la ligne
    }
    fseek(f, -1, SEEK_CUR);
    fscanf(f, "%d %d\n", &width, &height);
    fscanf(f, "%d\n", &max_value);

    image = malloc(sizeof(grey_image_type));
    assert(image != NULL);
    image->width = width;
    image->height = height;
    image->pixels = malloc(width * height * sizeof(unsigned char));
    assert(image->pixels != NULL);
    
    double start = omp_get_wtime();
    for (int i = 0; i < width * height; i++) {
        int g;
        fscanf(f, "%d", &g);
        image -> pixels[i] = (unsigned char)g;
    }
    double end = omp_get_wtime();
    printf("load grey img  time=%.10f\n", end - start);

    fclose(f);
    return image;
}

/**********************************************************************/

grey_image_type *createGreyImage(int width, int height) {
    grey_image_type *image = malloc(sizeof(grey_image_type));
    assert(image != NULL);
    image->width = width;
    image->height = height;
    image->pixels = malloc(width * height * sizeof(unsigned char));
    assert(image->pixels != NULL);
    return (image);
}

/**********************************************************************/

void saveGreyImage(const char *filename, grey_image_type *image) {
    FILE *f = fopen(filename, "w");

    if (!f) {
        fprintf(stderr, "Cannot open file %s...\n", filename);
        exit(-1);
    }
    fprintf(f, "P2\n%d %d\n255\n", image->width, image->height);
    double start = omp_get_wtime();
    for (int i = 0; i < image->width * image->height; i++) {
        fprintf(f, "%d\n", image->pixels[i]);
    }
    double end = omp_get_wtime();
    printf("save grey img  time=%.10f\n", end - start);
    
    fclose(f);
}

/**********************************************************************/
// cleaning functions: to free the heap of unused images

void clean_clrimg(color_image_type *clrimg) {   
    free(clrimg->pixels);
    free(clrimg);
}

void clean_gimg(grey_image_type *gimg) {
    free(gimg->pixels);
    free(gimg);
}