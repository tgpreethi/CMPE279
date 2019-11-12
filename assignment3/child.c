#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pwd.h>
#include<fcntl.h>
#include<sys/stat.h>
 
int main(int argc, char const *argv[]) {  

    struct passwd* passwd_ptr;
    int valread;
    char buffer[1024] = {0}; 
    // char *hello = "Hello from server";
    int file_descriptor;
    char* file_data;
    off_t file_size;
    off_t file_start;
    int file_read;
    struct stat st;
    char cwd[1024];
// fork returns 0 inside child process
    if(argc!=3){
        perror("Log: inside child.c: Not all arguments are provided\n ");
        _exit(2);
    }
    printf("Log: inside child.c: Current pid = %d\n",getpid());
    printf("Log: inside child.c: Current uid = %ld\n", (long) getuid()); 
    passwd_ptr = getpwnam("nobody");
    printf("Log: inside child.c: UID of nobody=%ld\n",(long) passwd_ptr->pw_uid);

    printf("Log: inside child.c: directory of nobody=%s\n",passwd_ptr->pw_dir);
    //Check if directory exists, it not, create directory
    if (stat(passwd_ptr->pw_dir, &st) == 0) {
        printf("Log: inside child.c: Directory exists\n");
    }
    else{
        int status = mkdir(passwd_ptr->pw_dir,100);
        if(status!=0){
            perror("Log: inside child.c: error in mkdir\n");
            _exit(2);
        }
        printf("Log: inside child.c: Directory created\n");

    }
    //change directory
    chdir(passwd_ptr->pw_dir);
    getcwd(cwd, sizeof(cwd));
    printf("Log: inside child.c: Current working dir: %s\n", cwd);
    //change root
    if(chroot(passwd_ptr->pw_dir)!=0){
        perror("Log: inside child.c: chroot() error\n");
        _exit(2);
    }
    getcwd(cwd, sizeof(cwd));
    printf("Log: inside child.c: Current working dir after chroot(): %s\n", cwd);
    //change uid
    if (setuid(passwd_ptr->pw_uid) != 0) {
        perror("Log: inside child.c: setuid() error\n");
        _exit(2);
    }
    else
        printf("Log: inside child.c: UID after setuid() = %ld\n",(long) getuid());

    file_descriptor = atoi(argv[2]);
    file_size = lseek(file_descriptor,0,SEEK_END); //get size of the file
    file_start = lseek(file_descriptor,0,SEEK_SET); //move the cursor to start of file
    file_data = (char*)calloc(file_size, sizeof(char));
    file_read = read(file_descriptor,file_data,file_size);

    valread = read((int)*argv[1], buffer, 1024);
    printf("%s\n",buffer );

    send((int)*argv[1], file_data , strlen(file_data) , 0 ); 
    printf("Hello message sent\n");
    free(file_data);
    return 0;
}