obj-m := pseudo.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

testpseudo: testpseudo.c
	gcc -Wall testpseudo.c -o testpseudo

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f testpseudo
