# bmp2chr
## needed data
* File Header

| Name        | Value |
| :---------- | :---- |
| bfType      | 'BM'  |
| bfSize      |       |
| bfReserved1 | 0     |
| bfReserved2 | 0     |
| bfOffBits   |       |

* Info Header

| Name           | Value |
| :------------- | :---- |
| biSize         | 40    |
| biWidth        | n\*8  |
| biHeight       | n\*8  |
| biPlanes       | 1     |
| biBitCount     | 8     |
| biCompression  | 0     |
| biSizeImage    |       |
| biXPixPerMeter |       |
| biYPixPerMeter |       |
| biClrUsed      |       |
| biCirImportant |       |


## treat of pallet
* ((x,y) pixel pallet number)%4 -> (x,y) pixel color number in chr


## movement
* Convert the image from the top right to the right
* When you get to the right edge, convert the image one level down to the right
* Repeat this


## usage
* bmp2chr.exe -n X input_filename output_filename
* if X != 0, then convert X data
* if X == 0, then convert all data
