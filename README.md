# CMPE279
CMPE279 Assignments

## Assignment 1

Extend the server code to use privilege separation. You should accomplish this by splitting up the code into two logical parts – the part that sets up the socket and a separate part that processes the data from the client. Once you locate this split, you should fork and have the child process setuid() to drop its privileges to an unprivileged user (maybe the “nobody” user, or a user you create). The server should wait for the child to exit and the child should process the connection from the client.

## Assignment 2

Starting with assignment 1 as a starting point, add in code to re-exec the server’s child process after forking. You will need to determine how to pass the socket file descriptor between the forked child and the new exec’ed child, so that the new exec’ed child can process the incoming child request.

## Assignment 3

Starting with assignment 2 as a starting point, modify the server and client code as follows:
• Accept a command line parameter to specify the port being used

• Accept an additional command line parameter on the server to specify a disk file whose content will be sent to the client instead of the static “Hello” message.

• After re-exec’ing, chroot() to an empty directory on the disk

• Open the disk file described above before fork/exec, and pass the file descriptor to the child

Questions:
1. What happens if the file being requested is not inside the chroot? Does it matter?
2. Explain the mechanism you are using to pass file descriptors between parent and child processes.
3. What happens if the file size of the disk file exceeds the size of the client’s hardcoded buffer?
Does the client have sufficient checks to ensure no buffer overruns occur?
