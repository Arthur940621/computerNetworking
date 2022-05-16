#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024 * 10
#define SMALL_BUF 100

using namespace std;

void* request_handler(void* arg);
void send_data(FILE* fp, char* ct, char* file_name);
char const* content_type(char* file);
void send_error(FILE* fp);
void error_handling(const string& buf);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }

    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1) {
        error_handling("bind() error!");
    }
    if (listen(serv_sock, 5)) {
        error_handling("listen() error!");
    }

    sockaddr_in clnt_addr;
    socklen_t clnt_addr_sz = sizeof(clnt_addr);
    while (1) {
        int clnt_sock = accept(serv_sock, reinterpret_cast<sockaddr*>(&clnt_addr), &clnt_addr_sz);
        cout << "Connection Request: " << inet_ntoa(clnt_addr.sin_addr) << ":" << ntohs(clnt_addr.sin_port) << endl;
        pthread_t t_id;
        pthread_create(&t_id, NULL, request_handler, &clnt_sock);
        pthread_detach(t_id);
    }

    close(serv_sock);
    return 0;
}

void* request_handler(void* arg) {
    int clnt_sock = *(reinterpret_cast<int*>(arg));
    FILE* clnt_read = fdopen(clnt_sock, "r");
    FILE* clnt_write = fdopen(dup(clnt_sock), "w");

    char req_line[SMALL_BUF]; // GET /index.html HTTP/1.1
    fgets(req_line, SMALL_BUF, clnt_read);
    if (strstr(req_line, "HTTP/") == NULL) { // 查看是否为 HTTP 提出的请求
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }

    char method[10];
    strcpy(method, strtok(req_line, " /")); // GET
    char file_name[30];
    strcpy(file_name, strtok(NULL, " /")); // 查看请求的文件名 index.html
    char ct[15];
    strcpy(ct, content_type(file_name)); // 查看 Content-type text/html

    if (strcmp(method, "GET") != 0) { // 查看是否为 GET 方式的请求
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }

    fclose(clnt_read);
    send_data(clnt_write, ct, file_name); // 响应
    return NULL;
}

char const* content_type(char* file) {
    char file_name[SMALL_BUF];
    strcpy(file_name, file);
    strtok(file_name, ".");

    char extension[SMALL_BUF];
    strcpy(extension, strtok(NULL, ".")); // 获取文件后缀

    if (!strcmp(extension, "html") || !strcmp(extension, "htm")) {
        return "text/html";
    } else {
        return "text/plain";
    }
}

void send_error(FILE* fp) {
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server\r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
        "<body><font size=+5><br>An error has occurred! View the request file name and request method!"
        "</font></body></html>";
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fputs(content, fp);
    fflush(fp);
}

void send_data(FILE* fp, char* ct, char* file_name) {
    char protocol[] ="HTTP/1.0 200 0K\r\n";
    char server[] = "Server:Linux Web Server\r\n";
    char cnt_len[] = "Content-length:2048\r\n";

    char cnt_type[SMALL_BUF];
    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    FILE* send_file;
    send_file = fopen(file_name, "r");
    if(send_file == NULL) {
        send_error(fp);
        return;
    }

    // 传输头信息
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);

    // 传输请求数据
    char buf[BUF_SIZE];
    while(fgets(buf, BUF_SIZE, send_file) != NULL) {
        fputs(buf, fp);
        fflush(fp);
    }
}

void error_handling(const string& buf) {
    cerr << buf << endl;
    exit(1);
}