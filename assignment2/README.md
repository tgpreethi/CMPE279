Contains 3 files
- server.c
- client.c
- child.c

Build child.c

    gcc -o child child.c

Build server.c and run the binary using root. Eg:

    gcc -o server server.c
    sudo ./server

Build client.c and run the binary

    gcc -o client client.c
    ./client
