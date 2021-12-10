#include <stdio.h>
#include <unistd.h>
#include "bmp8.h"


int main(int argc, char *argv[]) {
    // Get option
    int opt;
    int debug_enable      = 0;
    char *input_filename  = NULL;
    char *output_filename = NULL;
    char *num_string      = NULL;
    while ((opt = getopt(argc, argv, "i:o:dn:")) != -1) {
        switch (opt) {
            case 'i':
                input_filename  = optarg;
                break;
            case 'o':
                output_filename = optarg;
                break;
            case 'd':
                debug_enable    = 1;
                break;
            case 'n':
                num_string      = optarg;
                break;
            default:
                break;
        }
    }

    // Detect error
    if (input_filename == NULL || output_filename == NULL || num_string == NULL) {
        fprintf(stderr, "Usage : bmp2chr [-option]\n");
        fprintf(stderr, "-i : input file name\n");
        fprintf(stderr, "-o : output file name\n");
        fprintf(stderr, "-n : number of tile to convert. 0 mean all\n");
        fprintf(stderr, "-d : for debug\n");
        return 1;
    }

    // Process
    bmp8 *ptr = bmp8_read(input_filename);
    if (debug_enable == 1) {
        bmp8_debug(ptr);
    }
    bmp8_convert(ptr, output_filename, atoi(num_string));

    // End of program
    bmp8_free(ptr);
    return 0;
}
