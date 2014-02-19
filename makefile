# Makefile prototype for configure
# Copyright 2004 Phil Karn, KA9Q
# May be used under the terms of the GNU Lesser General Public License (LGPL)

# makefile.  Generated from makefile.in by configure.
srcdir = .
prefix = /usr/local
exec_prefix=${prefix}

CC=gcc
LIBS=dotprod_port.o 	peakval_port.o 	sumsq.o sumsq_port.o 	cpu_mode_x86_64.o fec.o sim.o viterbi27.o viterbi27_port.o viterbi29.o viterbi29_port.o \
	viterbi39.o viterbi39_port.o \
	viterbi615.o viterbi615_port.o encode_rs_char.o encode_rs_int.o encode_rs_8.o \
	decode_rs_char.o decode_rs_int.o decode_rs_8.o \
	init_rs_char.o init_rs_int.o ccsds_tab.o \
	encode_rs_ccsds.o decode_rs_ccsds.o ccsds_tal.o \
	dotprod.o dotprod_port.o \
	peakval.o peakval_port.o \
	sumsq.o sumsq_port.o

CFLAGS=-g -O2 -I. -fPIC -Wall -msse2
SHARED_LIB=libfec.so

all: libfec.a $(SHARED_LIB)
	
test:  vtest27 vtest29 vtest39 vtest615 rstest dtest sumsq_test peaktest
	@echo "Correctness tests:"
	./vtest27 -e 3.0 -n 1000 -v
	./vtest29 -e 2.5 -n 1000 -v
	./vtest39 -e 2.5 -n 1000 -v
	./vtest615 -e 1.0 -n 100 -v
	./rstest
	./dtest
	./sumsq_test
	./peaktest
	@echo "Speed tests:"
	./myCode
	./vtest27
	./vtest29
	./vtest39
	./vtest615

install: all
	mkdir -p ${exec_prefix}/lib 
	install -m 644 -p $(SHARED_LIB) libfec.a ${exec_prefix}/lib
#	(cd ${exec_prefix}/lib;ln -f -s $(SHARED_LIB) libfec.so)
	ldconfig
	mkdir -p ${prefix}/include
	install -m 644 -p fec.h ${prefix}/include
	mkdir -m 0755 -p ${prefix}/share/man/man3
	install -m 644 -p simd-viterbi.3 rs.3 dsp.3 ${prefix}/share/man/man3

peaktest: peaktest.o libfec.a
	gcc $(CFLAGS) -g -o $@ $^

sumsq_test: sumsq_test.o libfec.a
	gcc $(CFLAGS) -g -o $@ $^

dtest: dtest.o libfec.a
	gcc $(CFLAGS) -g -o $@ $^ -lm

myCode: myCode.o libfec.a
	gcc $(CFLAGS) -g -o $@ $^ -lm
easyViterbi: testEasyViterbi.o easyViterbi.o libfec.a
	gcc $(CFLAGS) -g -o $@ $^ -lm

vtest27: vtest27.o libfec.a
	gcc $(CFLAGS) -g -o $@ $^ -lm

vtest29: vtest29.o libfec.a
	gcc $(CFLAGS) -g -o $@ $^ -lm

vtest39: vtest39.o libfec.a
	gcc $(CFLAGS) -g -o $@ $^ -lm

vtest615: vtest615.o libfec.a
	gcc $(CFLAGS) -g -o $@ $^ -lm

rstest: rstest.o libfec.a
	gcc $(CFLAGS) -g -o $@ $^

rs_speedtest: rs_speedtest.o libfec.a
	gcc $(CFLAGS) -g -o $@ $^	

# for some reason, the test programs without args segfault on the PPC with -O2 optimization. Dunno why - compiler bug?

myCode.o: myCode.c fec.h
	gcc $(CFLAGS) -g -c $<
testEasyViterbi.o: testEasyViterbi.c easyViterbi.c fec.h
	gcc $(CFLAGS) -g -c $<

vtest27.o: vtest27.c fec.h
	gcc $(CFLAGS) -g -c $<

vtest29.o: vtest29.c fec.h
	gcc $(CFLAGS) -g -c $<

vtest39.o: vtest39.c fec.h
	gcc $(CFLAGS) -g -c $<

vtest615.o: vtest615.c fec.h
	gcc $(CFLAGS) -g -c $<

libfec.a: $(LIBS)
	ar rv $@ $^
	ranlib libfec.a

# for Darwin
libfec.dylib: $(LIBS)
	$(CC) -dynamiclib -install_name $@ -o $@ $^

# for Linux et al
libfec.so: $(LIBS)
	gcc -fPIC -shared -Xlinker -soname=$@ -o $@ -Wl,-whole-archive $^ -Wl,-no-whole-archive -lc

dotprod.o: dotprod.c fec.h

dotprod_port.o: dotprod_port.c fec.h

viterbi27.o: viterbi27.c fec.h

viterbi27_port.o: viterbi27_port.c fec.h

viterbi29.o: viterbi29.c fec.h

viterbi39.o: viterbi39.c fec.h

viterbi39_port.o: viterbi39_port.c fec.h

viterbi39_sse2.o: viterbi39_sse2.c fec.h

viterbi39_sse.o: viterbi39_sse.c fec.h

viterbi39_mmx.o: viterbi39_mmx.c fec.h

encode_rs_char.o: encode_rs_char.c char.h rs-common.h

encode_rs_int.o: encode_rs_int.c int.h rs-common.h

encode_rs_8.o: encode_rs_8.c fixed.h

encode_rs_av.o: encode_rs_av.c fixed.h

decode_rs_char.o: decode_rs_char.c char.h rs-common.h

decode_rs_int.o: decode_rs_int.c int.h rs-common.h

decode_rs_8.o: decode_rs_8.c fixed.h

init_rs_char.o: init_rs_char.c char.h rs-common.h

init_rs_int.o: init_rs_int.c int.h rs-common.h

ccsds_tab.o: ccsds_tab.c

ccsds_tab.c: gen_ccsds
	./gen_ccsds > ccsds_tab.c

gen_ccsds: gen_ccsds.o init_rs_char.o
	gcc $(CFLAGS) -o $@ $^

gen_ccsds.o: gen_ccsds.c
	gcc  $(CFLAGS) -c -o $@ $<

ccsds_tal.o: ccsds_tal.c

ccsds_tal.c: gen_ccsds_tal
	./gen_ccsds_tal > ccsds_tal.c

exercise_char.o: exercise.c
	gcc $(CFLAGS) -c -o $@ $<

exercise_int.o: exercise.c
	gcc -DBIGSYM=1 $(CFLAGS) -c -o $@ $<

exercise_8.o: exercise.c
	gcc -DFIXED=1 $(CFLAGS) -c -o $@ $<

exercise_ccsds.o: exercise.c
	gcc -DCCSDS=1 $(CFLAGS) -c -o $@ $<

viterbi27.o: viterbi27.c fec.h

viterbi27_port.o: viterbi27_port.c fec.h

viterbi27_av.o: viterbi27_av.c fec.h

viterbi27_mmx.o: viterbi27_mmx.c fec.h
	gcc $(CFLAGS) -mmmx -c -o $@ $<

viterbi27_sse.o: viterbi27_sse.c fec.h
	gcc $(CFLAGS) -msse -c -o $@ $<

viterbi27_sse2.o: viterbi27_sse2.c fec.h
	gcc $(CFLAGS) -msse2 -c -o $@ $<

viterbi29.o: viterbi29.c fec.h

viterbi29_port.o: viterbi29_port.c fec.h

viterbi29_av.o: viterbi29_av.c fec.h

viterbi29_mmx.o: viterbi29_mmx.c fec.h
	gcc $(CFLAGS) -mmmx -c -o $@ $<

viterbi29_sse.o: viterbi29_sse.c fec.h
	gcc $(CFLAGS) -msse -c -o $@ $<

viterbi29_sse2.o: viterbi29_sse2.c fec.h
	gcc $(CFLAGS) -msse2 -c -o $@ $<

viterbi39.o: viterbi39.c fec.h

viterbi39_port.o: viterbi39_port.c fec.h

viterbi39_av.o: viterbi39_av.c fec.h

viterbi39_mmx.o: viterbi39_mmx.c fec.h
	gcc $(CFLAGS) -mmmx -c -o $@ $<

viterbi39_sse.o: viterbi39_sse.c fec.h
	gcc $(CFLAGS) -msse -c -o $@ $<

viterbi39_sse2.o: viterbi39_sse2.c fec.h
	gcc $(CFLAGS) -msse2 -c -o $@ $<

viterbi615.o: viterbi615.c fec.h

viterbi615_port.o: viterbi615_port.c fec.h

viterbi615_av.o: viterbi615_av.c fec.h

viterbi615_mmx.o: viterbi615_mmx.c fec.h
	gcc $(CFLAGS) -mmmx -c -o $@ $<

viterbi615_sse.o: viterbi615_sse.c fec.h
	gcc $(CFLAGS) -msse -c -o $@ $<

viterbi615_sse2.o: viterbi615_sse2.c fec.h
	gcc $(CFLAGS) -msse2 -c -o $@ $<

cpu_mode_x86.o: cpu_mode_x86.c fec.h

cpu_mode_x86_64.o: cpu_mode_x86_64.c fec.h

cpu_mode_ppc.o: cpu_mode_ppc.c fec.h

#%.o:   %.s
#	$(AS) $< -o $@



clean:
	rm -f *.o $(SHARED_LIB) *.a rs_speedtest peaktest sumsq_test dtest easyViterbi myCode vtest27 vtest29 vtest39 vtest615 rstest ccsds_tab.c ccsds_tal.c gen_ccsds gen_ccsds_tal core
	rm -rf autom4te.cache

distclean: clean
	rm -f config.log config.cache config.status config.h makefile

