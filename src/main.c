#include "tf_func.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>



int main() {
    char *pathToGimg = "../resources/image2g.pgm";
    grey_image_type *gimg;
    FILE *fgimg;

    if ((fgimg = fopen(pathToGimg, "rb")) == NULL) {   // file non-existant
        gimg = color_to_grey("../resources/image2.ppm", "../resources/image2g.pgm");
        puts("color -> grey: done");
    } else {   // file already exists (no need to re-process it)
        gimg = loadGreyImage("../resources/image2g.pgm");
        puts("load grey img: done");
        fclose(fgimg);
    }
    

    emboss_pgm(gimg, "../resources/image2e.pgm");
    puts("grey -> embossed: done");

    contrast_pgm(gimg, "../resources/image2c.pgm");
    puts("grey -> contrasted: done");


    clean_gimg(gimg);

    return EXIT_SUCCESS;
}