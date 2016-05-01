all: myClient.c serv.c
	gcc -W -Wall -Wextra -m32 -O2 -pthread myClient.c  -o client.exe
	gcc -W -Wall -Wextra -m32 -O2  serv.c  -o serv.exe

clean:
	$(RM) client.exe server.exe
