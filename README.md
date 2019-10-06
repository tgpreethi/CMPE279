# CMPE279
CMPE279 Assignments

## Assignment 1

Extend the server code to use privilege separation. You should accomplish this by splitting up the code into two logical parts – the part that sets up the socket and a separate part that processes the data from the client. Once you locate this split, you should fork and have the child process setuid() to drop its privileges to an unprivileged user (maybe the “nobody” user, or a user you create). The server should wait for the child to exit and the child should process the connection from the client.
