@echo off
copy SparrowOS.img ..\z_tools\qemu\fdimage0.bin
cd ..\z_tools\qemu
qemu.exe -L . -m 32 -localtime -std-vga -fda fdimage0.bin