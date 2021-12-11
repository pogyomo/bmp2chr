#include <stdio.h>
#include <unistd.h>
#include "bmp8.h"


int main(int argc, char *argv[]) {
    // Get option
    int opt;
    int debug_enable      = 0;
    int help_enable       = 0;
    char *input_filename  = NULL;
    char *output_filename = NULL;
    char *num_string      = NULL;
    while ((opt = getopt(argc, argv, "i:o:dn:?")) != -1) {
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
            case '?':
                help_enable     = 1;
                break;
            default:
                break;
        }
    }

    // Detect help
    if (help_enable == 1) {
        fprintf(stderr, "Usage : bmp2chr [-option] [-? (for help)]\n");
        fprintf(stderr, "-i : input file name\n");
        fprintf(stderr, "-o : output file name\n");
        fprintf(stderr, "-n : number of 8x8 tile to convert linearly. 0 mean all\n");
        fprintf(stderr, "-d : for debug\n");
        return 1;
    }

    // Detect error
    int error_flag = 0;
    if (input_filename == NULL) {
        fprintf(stderr, "You need to specify input file name\n");
        error_flag = 1;
    }
    if (input_filename == NULL) {
        fprintf(stderr, "You need to specify input file name\n");
        error_flag = 1;
    }
    if (num_string == NULL) {
        fprintf(stderr, "You need to specify the number of 8x8 tile to convert linearly\n");
        error_flag = 1;
    }
    if (error_flag == 1) {
        return 1;
    }

    // Process
    bmp8 *ptr = bmp8_read(input_filename);
    if (debug_enable == 1) {
        bmp8_debug(ptr);
    }
    bmp8_convert(ptr, output_filename, atoi(num_string), debug_enable);

    // End of program
    bmp8_free(ptr);
    return 0;
}
