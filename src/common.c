#include "common.h"

char *get_current_time(char str_time[21]) {
    time_t current_time = time(NULL);
    struct tm *local_time = localtime(&current_time);

    //char str_time[12];
    sprintf(str_time, "%02d.%02d.%2d - %02d:%02d:%02d: ",local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900, 
                                                            local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

    return str_time;
}

int send_message(int sock, int type) {
    int result = 1;

    char send_buffer[BUFFER_SIZE] = {0};

    if (type == SIZE_WINDOW)
    {
        if (!get_size_window(send_buffer)) {
            result = 0;
        }
    }
    else if (type == DISPLAY_RESOLUTION)
    {
        if (!get_display_resolution(send_buffer)) 
        {
            result = 0;
        }
        
    }
    else if (type == WORKTIME)
    {
        if (!get_server_work_time(send_buffer)) 
        {
            result = 0;
        }
    }
    else if (type == COUNT_THREADS)
    {
        if (!get_count_threads(send_buffer)) 
        {
            result = 0;
        }
    }

    char send_message[BUFFER_SIZE] = {0};
    char time[21];
    get_current_time(time);

    strcat(send_message, time);
    strcat(send_message, send_buffer);
    
    send(sock, send_message, strlen(send_message) + 1, 0); 
    
    return result;
}

int get_display_resolution(char *dest_str) {
    int result = 1;

    FILE *fp = popen("xrandr | grep '*' | awk '{print $1}'", "r");

    if (fgets(dest_str, BUFFER_SIZE, fp) != NULL) {
        // Удаляем конец строки
        //printf("%s", dest_str);
        dest_str[strcspn(dest_str, "\n")] = 0;
        //printf("%s", dest_str);
        pclose(fp);
    } else {
        result = 0;
    }

    return result;
}

int get_size_window(char *dest_str) {
    int result = 1; 

    FILE *fp = popen("stty size", "r");

    if (fgets(dest_str, BUFFER_SIZE, fp) != NULL) {
        // Удаляем конец строки
        dest_str[strcspn(dest_str, "\n")] = 0;
        pclose(fp);
    } else {
        result = 0;
    }

    return result;
}

int get_count_threads(char *dest_str) {
    int result = 1;

    pid_t pid = getpid(); 
    char command[BUFFER_SIZE];
    sprintf(command, "cat /proc/%d/status | grep Threads | awk '{print $2}'", pid);
    FILE *fp = popen(command, "r");

    if (fgets(dest_str, BUFFER_SIZE, fp) != NULL) {
        // Удаляем конец строки
        dest_str[strcspn(dest_str, "\n")] = 0;
        pclose(fp);
    } else {
        result = 0;
    }

    return result;
}

int get_server_work_time(char *dest_str) {
    int result = 1;

    pid_t pid = getpid(); 
    char command[BUFFER_SIZE];
    //sprintf(command, "ps -eo pid,comm,etime | grep %d | awk '{split($3,a,\"-\"); split(a[1],b,\":\"); if (length(a) > 1); then {print a[1]*24*60 + b[1]*60 + b[2]} elif (length(b) > 2); then {print b[1]*60 + b[2]} elif (length(b) == 2); then {print b[0]} else {print 0 fi}}'", pid);
    sprintf(command, "./get_time.sh %d", pid);


    FILE *fp = popen(command, "r");

    if (fgets(dest_str, BUFFER_SIZE, fp) != NULL) {
        // Удаляем конец строки
        dest_str[strcspn(dest_str, "\n")] = 0;
        pclose(fp);
    } else {
        result = 0;
    }

    return result;
}

