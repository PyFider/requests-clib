#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFLEN 1024

int request(const char* method, const char* url, const char* data) {
  // Разбиваем URL на хост и путь
  char host[256];
  char path[256];
  sscanf(url, "http://%255[^/]/%255s", host, path);

  // Создаем сокет
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    return -1;
  }

  // Получаем адрес сервера
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(80);
  if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
    /* inet_pton */
    close(sock);
		return 200;
  }

  // Подключаемся к серверу
  if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    /* CONNECT */
		close(sock);
    return -1;
  }

  // Формируем HTTP-запрос
  char request[BUFLEN];
  int request_len = snprintf(request, BUFLEN, "%s /%s HTTP/1.1\r\nHost: %s\r\n", method, path, host);
  if (data)
		request_len += snprintf(request + request_len, BUFLEN - request_len, "Content-Length: %zu\r\n\r\n%s", strlen(data), data);
	else
		request_len += snprintf(request + request_len, BUFLEN - request_len, "\r\n");

  // Отправляем запрос
  if (send(sock, request, request_len, 0) < 0) {
    /* Send */
    close(sock);
    return -1;
  }

  // Получаем ответ
	char response[BUFLEN];
  int bytes_read = recv(sock, response, BUFLEN-1, 0);
  if (bytes_read < 0) {
		perror("recv");
    close(sock);
    return -1;
  }
  response[bytes_read] = '\0';

  // Закрываем соединение
  close(sock);
  return 200;
}

