#include <stdint.h>
#include <stdio.h>



/* Bitmap file header structure */
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;


typedef struct {
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
} BITMAPINFOHEADER;


typedef struct {
    uint8_t  rgbBlue;
    uint8_t  rgbGreen;
    uint8_t  rgbRed;
    uint8_t  rgbReserved;
} RGBQUAD;




void getHeaderData(FILE *ifp, BITMAPFILEHEADER *bmp_file, BITMAPINFOHEADER *bmp_info){
    /* get file header */
    fread(&bmp_file->bfType,      sizeof(bmp_file->bfType),      1, ifp);
    fread(&bmp_file->bfSize,      sizeof(bmp_file->bfSize),      1, ifp);
    fread(&bmp_file->bfReserved1, sizeof(bmp_file->bfReserved1), 1, ifp);
    fread(&bmp_file->bfReserved2, sizeof(bmp_file->bfReserved2), 1, ifp);
    fread(&bmp_file->bfOffBits,   sizeof(bmp_file->bfOffBits),   1, ifp);


    /* get info header */
    fread(&bmp_info->biSize,         sizeof(bmp_info->biSize),         1, ifp);
    fread(&bmp_info->biWidth,        sizeof(bmp_info->biWidth),        1, ifp);
    fread(&bmp_info->biHeight,       sizeof(bmp_info->biHeight),       1, ifp);
    fread(&bmp_info->biPlanes,       sizeof(bmp_info->biPlanes),       1, ifp);
    fread(&bmp_info->biBitCount,     sizeof(bmp_info->biBitCount),     1, ifp);
    fread(&bmp_info->biCompression,  sizeof(bmp_info->biCompression),  1, ifp);
    fread(&bmp_info->biSizeImage,    sizeof(bmp_info->biSizeImage),    1, ifp);
    fread(&bmp_info->biXPixPerMeter, sizeof(bmp_info->biXPixPerMeter), 1, ifp);
    fread(&bmp_info->biYPixPerMeter, sizeof(bmp_info->biYPixPerMeter), 1, ifp);
    fread(&bmp_info->biClrUsed,      sizeof(bmp_info->biClrUsed),      1, ifp);
    fread(&bmp_info->biCirImportant, sizeof(bmp_info->biCirImportant), 1, ifp);
}
