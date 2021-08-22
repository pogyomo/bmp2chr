#include <cstdint>
#include <stdio.h>
#include <vector>
#include "include/bmp_util.c"



/* check value for header data */
#define BFTYPE      0x4d42  //converted value for little endian
#define BISIZE      0x00000028
#define BIBITCOUNT  0x0008



int main(int argc, char *argv[]){
    /* declare variable */
    BITMAPFILEHEADER bmp_file;
    BITMAPINFOHEADER bmp_info;
    uint8_t trash;
    FILE *ifp;
    FILE *ofp;


    /* check argument */
    if (argc != 3){
        fprintf(stderr, "incorrect argument\n");
        return 1;
    }


    /* open file */
    ifp = fopen(argv[1], "rb");
    if (ifp == NULL){
        fprintf(stderr, "cannot open file\n");
        return 2;
    }


    /* create file */
    ofp = fopen(argv[2], "wb");
    if (ofp == NULL){
        fprintf(stderr, "cannot create file\n");
        return 3;
    }


    /* get file header data */
    getFileHeader(ifp, &bmp_file);
    if (bmp_file.bfType != BFTYPE){
        fprintf(stderr, "this file may not bmp file\n");
        return 4;
    }



    /* get info header data */
    getInfoHeader(ifp, &bmp_info);
    if (bmp_info.biSize != BISIZE){
        fprintf(stderr, "incorrect info header size %x\n", bmp_info.biSize);
        return 4;
    }
    if (bmp_info.biWidth%8 != 0){
        fprintf(stderr, "incorrect file width\n");
        return 4;
    }
    if (bmp_info.biHeight%8 != 0){
        fprintf(stderr, "incorrect file height\n");
        return 4;
    }
    if (bmp_info.biBitCount != BIBITCOUNT){
        fprintf(stderr, "incorrect color bit\n");
        return 4;
    }


    /* End of program */
    fclose(ifp);
    fclose(ofp);
    return 0;
}
