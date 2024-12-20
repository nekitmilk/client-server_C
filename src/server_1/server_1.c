#include "../common.h"

#define PORT 8080

void *connection_handler(void *socket_desc);
void command_handler(char *command, int sock, char *old_size_window, char *old_display_resolution);
int get_display_resolution(char *dest_str);
int get_size_window(char *str_position);




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

    printf("Сервер 1 запущен на порту %d\n", PORT);

    while (1) {
        // Принятие соединения
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("Соединение установлено с клиентом на сервере 1\n");
        

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
    int read_size = 1;

    send(sock, "Соединение установлено на сервере 1\n", strlen("Соединение установлено на сервере 1\n"), 0);

    char size_window[BUFFER_SIZE] = {0};
    char display_resolution[BUFFER_SIZE] = {0};

    // Чтение данных от клиента
    while (read_size > 0) {
        char command_buffer[BUFFER_SIZE] = {0};
        read_size = read(sock, command_buffer, BUFFER_SIZE);
        command_handler(command_buffer, sock, size_window, display_resolution);
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

void command_handler(char *command, int sock, char *old_size_window, char *old_display_resolution) {

    char new_size_window[BUFFER_SIZE];
    char new_display_resolution[BUFFER_SIZE];

    get_size_window(new_size_window);
    get_display_resolution(new_display_resolution);

    // printf("%s %s\n", old_size_window, new_size_window);
    // printf("%d\n", strcmp(old_size_window, new_size_window) == 0);

    if (strcmp(command, "GET_SIZE_WINDOW") == 0)
    {
        //send_size_window(sock);
        send_message(sock, SIZE_WINDOW);
    }
    else if (strcmp(command, "GET_DISPLAY_RESOLUTION") == 0)
    {
        //send_display_resolution(sock);
        send_message(sock, DISPLAY_RESOLUTION);
    }
    else if ((strcmp(command, "GET_SIZE_WINDOW_1") == 0) && (strcmp(old_size_window, new_size_window) != 0))
    {
        strcpy(old_size_window, new_size_window);
        send_message(sock, SIZE_WINDOW);
    }
    else if ((strcmp(command, "GET_DISPLAY_RESOLUTION_1") == 0) && (strcmp(old_display_resolution, new_display_resolution) != 0))
    {
        //send_display_resolution(sock);
        strcpy(old_display_resolution, new_display_resolution);
        send_message(sock, DISPLAY_RESOLUTION);
    }
    else {
        send(sock, "nothing", 8, 0);
        printf("Nothing change\n");
    }

}