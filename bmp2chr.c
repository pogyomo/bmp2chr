#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BFTYPE              0x4d42  // It mean 'MB'
#define BFRESERVED1         0x0000
#define BFRESERVED2         0x0000
#define BISIZE              0x00000028
#define BIPLANES            0x0001
#define BIBITCOUNT          0x0008
#define BICOMPRESSION       0x00000000
#define BMP_FILEHEADER_BYTE 14
#define BMP_PALETTE_BYTE    4


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
    uint32_t biCirImportant; };

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


bmp8 *read_bmp8(char *);
void  free_bmp8(bmp8 *);
void  debug_bmp8(bmp8 *);


int main(int argc, char *argv[]) {
    bmp8 *ptr = read_bmp8(argv[1]);
    debug_bmp8(ptr);
    free_bmp8(ptr);
}


bmp8 *read_bmp8(char *filename) {
    // Read file
    // If file pointer is null, stop process
    FILE *ifp = fopen(filename, "r");
    if (ifp == NULL) {
        return NULL;
    }

    // Alloc memory for result
    bmp8 *result = (bmp8 *)malloc(sizeof(bmp8));
    if (result == NULL) {
        fclose(ifp);
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
        fclose(ifp);
        return NULL;
    }

    // If reserved data is incorrect, stop process
    if (result->file.bfReserved1 != BFRESERVED1 || result->file.bfReserved2 != BFRESERVED2) {
        free(result);
        fclose(ifp);
        return NULL;
    }

    // If info header size is incorrect, stop process
    if (result->info.biSize != BISIZE) {
        free(result);
        fclose(ifp);
        return NULL;
    }

    // If plane is not 1, stop process
    if (result->info.biPlanes != BIPLANES) {
        free(result);
        fclose(ifp);
        return NULL;
    }

    // If palette isn't 8bit, stop process
    if (result->info.biBitCount != BIBITCOUNT) {
        free(result);
        fclose(ifp);
        return NULL;
    }

    // If file is compressed, stop process
    if (result->info.biCompression != BICOMPRESSION) {
        free(result);
        fclose(ifp);
        return NULL;
    }

    // Alloc memory for palette
    int num_of_palette =
        (result->file.bfOffBits - BMP_FILEHEADER_BYTE - result->info.biSize)/BMP_PALETTE_BYTE;
    result->palette = (rgb_quad *)malloc(sizeof(rgb_quad) * num_of_palette);
    if (result->palette == NULL) {
        free(result);
        fclose(ifp);
        return NULL;
    }

    // Get palette data
    for (int i = 0; i < num_of_palette; i++) {
        fread(&result->palette[i].rgbBlue,     sizeof(result->palette[i].rgbBlue),     1, ifp);
        fread(&result->palette[i].rgbGreen,    sizeof(result->palette[i].rgbGreen),    1, ifp);
        fread(&result->palette[i].rgbRed,      sizeof(result->palette[i].rgbRed),      1, ifp);
        fread(&result->palette[i].rgbReserved, sizeof(result->palette[i].rgbReserved), 1, ifp);
    }

    // Alloc memory for data
    result->data = (uint8_t **)malloc(sizeof(uint8_t *) * result->info.biWidth);
    if (result->data == NULL) {
        free(result->palette);
        free(result);
        fclose(ifp);
        return NULL;
    }
    for (int i = 0; i < result->info.biWidth; i++) {
        result->data[i] = (uint8_t *)malloc(sizeof(uint8_t) * result->info.biHeight);
        if (result->data[i] == NULL) {
            for (int j = 0; j <= i; j++) {
                free(result->data[j]);
            }
            free(result->data);
            free(result->palette);
            free(result);
            fclose(ifp);
            return NULL;
        }
    }

    // Get data
    // Input file's width and height is multiple of 8, so don't think padding
    for (int i = result->info.biHeight - 1; i >= 0; i--) {
        for (int j = 0; j < result->info.biWidth; j++) {
            fread(&result->data[j][i], sizeof(result->data[j][i]), 1, ifp);
        }
    }

    // End of function
    fclose(ifp);
    return result;
}


void free_bmp8(bmp8 *ptr) {
    // Free data ptr
    for (int i = 0; i < ptr->info.biWidth; i++) {
        free(ptr->data[i]);
    }
    free(ptr->data);

    // Free palette ptr
    free(ptr->palette);

    // Free ptr
    free(ptr);
}


void debug_bmp8(bmp8 *ptr) {
    // Print file header
    printf("----------File header----------\n");
    printf("bfType      : %d\n", ptr->file.bfType);
    printf("bfSize      : %d\n", ptr->file.bfSize);
    printf("bfReserved1 : %d\n", ptr->file.bfReserved1);
    printf("bfReserved2 : %d\n", ptr->file.bfReserved2);
    printf("bfOffBits   : %d\n", ptr->file.bfOffBits);
    printf("\n");

    // Print info header
    printf("----------Info header----------\n");
    printf("biSize         : %d\n", ptr->info.biSize);
    printf("biWidth        : %d\n", ptr->info.biWidth);
    printf("biHeight       : %d\n", ptr->info.biHeight);
    printf("biPlanes       : %d\n", ptr->info.biPlanes);
    printf("biBitCount     : %d\n", ptr->info.biBitCount);
    printf("biCompression  : %d\n", ptr->info.biBitCount);
    printf("biSizeImage    : %d\n", ptr->info.biSizeImage);
    printf("biXPixPerMeter : %d\n", ptr->info.biXPixPerMeter);
    printf("biYPixPerMeter : %d\n", ptr->info.biYPixPerMeter);
    printf("biClrUsed      : %d\n", ptr->info.biClrUsed);
    printf("biCirImportant : %d\n", ptr->info.biCirImportant);
    printf("\n");

    // Print palette
    printf("------------Palette------------\n");
    printf("\n");
}
