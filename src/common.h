#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <time.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <fcntl.h>

#define BUFFER_SIZE 1024

#define SIZE_WINDOW 1
#define DISPLAY_RESOLUTION 2
#define WORKTIME 3
#define COUNT_THREADS 4

char *get_current_time(char str_time[12]); 

int send_message(int sock, int type);
int get_display_resolution(char *dest_str);
int get_size_window(char *str_position);
int get_server_work_time(char *dest_str);
int get_count_threads(char *dest_str);


typedef struct 
{
    int *running;
    int sock;
} thread_input;



#endif /* COMMON_H */

