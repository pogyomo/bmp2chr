#include "bmp8.h"


bmp8 *bmp8_read(char *filename) {
    // Read file
    // If file pointer is null, stop process
    FILE *ifp = fopen(filename, "rb");
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

    // If file width or height isn't multiple of 8, stop process
    if (result->info.biWidth % 8 != 0 || result->info.biHeight % 8 != 0) {
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


void bmp8_free(bmp8 *ptr) {
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


void bmp8_debug(bmp8 *ptr) {
    // Print file header
    printf("----------File header----------\n");
    printf("bfType      : %0x\n", ptr->file.bfType);
    printf("bfSize      : %0x\n", ptr->file.bfSize);
    printf("bfReserved1 : %0x\n", ptr->file.bfReserved1);
    printf("bfReserved2 : %0x\n", ptr->file.bfReserved2);
    printf("bfOffBits   : %0x\n", ptr->file.bfOffBits);
    printf("\n");

    // Print info header
    printf("----------Info header----------\n");
    printf("biSize         : %0x\n", ptr->info.biSize);
    printf("biWidth        : %0x\n", ptr->info.biWidth);
    printf("biHeight       : %0x\n", ptr->info.biHeight);
    printf("biPlanes       : %0x\n", ptr->info.biPlanes);
    printf("biBitCount     : %0x\n", ptr->info.biBitCount);
    printf("biCompression  : %0x\n", ptr->info.biBitCount);
    printf("biSizeImage    : %0x\n", ptr->info.biSizeImage);
    printf("biXPixPerMeter : %0x\n", ptr->info.biXPixPerMeter);
    printf("biYPixPerMeter : %0x\n", ptr->info.biYPixPerMeter);
    printf("biClrUsed      : %0x\n", ptr->info.biClrUsed);
    printf("biCirImportant : %0x\n", ptr->info.biCirImportant);
    printf("\n");

    // Print palette
    printf("------------Palette------------\n");
    int num_of_palette =
        (ptr->file.bfOffBits - BMP_FILEHEADER_BYTE - ptr->info.biSize)/BMP_PALETTE_BYTE;
    for (int i = 0; i < num_of_palette; i++) {
        printf("Palette%3d : blue = %02x, green = %02x, red = %02x, reserved = %02x\n",
                i,
                ptr->palette[i].rgbBlue,
                ptr->palette[i].rgbGreen,
                ptr->palette[i].rgbRed,
                ptr->palette[i].rgbReserved
                );
    }
    printf("\n");

    // Print data
    printf("-----------Pixel data-----------\n");
    for (int i = 0; i < ptr->info.biHeight; i++) {
        // Print first half of row
        printf("Line%3d_1 : ", i + 1);
        for (int j = 0; j < ptr->info.biWidth/2; j++) {
            printf("%02x ", ptr->data[j][i]);
        }
        printf("\n");

        // Print last half of row
        printf("Line%3d_2 : ", i + 1);
        for (int j = ptr->info.biWidth/2; j < ptr->info.biWidth; j++) {
            printf("%02x ", ptr->data[j][i]);
        }
        printf("\n");
    }
}


int bmp8_convert(bmp8 *ptr, char *filename, int num) {
    // Open file with write only
    FILE *ofp = fopen(filename, "wb");
    if (ofp == NULL) {
        return 1;
    }

    // Alloc memory for convert
    uint8_t **conv_temp1 = (uint8_t **)malloc(sizeof(uint8_t *) * 8);
    if (conv_temp1 == NULL) {
        return 1;
    }
    for (int i = 0; i < 8; i++) {
        conv_temp1[i] = (uint8_t *)malloc(sizeof(uint8_t) * 8);
        if (conv_temp1[i] == NULL) {
            for (int j = 0; j <= i; j++) {
                free(conv_temp1[j]);
                free(conv_temp1);
                return 1;
            }
        }
    }
    uint8_t **conv_temp2 = (uint8_t **)malloc(sizeof(uint8_t *) * 8);
    if (conv_temp2 == NULL) {
        return 1;
    }
    for (int i = 0; i < 8; i++) {
        conv_temp2[i] = (uint8_t *)malloc(sizeof(uint8_t) * 8);
        if (conv_temp2[i] == NULL) {
            for (int j = 0; j <= i; j++) {
                free(conv_temp2[j]);
                free(conv_temp2);
                return 1;
            }
        }
    }

    // Convert all data or subset of data
    uint8_t temp;
    int cnt = 0;
    for (int i = 0; i < ptr->info.biHeight; i += 8) {
        for (int j = 0; j < ptr->info.biWidth; j += 8) {
            // Convert data to 0 or 1
            for (int k = 0; k < 8; k++) {
                for (int l = 0; l < 8; l++) {
                    if (ptr->data[j + l][i + k] % 4 == 0) {
                        conv_temp1[l][k] = 0;
                        conv_temp2[l][k] = 0;
                    } else if (ptr->data[j + l][i + k] % 4 == 1) {
                        conv_temp1[l][k] = 1;
                        conv_temp2[l][k] = 0;
                    } else if (ptr->data[j + l][i + k] % 4 == 2) {
                        conv_temp1[l][k] = 0;
                        conv_temp2[l][k] = 1;
                    } else if (ptr->data[j + l][i + k] % 4 == 3) {
                        conv_temp1[l][k] = 1;
                        conv_temp2[l][k] = 1;
                    } 
                }
            }

            // Write data to file
            for (int k = 0; k < 8; k++){
                temp = 0;
                for (int l = 0; l < 8; l++){
                    temp += conv_temp1[l][k]<<(7-l);
                }
                fwrite(&temp, sizeof(temp), 1, ofp);
            }
            for (int k = 0; k < 8; k++){
                temp = 0;
                for (int l = 0; l < 8; l++){
                    temp += conv_temp2[l][k]<<(7-l);
                }
                fwrite(&temp, sizeof(temp), 1, ofp);
            }

            // Detect end
            // If num is 0, convert all data
            cnt++;
            if (cnt == num) {
                for (int i = 0; i < 8; i++) {
                    free(conv_temp1[i]);
                    free(conv_temp2[i]);
                }
                free(conv_temp1);
                free(conv_temp2);
                return 0;
            }
        }
    }

    // End of program
    for (int i = 0; i < 8; i++) {
        free(conv_temp1[i]);
        free(conv_temp2[i]);
    }
    free(conv_temp1);
    free(conv_temp2);
    return 0;
}
