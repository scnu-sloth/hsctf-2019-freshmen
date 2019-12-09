all: rsa
rsa: babyRSA.c
	gcc --std=c99 -m32 -fno-stack-protector -no-pie ./babyRSA.c -o babyRSA
	strip babyRSA

clean:
	rm ./babyRSA
