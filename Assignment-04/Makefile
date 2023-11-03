run : all clean

all:
	gcc  -m32 -no-pie -nostdlib -o sum sum.c
	gcc -m32 -o loader loader.c
	./loader sum
clean:
	rm sum
	rm loader


# all:
# 	gcc  -m32 -no-pie -nostdlib -o fib fib.c
# 	gcc -m32 -o loader loader.c
# 	./loader fib
# clean:
# 	rm fib
# 	rm loader


# all:
# 	gcc  -m32 -no-pie -nostdlib -o prime prime.c
# 	gcc -m32 -o loader loader.c
# 	./loader prime
# clean:
# 	rm prime
# 	rm loader