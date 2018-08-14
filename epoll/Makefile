PROGS= client server-lt server-et server-et-fixed

all:  bin $(PROGS)

%:	%.c
	gcc -D_GNU_SOURCE $@.c -o bin/$@

bin:
	mkdir bin

clean:
	rm -rf bin