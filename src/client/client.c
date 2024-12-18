#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER1_PORT 8080
#define SERVER2_PORT 8081
#define BUFFER_SIZE 1024

void connect_to_server(int port, int *sock);
void disconnect_from_server(int *sock);
void send_command(int sock, char *command);
// void get_server_1_window_position();

int sock1 = -1; // Сокет для первого сервера
int sock2 = -1; // Сокет для второго сервера

int main() {
    int choice;
    printf("\nВыберите действие:\n");
    printf("1. Подключиться к серверу 1\n");
    printf("2. Подключиться к серверу 2\n");
    printf("3. Отключиться от сервера 1\n");
    printf("4. Отключиться от сервера 2\n");
    printf("5. Получить координаты окна серверного процесса\n");
    printf("6. Получить разрешение основного монитора\n");
    printf("7. Получить время работы серверного процесса в минутах\n");
    printf("8. Получить количество потоков серверного процесса\n");
    printf("9. Выход\n");

    while (1) {

        printf("Ваш выбор: ");
        
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                connect_to_server(SERVER1_PORT, &sock1);
                break;
            case 2:
                connect_to_server(SERVER2_PORT, &sock2);
                break;
            case 3:
                disconnect_from_server(&sock1);
                break;
            case 4:
                disconnect_from_server(&sock2);
                break;
            case 5:
                send_command(sock1, "GET_SIZE_WINDOW");
                break;
            case 6:
                send_command(sock1, "GET_DISPLAY_RESOLUTION");
                break;
            case 7:
                send_command(sock2, "GET_WORKTIME");
                break;
            case 8:
                send_command(sock2, "GET_COUNT_THREADS");
                break;
            case 9:
                disconnect_from_server(&sock1);
                disconnect_from_server(&sock2);
                exit(0);
            default:
                printf("Неверный выбор. Пожалуйста, попробуйте снова.\n");
        }
    }

    return 0;
}

void connect_to_server(int port, int *sock) {
    struct sockaddr_in serv_addr;

    if (*sock != -1) { // Проверка активного соединения
        printf("Уже подключено к серверу на порту %d\n", port);
        return;
    }
    
    if ((*sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Ошибка создания сокета \n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Преобразование IPv4 адреса из текстового представления в бинарное
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nНеверный адрес/Адрес не поддерживается \n");
        return;
    }

    if (connect(*sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nОшибка подключения к серверу на порту %d\n", port);
        close(*sock);
        *sock = -1; // Установка сокета в недоступное состояние
        return;
    }

    char buffer[BUFFER_SIZE] = {0};
    read(*sock, buffer, BUFFER_SIZE);
    printf("%s", buffer);
}

void disconnect_from_server(int *sock) {
    if (*sock != -1) {
        close(*sock);
        printf("Соединение разорвано\n");
        *sock = -1; // Установка сокета в недоступное состояние
    } else {
        printf("Нет активного соединения для разрыва\n");
    }
}

void send_command(int sock, char *command) {
    if (sock != -1)
    {
        send(sock, command, strlen(command), 0);
        char buffer[BUFFER_SIZE] = {0};
        if (read(sock, buffer, BUFFER_SIZE)) {
            printf("%s\n", buffer);
        } else {
            printf("Ошибка чтения данных.\n");
        }
    } 
    else {
        printf("Нет подключения к серверу\n");
    }
}