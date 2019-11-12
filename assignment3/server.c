// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pwd.h>
#include<fcntl.h>
// #define PORT 8080 
int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    pid_t child_pid, f_pid;
    int status;
    int PORT = atoi(argv[1]);
    int file_descriptor;
    char fd[11];

    if(argc!=3){
        perror("Log: Parent Process: Not all arguments are provided\n ");
        _exit(2);
    }

    printf("Log: Parent Process: Current pid = %d\n",getpid());
    printf("Log: Parent Process: Current uid = %ld\n",(long) getuid());   

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }
    
    //Open file provided in the command line argument
    file_descriptor = open(argv[2],O_RDONLY );
    //Exit if file does not exist 
    if(file_descriptor == -1){
        perror("file");
        exit(EXIT_FAILURE);
    }
    
    // Forking
    child_pid = fork();                                                                                                                                              
    if( child_pid == 0 ){
        // fork returns 0 inside child process
        printf("Log: Child Process: forked child_pid = %d\n", child_pid);
        sprintf(fd,"%d",file_descriptor);
        char * exec_args[4]={"./child",(char*)&new_socket, fd ,NULL};
        execv(exec_args[0],exec_args);
        perror("Log: Child Process: execv failed\n");
        _exit(2); 

    }
    else if (child_pid > 0){
        //fork return PID of child process inside parent
        //Waiting for child process to complete
        printf("Log: Parent Process: Waiting for child process to complete\n");
        if((f_pid = wait(&status)) < 0){
            perror("Log: Parent Process: Error in wait");
            _exit(1);
        }
        close(file_descriptor);
        printf("Log: Parent Process: Forked child pid: %d\n", child_pid); 
        printf("Log: Parent Process: Current pid = %d\n",getpid());
        printf("Log: Parent Process: Current uid = %ld\n",(long) getuid());   
        printf("Log: Parent Process: Completed\n");

    }
    else{
        perror("Error! fork() failed\n");
        _exit(2);                                                                                                                                          
    }
    return 0; 
} 
