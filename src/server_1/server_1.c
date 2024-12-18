#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void *connection_handler(void *socket_desc);
void command_handler(char *command, int sock);
int send_display_resolution(int sock);
int get_display_resolution(char *dest_str);
int send_window_position(int sock);
int get_window_position(char *str_position);




int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    

    // Создание сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Привязка сокета к порту
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Прослушивание соединений
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Сервер 2 запущен на порту %d\n", PORT);

    while (1) {
        // Принятие соединения
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("Соединение установлено с клиентом на сервере 2\n");
        

        pthread_t thread_id;
        int *new_sock = malloc(1);
        *new_sock = new_socket;

        // Создание нового потока для обработки клиента
        if (pthread_create(&thread_id, NULL, connection_handler, (void *)new_sock) < 0) {
            perror("Ошибка при создании потока");
            free(new_sock);
            continue;
        }

        // Не ждем завершения потока
        pthread_detach(thread_id);
    }

    return 0;
}


void *connection_handler(void *socket_desc) {
    int sock = *(int *)socket_desc;
    char command_buffer[BUFFER_SIZE];
    int read_size;

    send(sock, "Соединение установлено на сервере 2\n", strlen("Соединение установлено на сервере 2\n"), 0);

    // Чтение данных от клиента
    while ((read_size = recv(sock, command_buffer, BUFFER_SIZE, 0)) > 0) {

        command_handler(command_buffer, sock);
        
    }

    if (read_size == 0) {
        printf("Клиент отключился\n");
    } else if (read_size == -1) {
        perror("Ошибка при получении данных");
    }

    free(socket_desc);
    close(sock);
    return NULL;
}

void command_handler(char *command, int sock) {
    if (strcmp(command, "GET_WINDOW_POSITION") == 0)
    {
        send_window_position(sock);
    }
    else if (strcmp(command, "GET_DISPLAY_RESOLUTION") == 0)
    {
        send_display_resolution(sock);
    }
    else {
        const char *error_message = "Неверная команда\n";
        send(sock, error_message, strlen(error_message), 0);
    }
}

int send_display_resolution(int sock) {
    int result = 1;

    char send_buffer[BUFFER_SIZE];
    if (get_display_resolution(send_buffer)) {
        send(sock, send_buffer, strlen(send_buffer), 0); 
    }
    else {
        result = 0;
    }

    return result;
}

int get_display_resolution(char *dest_str) {
    int result = 1;

    FILE *fp = popen("xrandr | grep '*' | awk '{print $1}'", "r");

    if (fgets(dest_str, sizeof(dest_str), fp) != NULL) {
        // Удаляем конец строки
        dest_str[strcspn(dest_str, "\n")] = 0;
        pclose(fp);
    } else {
        result = 0;
    }

    return result;
}

int send_window_position(int sock) {
    int result = 1;

    char send_buffer[BUFFER_SIZE];
    if (get_window_position(send_buffer)) {
        send(sock, send_buffer, strlen(send_buffer), 0); 
    }
    else {
        result = 0;
    }

    return result;
}

int get_window_position(char *str_position) {
    int result = 1; 

    return result;
}