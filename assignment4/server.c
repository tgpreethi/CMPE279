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
#include<seccomp.h>
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
    // Initialize filter
    scmp_filter_ctx filter_ctx;
    filter_ctx = seccomp_init(SCMP_ACT_KILL); 
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(rt_sigreturn), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 0);

    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(access), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(fstat), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(mmap), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(close), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(arch_prctl), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(mprotect), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(munmap), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(socket), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(connect), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(sendto), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(getpid), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(setsockopt), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(bind), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(listen), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(clone), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(getpid), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(getuid), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(wait4), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(openat), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(accept), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(setuid), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(chroot), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(mkdir), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(stat), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(chdir), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(getcwd), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(dup2), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(lseek), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(fcntl), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit_group), 0);
    seccomp_rule_add(filter_ctx, SCMP_ACT_ALLOW, SCMP_SYS(brk), 0);
    seccomp_load(filter_ctx);

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
