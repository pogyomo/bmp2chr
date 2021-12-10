#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


// Value to check header data
#define BFTYPE        0x4d42  // It mead 'MB'
#define BFRESERVED1   0x0000
#define BFRESERVED2   0x0000
#define BISIZE        0x00000028
#define BIPLANES      0x0001
#define BIBITCOUNT    0x0008
#define BICOMPRESSION 0x00000000


typedef struct bmp_file_header bmp_file_header;
struct bmp_file_header {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

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

typedef struct rgb_quad rgb_quad;
struct rgb_quad {
    uint8_t  rgbBlue;
    uint8_t  rgbGreen;
    uint8_t  rgbRed;
    uint8_t  rgbReserved;
};

typedef struct bmp8 bmp8;
struct bmp8 {
    bmp_file_header file;
    bmp_info_header info;
    rgb_quad *palette;
    uint8_t **data;
};


bmp8 *read_bmp8(FILE *);
void  free_bmp8(bmp8 *);


int main(int argc, char *argv[]) {
}


bmp8 *read_bmp8(FILE *ifp) {
    // Alloc memory for result
    bmp8 *result = (bmp8 *)malloc(sizeof(bmp8));

    // If file pointer is null, stop process
    if (ifp == NULL) {
        free(result);
        return NULL;
    }

    // Get file header
    fread(&result->file.bfType,      sizeof(result->file.bfType),      1, ifp);
    fread(&result->file.bfSize,      sizeof(result->file.bfSize),      1, ifp);
    fread(&result->file.bfReserved1, sizeof(result->file.bfReserved1), 1, ifp);
    fread(&result->file.bfReserved2, sizeof(result->file.bfReserved2), 1, ifp);
    fread(&result->file.bfOffBits,   sizeof(result->file.bfOffBits),   1, ifp);

    // Get info header
    fread(&result->info.biSize,         sizeof(result->info.biSize),         1, ifp);
    fread(&result->info.biWidth,        sizeof(result->info.biWidth),        1, ifp);
    fread(&result->info.biHeight,       sizeof(result->info.biHeight),       1, ifp);
    fread(&result->info.biPlanes,       sizeof(result->info.biPlanes),       1, ifp);
    fread(&result->info.biBitCount,     sizeof(result->info.biBitCount),     1, ifp);
    fread(&result->info.biCompression,  sizeof(result->info.biCompression),  1, ifp);
    fread(&result->info.biSizeImage,    sizeof(result->info.biSizeImage),    1, ifp);
    fread(&result->info.biXPixPerMeter, sizeof(result->info.biXPixPerMeter), 1, ifp);
    fread(&result->info.biYPixPerMeter, sizeof(result->info.biYPixPerMeter), 1, ifp);
    fread(&result->info.biClrUsed,      sizeof(result->info.biClrUsed),      1, ifp);
    fread(&result->info.biCirImportant, sizeof(result->info.biCirImportant), 1, ifp);

    // If it isn't bmp file, stop process
    if (result->file.bfType != BFTYPE) {
        free(result);
        return NULL;
    }

    // If reserved data is incorrect, stop process
    if (result->file.bfReserved1 != 0 || result->file.bfReserved2 != 0) {
        free(result);
        return NULL;
    }

    // If palette isn't 8bit, stop process
    if (result->info.biBitCount != 8) {
        free(result);
        return NULL;
    }


    // End of function
    return result;
}
