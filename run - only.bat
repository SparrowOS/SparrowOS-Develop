@echo off
cd ..\z_tools\qemu
qemu.exe -L . -m 32 -localtime -std-vga -fda "F:\SparrowOS Develop\SparrowOS\img\SparrowOS-v0.2.9-BuildVersion2.9.img"