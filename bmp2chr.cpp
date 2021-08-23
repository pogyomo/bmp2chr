#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "include/bmp_util.c"



/* check value for header data */
#define BFTYPE        0x4d42  //converted value for little endian
#define BFRESERVED1   0x0000
#define BFRESERVED2   0x0000
#define BISIZE        0x00000028
#define BIPLANES      0x0001
#define BIBITCOUNT    0x0008
#define BICOMPRESSION 0x00000000



void finish(FILE*, FILE*, int);



int main(int argc, char *argv[]){
    /* declare variable */
    BITMAPFILEHEADER bmp_file;
    BITMAPINFOHEADER bmp_info;
    std::vector<std::vector<uint8_t>> image_data;
    std::vector<std::vector<uint8_t>> convert_tmp1(8, std::vector<uint8_t>(8));
    std::vector<std::vector<uint8_t>> convert_tmp2(8, std::vector<uint8_t>(8));
    uint8_t tmp;
    int number;
    FILE *ifp = NULL;
    FILE *ofp = NULL;


    /* check argument */
    if (argc != 5){
        fprintf(stderr, "incorrect argument\n");
        finish(ifp, ofp, 1);
    }
    

    /* get number of character to convert */
    if (strcmp(argv[1], "-n") == 0){
        number = atoi(argv[2]); //if number is equal 0, this program convert all data
    }


    /* open file */
    ifp = fopen(argv[3], "rb");
    if (ifp == NULL){
        fprintf(stderr, "cannot open file\n");
        finish(ifp, ofp, 2);
    }


    /* get file header data */
    getHeaderData(ifp, &bmp_file, &bmp_info);


    /* check header data */
    if (bmp_file.bfType != BFTYPE){
        fprintf(stderr, "this file may not bmp file\n");
        finish(ifp, ofp, 4);
    }
    if (bmp_file.bfReserved1 != BFRESERVED1){
        fprintf(stderr, "bfReserved1 must be %#x\n", BFRESERVED1);
        finish(ifp, ofp, 4);
    }
    if (bmp_file.bfReserved2 != BFRESERVED2){
        fprintf(stderr, "bfReserved1 must be %#x\n", BFRESERVED2);
        finish(ifp, ofp, 4);
    }
    if (bmp_info.biSize != BISIZE){
        fprintf(stderr, "incorrect info header size %x\n", bmp_info.biSize);
        finish(ifp, ofp, 4);
    }
    if (bmp_info.biWidth%8 != 0){
        fprintf(stderr, "incorrect file width\n");
        finish(ifp, ofp, 4);
    }
    if (bmp_info.biHeight%8 != 0){
        fprintf(stderr, "incorrect file height\n");
        finish(ifp, ofp, 4);
    }
    if (bmp_info.biPlanes != BIPLANES){
        fprintf(stderr, "biPlanes must be %#x\n", BIPLANES);
        finish(ifp, ofp, 4);
    }
    if (bmp_info.biBitCount != BIBITCOUNT){
        fprintf(stderr, "incorrect color bit\n");
        finish(ifp, ofp, 4);
    }
    if (bmp_info.biCompression != BICOMPRESSION){
        fprintf(stderr, "biCompression must be %#x\n", BICOMPRESSION);
        finish(ifp, ofp, 4);
    }


    /* create file */
    ofp = fopen(argv[4], "wb");
    if (ofp == NULL){
        fprintf(stderr, "cannot create file\n");
        finish(ifp, ofp, 3);
    }


    /* ignore pallet data */
    for (int i = 0; i < bmp_file.bfOffBits - 14 - bmp_info.biSize; i++){
        fread(&tmp, sizeof(tmp), 1, ifp);
    }


    /* get image data */
    image_data.resize(bmp_info.biHeight);
    for (int i = 0; i < bmp_info.biHeight; i++){
        image_data[i].resize(bmp_info.biWidth);
    }
    for (int i = bmp_info.biHeight - 1; i >= 0; i--){  // Width and Height is multiple of 4 
        for (int j = 0; j < bmp_info.biWidth; j++){    // So, dont think about padding
            fread(&tmp, sizeof(tmp), 1, ifp);
            image_data[i][j] = tmp;
        }
    }


    /* convert */
    int cnt=0;
    if (number != 0){
        for (int i = 0; i < bmp_info.biHeight; i+=8){
            for (int j = 0; j < bmp_info.biWidth; j+=8){


                /* print number */
                printf("converting data %d\n", cnt);
                cnt++;


                /* convert data */
                for (int k = 0; k < 8; k++){
                    for (int l = 0; l < 8; l++){
                        if (image_data[i+k][j+l]%4 == 0){
                            convert_tmp1[k][l] = 0;
                            convert_tmp2[k][l] = 0;
                        }
                        if (image_data[i+k][j+l]%4 == 1){
                            convert_tmp1[k][l] = 1;
                            convert_tmp2[k][l] = 0;
                        }
                        if (image_data[i+k][j+l]%4 == 2){
                            convert_tmp1[k][l] = 0;
                            convert_tmp2[k][l] = 1;
                        }
                        if (image_data[i+k][j+l]%4 == 3){
                            convert_tmp1[k][l] = 1;
                            convert_tmp2[k][l] = 1;
                        }
                    }
                }


                /* write data to file */
                for (int k = 0; k < 8; k++){
                    tmp = 0;
                    for (int l = 0; l < 8; l++){
                        tmp += convert_tmp1[k][l]<<(7-l);
                    }
                    fwrite(&tmp, sizeof(tmp), 1, ofp);
                }
                for (int k = 0; k < 8; k++){
                    tmp = 0;
                    for (int l = 0; l < 8; l++){
                        tmp += convert_tmp2[k][l]<<(7-l);
                    }
                    fwrite(&tmp, sizeof(tmp), 1, ofp);
                }

                
                /* dec counter */
                number--;
                if (number == 0){
                    finish(ifp, ofp, 0);
                }


            }
        }
    }else{
        for (int i = 0; i < bmp_info.biHeight; i+=8){
            for (int j = 0; j < bmp_info.biWidth; j+=8){


                /* print number */
                printf("converting data %d\n", cnt);
                cnt++;


                /* convert data */
                for (int k = 0; k < 8; k++){
                    for (int l = 0; l < 8; l++){
                        if (image_data[i+k][j+l]%4 == 0){
                            convert_tmp1[k][l] = 0;
                            convert_tmp2[k][l] = 0;
                        }
                        if (image_data[i+k][j+l]%4 == 1){
                            convert_tmp1[k][l] = 1;
                            convert_tmp2[k][l] = 0;
                        }
                        if (image_data[i+k][j+l]%4 == 2){
                            convert_tmp1[k][l] = 0;
                            convert_tmp2[k][l] = 1;
                        }
                        if (image_data[i+k][j+l]%4 == 3){
                            convert_tmp1[k][l] = 1;
                            convert_tmp2[k][l] = 1;
                        }
                    }
                }


                /* write data to file */
                for (int k = 0; k < 8; k++){
                    tmp = 0;
                    for (int l = 0; l < 8; l++){
                        tmp += convert_tmp1[k][l]<<(7-l);
                    }
                    fwrite(&tmp, sizeof(tmp), 1, ofp);
                }
                for (int k = 0; k < 8; k++){
                    tmp = 0;
                    for (int l = 0; l < 8; l++){
                        tmp += convert_tmp2[k][l]<<(7-l);
                    }
                    fwrite(&tmp, sizeof(tmp), 1, ofp);
                }


            }
        }
    }


    /* End of program */
    finish(ifp, ofp, 0);
}



void finish(FILE *ifp, FILE *ofp, int return_num){
    if (ifp != NULL){
        fclose(ifp);
    }
    if (ofp != NULL){
        fclose(ofp);
    }
    exit(return_num);
}
