#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
//needed for IO things. Attention that this is different from kernel mode int lcd;

#define SCULL_IOC_MAGIC 'k'
#define SCULL_HELLO _IO(SCULL_IOC_MAGIC, 1)
#define SET_DEV_MSG _IOW(SCULL_IOC_MAGIC, 2, char*)
#define GET_DEV_MSG _IOR(SCULL_IOC_MAGIC, 3, char*)
#define WR_DEV_MSG _IOWR(SCULL_IOC_MAGIC, 4, char*)

int lcd;
void test() 
{ 
    int k, i, sum; 
    char s[3]; 
    memset(s, '2', sizeof(s)); 
    printf("test begin!\n"); 
    k = write(lcd, s, sizeof(s)); 
    printf("written = %d\n", k); 
    k = ioctl(lcd, SCULL_HELLO); 
    printf("result = %d\n", k); 

    char *message = "This is a device message\n";
    if(k = ioctl(lcd, SET_DEV_MSG, message)){
        printf("ioctl set msg fail\n");
        return ;
    }
    char *user_msg = (char *)malloc(60);
    if(!user_msg){
        printf("NO MEMORY\n");
        return ;
    }
    char *user_msg_tmp = (char *)malloc(60);
    if(k = ioctl(lcd, GET_DEV_MSG, user_msg_tmp)){
        printf("_IOR ioctl get msg fail\n");
        return ;
    }
    printf("_IOR user_msg is: %s \n", user_msg_tmp);
    free(user_msg_tmp);

    char *new_user_msg = "This is new message!\n";
    strcpy(user_msg, new_user_msg);
    if(k = ioctl(lcd, WR_DEV_MSG, user_msg)){
        printf("_IOWR ioctl get msg fail\n");
        return ;
    }
    printf("_IOWR user_msg is: %s \n", user_msg);
    free(user_msg);

} 

int main(int argc, char **argv) 
{ 
    lcd = open("/dev/onebyte", O_RDWR); 
    if (lcd == -1) {
        perror("unable to open onebyte"); 
        exit(EXIT_FAILURE); 
    } 
    test(); 
    close(lcd); 
    return 0; 
} 

