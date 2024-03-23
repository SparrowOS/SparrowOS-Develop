// #include <stdio.h>
// #include "bootpack.h"


// void puttext(unsigned char *vram, int xsize, int x, int y, char c, char *word)
// {
//     extern char font[];
// 	for (; *word != 0x00; word++) {
//         offset = (94*(unsigned int)(word[0]-0xa0-1)+(word[1]-0xa0-1))*32;
//         font += offset;
//         for(k=0; k<16; k++){
//             for(j=0; j<2; j++){
//                 for(i=0; i<8; i++){
//                     flag = font[k*2+j]&key[i];
//                     flag?boxfill8(vram, xsize, c, x, y, x, y):boxfill8(vram, xsize, 99, x, y, x, y);
//                     x++;
//                 }
//             }
//             x = 0;
//             y++;
//         }
//     }
//     return;
// }
