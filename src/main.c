#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "tf_func.h"
#include "func.h"

#define LEN 300


int main() {

    // color_to_grey("../resources/image0.ppm", "../resources/image0gtest.pgm");
    emboss_pgm("../resources/image0g.pgm", "../resources/image0e.pgm");

    puts("test");
    



    return EXIT_SUCCESS;
}