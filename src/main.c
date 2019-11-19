#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tf_func.h"
#include "func.h"

#define LEN 300


int main() {

    // color_to_grey("./resources/image0.ppm", "./resources/image0g.pgm");
    emboss_pgm("./resources/image0g.pgm", "./resources/image0e.pgm");




    return EXIT_SUCCESS;
}