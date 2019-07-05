all:test_tty

CROSS_COMPILE	?= /home/maix2/work/rk3399pro/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-

test_tty:test_tty.o
	$(CROSS_COMPILE)gcc -g -o test_tty test_tty.c
clean:
	rm test_tty *.o