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


## 動き
* 右上から右方向に画像をコンバートしていく
* 右端についたら一段下の画像を右方向にコンバートしていく
* これを繰り返す
