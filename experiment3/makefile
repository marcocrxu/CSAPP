GID="SA21011244"

fizz:
	./sendstring < exploit-fizz.txt > exploit-raw.txt

smoke:
	./sendstring < exploit-smoke.txt > exploit-raw.txt

all:
	./bufbomb -t ${GID} < exploit-raw.txt

bang:
	gcc -c bang.s
	objdump -d bang.o > bang.d	
	./sendstring < exploit-bang.txt > exploit-raw.txt

dynamite:
	gcc -c dynamite.s
	objdump -d dynamite.o > dynamite.d
	./sendstring < exploit-dynamite.txt > exploit-raw.txt

clean:
	rm -rf ./exploit-raw.txt
