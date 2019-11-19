#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include "tf_func.h"

#define NB_EMB_CF 9  // nb of emboss coeffs

int emboss_coeffs[] = {-2, -1, 0, -1, 0, 1, 0, 1, 2};

// index should be the pixel corresponding to the first emboss coeff
static void emboss_coeff(const unsigned char *pixels, int index, unsigned char *newpx) {
    *newpx = 128;
    for (int i = 0; i < NB_EMB_CF; i++) {
        *newpx += pixels[index + i] * emboss_coeffs[i];
    }
}


void color_to_grey(const char *colorimg, const char *greyimg) {
    color_image_type *clrimg = loadColorImage(colorimg);
    int hgt = clrimg -> height;
    int wdt = clrimg -> width;
    grey_image_type *gimg = createGreyImage(wdt, hgt);
    unsigned char r, g, b;

    color_pixel_type *cpixels = clrimg -> pixels;
    unsigned char *gpixels = gimg -> pixels;
    
    for (int i = 0; i < wdt * hgt; i++) {
        r = cpixels[i].r, g = cpixels[i].g, b = cpixels[i].b;
        gpixels[i] = (unsigned char)((299 * r + 587 * g + 114 * b) / 1000);
    }

    saveGreyImage(greyimg, gimg);
    // free(clrimg);
    // free(gimg);
}

void emboss_pgm(const char *greyimg, const char *embimg) {
    grey_image_type *gimg = loadGreyImage(greyimg);
    int wdt = gimg -> width;
    int hgt = gimg -> height;
    unsigned char *gpixels = gimg -> pixels;
    grey_image_type *embossed_img = createGreyImage(wdt, hgt);
    unsigned char *egpixels = embossed_img->pixels;        // pixels to be embossed

    for (int i = wdt; i < (hgt - 2) * wdt; i++) {          // skip first and last lines
        if ((i % wdt != 0) && ((i + 1) % wdt != 0)) {      // skip fst and last columns
            emboss_coeff(gpixels, i - wdt - 1, &egpixels[i]);
        } else {
            egpixels[i] = gpixels[i];
        }
        for (int i = 0; i < wdt; i++) 
            egpixels[i] = gpixels[i];
        for (int i = (hgt - 2) * wdt; i < (hgt - 1) * wdt; i++) 
            egpixels[i] = gpixels[i];    
    }

    saveGreyImage(embimg, embossed_img);
    // free(gimg);
    // free(embimg);
}
