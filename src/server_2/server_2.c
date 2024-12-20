#include "../common.h"

#define PORT 8081

void *connection_handler(void *socket_desc);
void command_handler(char *command, int sock, char* old_worktime, char *old_count_threads);
int get_server_work_time(char *dest_str);
int get_count_threads(char *dest_str);


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
    //char command_buffer[BUFFER_SIZE];
    int read_size = 1;

    send(sock, "Соединение установлено на сервере 2\n", strlen("Соединение установлено на сервере 2\n"), 0);

    char worktime[BUFFER_SIZE] = {0};
    char count_threads[BUFFER_SIZE] = {0};

    // Чтение данных от клиента
    while (read_size > 0) {
        char command_buffer[BUFFER_SIZE] = {0};
        read_size = read(sock, command_buffer, BUFFER_SIZE);
        command_handler(command_buffer, sock, worktime, count_threads);
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

void command_handler(char *command, int sock, char* old_worktime, char *old_count_threads) {
    char new_worktime[BUFFER_SIZE];
    char new_count_threads[BUFFER_SIZE];

    get_server_work_time(new_worktime);
    get_count_threads(new_count_threads);

    if (strcmp(command, "GET_WORKTIME") == 0)
    {
        //send_server_work_time(sock);
        send_message(sock, WORKTIME);
    }
    else if (strcmp(command, "GET_COUNT_THREADS") == 0)
    {
        //send_count_threads(sock);
        send_message(sock, COUNT_THREADS);
    }
    else if ((strcmp(command, "GET_WORKTIME_2") == 0) && (strcmp(old_worktime, new_worktime) != 0))
    {
        strcpy(old_worktime, new_worktime);
        send_message(sock, WORKTIME);
    }
    else if ((strcmp(command, "GET_COUNT_THREADS_2") == 0) && (strcmp(old_count_threads, new_count_threads) != 0))
    {
        //send_display_resolution(sock);
        strcpy(old_count_threads, new_count_threads);
        send_message(sock, COUNT_THREADS);
    }
    else {
        send(sock, "nothing", 8, 0);
        printf("Nothing change\n");
    }
}
