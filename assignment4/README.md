Contains 3 files
- server.c
- client.c
- child.c

Install libseccomp-dev

    sudo apt-get libseccomp-dev

Build child.c

    gcc -o child child.c

Build server.c and run the binary using root. Eg:

    gcc -o server server.c -lseccomp
    sudo ./server <port number> <file path>

Build client.c and run the binary

    gcc -o client client.c -lseccomp
    ./client <port number>
