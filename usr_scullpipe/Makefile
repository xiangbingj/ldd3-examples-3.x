all : pipe_test asynctest

CROSS_COMPILE	?= /home/maix2/work/rk3399pro/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-

pipe_test:pipe_test.o
	$(CROSS_COMPILE)gcc -g -o pipe_test pipe_test.c
asynctest:asynctest.o
	$(CROSS_COMPILE)gcc -g -o asynctest asynctest.c
clean:
	rm pipe_test asynctest *.o