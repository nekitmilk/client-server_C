#include "logger.h"

#define NAME1 "server_1.log"
#define NAME2 "server_2.log"
#define FILE1 "/tmp/server_1"
#define FILE2 "/tmp/server_2"

void *write_log(void *arg);
void *out(void *arg);

int main() {    
    pthread_t serv1;
    pthread_t serv2;
    char *name1 = NAME1;
    char *name2 = NAME2;
    if (pthread_create(&serv1, NULL, write_log, (void*)name1)) {
        return 1;
    }
    pthread_detach(serv1);
    if (pthread_create(&serv2, NULL, write_log, (void*)name2)) {

        return 1;
    }
    pthread_detach(serv2);
    printf("Start log server\n");

    getchar();
    printf("End log server\n");
    return 0;
}

void *write_log(void *arg) {
    char *name = (char *)arg;
    char *file = (strcmp(name, NAME1) == 0) ? FILE1 : FILE2;
    int fd = mkfifo(file, 0666);
    if (fd == -1) {
        //perror("mkfifo");
        fd = open(file, O_RDONLY | O_TRUNC, 777);
        //perror("fd open");
        if (fd == -1) {
            return NULL;
        }
    }
    FILE *f = NULL;
    ssize_t bytes = 0;
    // if ((f = fopen(name, "a")) != NULL) {
    //     while (1) {
    //         char buffer[BUFFER_SIZE] = {0};
    //         bytes = read(fd, buffer, BUFFER_SIZE);
    //         if (bytes > 0) {
    //                 // printf("Read %lu bytes\n", bytes);
    //                 printf("%s\n", buffer);
    //                 fputs(buffer, f);
    //         } else if (bytes == -1) {
    //             printf("Read error\n");
    //             break;
    //         } 
    //     }
    //     fclose(f);
    // }
    // close(fd);

    if ((f = fopen(name, "a")) != NULL) {
        fclose(f);
        while (1) {
            char buffer[BUFFER_SIZE] = {0};
            bytes = read(fd, buffer, BUFFER_SIZE);
            if (bytes > 0) {
                    // printf("Read %lu bytes\n", bytes);
                    if ((f = fopen(name, "a")) != NULL)
                    {
                        printf("%s\n", buffer);
                        fputs(buffer, f);
                        fclose(f);
                    }
                    
            } else if (bytes == -1) {
                printf("Read error\n");
                break;
            } 
        }
    }
    close(fd);
}