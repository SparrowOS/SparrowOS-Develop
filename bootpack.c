/* 其他处理 */

#include "bootpack.h"
#include <stdio.h>

unsigned int memtest(unsigned int start, unsigned int end);
void memman_init(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man);
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	char s[40], mcursor[256], keybuf[32], mousebuf[128];
	int mx, my, i;
	unsigned int memtotal;
	struct MOUSE_DEC mdec;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	
	init_gdtidt();
	init_pic();
	io_sti(); /* IDT/PICの初期化が終わったのでCPUの割り込み禁止を解除 */
	fifo8_init(&keyfifo, 32, keybuf);
	fifo8_init(&mousefifo, 128, mousebuf);
	io_out8(PIC0_IMR, 0xf9); /* PIC1とキーボードを許可(11111001) */
	io_out8(PIC1_IMR, 0xef); /* マウスを許可(11101111) */

	init_keyboard();
	enable_mouse(&mdec);
	memtotal = memtest(0x00400000, 0xbfffffff);
	memman_init(memman);
	memman_free(memman, 0x00001000, 0x0009e000);
	memman_free(memman, 0x00400000, memtotal - 0x00400000);
	
	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s, "(%d, %d)", mx, my);
	putfonts8(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

	sprintf(s, "memory: %dMB    free: %dKB",
			memtotal / (1024 * 1024), memman_total(memman) / 1024);
	putfonts8(binfo->vram, binfo->scrnx, 0, 32, COL8_FFFFFF, s);

	//水印
		putfonts8(binfo->vram, binfo->scrnx, 239, 155, COL8_000000, "SparrowOS.");
		putfonts8(binfo->vram, binfo->scrnx, 240, 155, COL8_FFFFFF, "SparrowOS.");
	
	putfonts8(binfo->vram, binfo->scrnx, 210, 73, COL8_000000, "Powered by:");
	putfonts8(binfo->vram, binfo->scrnx, 211, 73, COL8_FFFFFF, "Powered by:");
	puttext(binfo->vram, binfo->scrnx, 225, 90, COL8_0000FF, 6);
	puttext(binfo->vram, binfo->scrnx, 242, 90, COL8_0000FF, 7);
	puttext(binfo->vram, binfo->scrnx, 259, 90, COL8_0000FF, 8);
	puttext(binfo->vram, binfo->scrnx, 276, 90, COL8_0000FF, 9);
	putfonts8(binfo->vram, binfo->scrnx, 225, 107, COL8_0000FF, "Kdq_kongdeqi");
	putfonts8(binfo->vram, binfo->scrnx, 225, 124, COL8_0000FF, "buyio");
	
	for (;;) {
		io_cli();
		if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0) {
			io_stihlt();
		} else {
			if (fifo8_status(&keyfifo) != 0) {
				i = fifo8_get(&keyfifo);
				io_sti();
				sprintf(s, "%02X", i);
				boxfill8(binfo->vram, binfo->scrnx, COL8_008484,  0, 16, 15, 31);
				putfonts8(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
			} else if (fifo8_status(&mousefifo) != 0) {
				i = fifo8_get(&mousefifo);
				io_sti();
				if (mouse_decode(&mdec, i) != 0) {
					/* 鼠标的3个字节都齐了，显示出来 */
					sprintf(s, "[lcr %4d %4d]", mdec.x, mdec.y);
					if ((mdec.btn & 0x01) != 0) {
						s[1] = 'L';
					}
					if ((mdec.btn & 0x02) != 0) {
						s[3] = 'R';
					}
					if ((mdec.btn & 0x04) != 0) {
						s[2] = 'C';
					}
					
					//水印
						putfonts8(binfo->vram, binfo->scrnx, 239, 155, COL8_000000, "SparrowOS.");
						putfonts8(binfo->vram, binfo->scrnx, 240, 155, COL8_FFFFFF, "SparrowOS.");
				
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 32 + 15 * 8 - 1, 31);
					putfonts8(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
					
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, mx, my, mx + 15, my + 15);
					mx += mdec.x;
					my += mdec.y;
					if (mx < 0) {
						mx = 0;
					}
					if (my < 0) {
						my = 0;
					}
					if (my > binfo->scrny - 16) {
						my = binfo->scrny - 16;
					}
					if (mx > binfo->scrnx - 16) {
						mx = binfo->scrnx - 16;
					}
					sprintf(s, "(%3d, %3d)", mx, my);
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 0, 79, 15);
					putfonts8(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
					putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
				}
			}
		}
	}
}
