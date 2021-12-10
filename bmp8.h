#ifndef BMP8_H
#define BMP8_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


// Define to check header data
#define BFTYPE              0x4d42
#define BFRESERVED1         0x0000
#define BFRESERVED2         0x0000
#define BISIZE              0x00000028
#define BIPLANES            0x0001
#define BIBITCOUNT          0x0008
#define BICOMPRESSION       0x00000000


// Define for calcuration
#define BMP_FILEHEADER_BYTE 14
#define BMP_PALETTE_BYTE    4


// Bitmap file header
typedef struct bmp_file_header bmp_file_header;
struct bmp_file_header {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};


// Bitmap info header
typedef struct bmp_info_header bmp_info_header;
struct bmp_info_header {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPixPerMeter;
    int32_t  biYPixPerMeter;
    uint32_t biClrUsed;
    uint32_t biCirImportant;
};


// Bitmap palette data
typedef struct rgb_quad rgb_quad;
struct rgb_quad {
    uint8_t  rgbBlue;
    uint8_t  rgbGreen;
    uint8_t  rgbRed;
    uint8_t  rgbReserved;
};


// Bitmap with 8bit palette
typedef struct bmp8 bmp8;
struct bmp8 {
    bmp_file_header file;
    bmp_info_header info;
    rgb_quad *palette;
    uint8_t **data;
};


// Name : read_bmp8
// Move : Read bitmap file from given file
// Ret  : If failed, return NULL
bmp8 *bmp8_read(char *);


// Name : free_bmp8
// Move : Free memory
// Ret  : None
void bmp8_free(bmp8 *);


// Name : debug_bmp8
// Move : Display bmp8's data
// Ret  : None
void bmp8_debug(bmp8 *);


// Name : bmp8_convert
// Move : Convert bmp8 to chr and write it to file
//        If 3rd argument is 0, convert all data
//        Else, convert argument's quantity of data linearly
// Ret  : If failed, return 1, else return 0
int bmp8_convert(bmp8 *, char *, int);


#endif
