#include "tf_func.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>

// grey pixel max value + 1 (can be a parameter)
#define GPX_MAXVAL 256

const int emboss_coeffs[] = {-2, -1, 0, -1, 0, 1, 0, 1, 2};

static void emboss_coeff(const unsigned char *pixels, int index, unsigned char *newpx, int width) {
    unsigned char ext_mat[EMB_CF_NB];
    extract_mat(pixels, width, index, ext_mat);
    *newpx = 128 + scalar_prod(ext_mat, emboss_coeffs, EMB_CF_NB);
}

grey_image_type *color_to_grey(const char *colorimg, const char *greyimg) {
    color_image_type *clrimg = loadColorImage(colorimg);
    int hgt = clrimg -> height;
    int wdt = clrimg -> width;
    grey_image_type *gimg = createGreyImage(wdt, hgt);
    unsigned char r, g, b;

    color_pixel_type *cpixels = clrimg -> pixels;
    unsigned char *gpixels = gimg -> pixels;
    
    double start = omp_get_wtime();
#pragma omp parallel for num_threads(NB_THD)
    for (int i = 0; i < wdt * hgt; i++) {
        r = cpixels[i].r, g = cpixels[i].g, b = cpixels[i].b;
        gpixels[i] = (unsigned char)((299 * r + 587 * g + 114 * b) / 1000);
    }
    double end = omp_get_wtime();
    printf("color -> grey  time=%.10f\n", end - start);

    saveGreyImage(greyimg, gimg);
    clean_clrimg(clrimg);
    return gimg;
}

void emboss_pgm(const grey_image_type *gimg, const char *embimg) {
    int wdt = gimg -> width;
    int hgt = gimg -> height;
    unsigned char *gpixels = gimg -> pixels;
    grey_image_type *embossed_img = createGreyImage(wdt, hgt);
    unsigned char *egpixels = embossed_img->pixels;        // pixels to be embossed
    double start = omp_get_wtime();

    for (int i = 0; i < wdt; i++) 
        egpixels[i] = gpixels[i];
    for (int i = (hgt - 1) * wdt; i < hgt * wdt; i++) 
        egpixels[i] = gpixels[i];    
#pragma omp parallel for num_threads(NB_THD)
    for (int i = wdt; i < (hgt - 1) * wdt; i++) {          // skip first and last lines
        if ((i % wdt != 0) && ((i + 1) % wdt != 0)) {      // skip fst and last columns
            emboss_coeff(gpixels, i, &egpixels[i], wdt);
        } else {
            egpixels[i] = gpixels[i];
        }
    }
    double end = omp_get_wtime();
    printf("grey -> emboss  time=%.10f\n", end - start);

    saveGreyImage(embimg, embossed_img);
    clean_gimg(embossed_img);
}

void contrast_pgm(const grey_image_type *gimg, const char *contrastimg) {
    int wdt = gimg -> width;
    int hgt = gimg -> height;
    unsigned char *gpixels = gimg -> pixels;
    grey_image_type *cimg = createGreyImage(wdt, hgt);
    unsigned char *cpixels = cimg -> pixels;
    int *hist = calloc(GPX_MAXVAL, sizeof(int));
    int *chist = calloc(GPX_MAXVAL, sizeof(int));

    double start = omp_get_wtime();
    // histograms computation
#pragma omp parallel num_threads(NB_THD)
    {
#pragma omp for
        for (int i = 0; i < wdt * hgt; i++) {
            hist[gpixels[i]]++;
        }
#pragma omp single
        {
            chist[0] = hist[0];
            for (int i = 1; i < GPX_MAXVAL; i++) {
                chist[i] = chist[i - 1] + hist[i];
            }
        }
#pragma omp for
        for (int i = 0; i < wdt * hgt; i++) {
            cpixels[i] = (unsigned char)((float)(256 * chist[gpixels[i]]) / (float)(wdt * hgt));
        }
    }
    double end = omp_get_wtime();
    printf("grey -> contrast  time=%.10f\n", end - start);

    saveGreyImage(contrastimg, cimg);
    clean_gimg(cimg);
    free(hist);
    free(chist);
}

// used for debug purposes
static void print_mat(const int *mat, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d ", mat[i]);
    }
    puts("");
}
