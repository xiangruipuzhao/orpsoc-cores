CROSS_COMPILE=/opt/or1k-toolchain/bin/or1k-elf-gcc

all:
	${CROSS_COMPILE} -o ${BUILD_ROOT}/or1k_main or1k_main.c
	g++ -o testlib main.c -L/opt/or1ksim/lib -lsim

clean:
	rm -f or1k_main testlib *.o *.~
