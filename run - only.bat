@echo off
cd ..\z_tools\qemu
qemu.exe -L . -m 32 -localtime -std-vga -fda ../../SparrowOS/SparrowOS.img