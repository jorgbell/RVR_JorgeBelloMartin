/*Jorge Bello Martín, Redes y Videojuegos en Red, Universidad Complutense de Madrid
2021*/

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
/*MAN
int bind(int sockfd, const struct sockaddr *addr,
                socklen_t addrlen);
el sockaddr se saca de:
int getaddrinfo(const char *node, const char *service,
                       const struct addrinfo *hints,
                       struct addrinfo **res);

       void freeaddrinfo(struct addrinfo *res);

       const char *gai_strerror(int errcode);
int socket(int domain, int type, int protocol); //family, socktype, protocol
bind(sock, (struct sockaddr *), ai_addr, ai_addrlen)
despues de hacer bind, hacer sendto y recvfrom
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
                      const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen);
*/
//---------------------------------------------------------------------------------//
//                              SERVIDOR UDP
//---------------------------------------------------------------------------------//
int main(int argc, char**argv){

    //RECEPCION DE IP Y PUERTO
    struct addrinfo hints;
    struct addrinfo * res;

    memset((void*) &hints, 0, sizeof (struct addrinfo)); //inicializa a 0 el struct
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int returnCode = getaddrinfo(argv[1], argv[2],  //node, service
                       &hints, &res);               //hints, resultado

    
    if (returnCode !=0){ //Manejo de errores:
        std::cerr << "getaddrinfo: " << gai_strerror(returnCode) << std::endl; 
        return -1;
    }

    //------------------
    //APERTURA DEL SOCKET
    int sck = socket(res->ai_family, res->ai_socktype, 0);        
   
    if(sck == -1){  //Manejo de errores:
        std::cerr << "ERROR en socket() : creación del socket" << std::endl; 
        return -1;
    }  

    //------------------    
    //ASOCIAR DIRECCIÓN AL SOCKET  
    int bnd = bind(sck, res->ai_addr, res->ai_addrlen);     

    //------------------ 
    //COMUNICACIÓN
    bool quit = false;
    while (!quit){                                                   
        int len = 80;                                          
        char buffer[len];        //buffer de 80 posiciones de memoria
        struct sockaddr client;  //al ser un parametro de salida no hace falta inicializarlo
        socklen_t clientLen = sizeof(struct sockaddr); //al ser un parametro de e/s hay que inicializarlo

        int bytes = recvfrom(sck, (void * ) buffer, len-1, 0 , &client, &clientLen); 
        buffer[bytes] = '\0';
        if(bytes == -1){
            return -1;
        }
        char host[NI_MAXHOST]; //contenedores de la información sacada por getnameinfo
        char serv[NI_MAXSERV];
        int nameInfo = getnameinfo(&client, clientLen , 
                                    host, NI_MAXHOST,
                                    serv, NI_MAXSERV, NI_NUMERICHOST+NI_NUMERICSERV);
        
        std::cout << "Host: "<< host << " Port: " << serv << std::endl;
        std::cout << "\tData: "<< buffer << std::endl;

        //preparacion del mensaje de envío de vuelta
        std::string answer;
        char char_array[15];
        memset((void*) &char_array, 0, sizeof (char_array));
        time_t t = time(NULL);
        struct tm* res = localtime(&t);;
        if(buffer[0] == 't') { 
            strftime(char_array, sizeof(char_array), "%r", res);
            std::cout << answer << std::endl;
            strcpy(char_array, answer.c_str());
            sendto(sck, char_array, sizeof(char_array), 0, &client, clientLen);  
        }      
        else if(buffer[0] == 'd'){ 
            strftime(char_array, sizeof(char_array), "%D", res);
            std::cout << answer << std::endl;
            strcpy(char_array, answer.c_str());
            sendto(sck, char_array, sizeof(char_array), 0, &client, clientLen);  
        } 
        else if(buffer[0] == 'q'){ 
            quit = true;
            answer = "QUITTING...";
            std::cout << answer << std::endl;
            int n = answer.length();
            char c[n+1];
            memset((void*) &c, 0, sizeof (c));
            strcpy(c, answer.c_str());
            sendto(sck, c, sizeof(c), 0, &client, clientLen);  
        }
    	else{
            answer = "Commands: t (time) ; d (day) ; q (quit)";
            std::cout << answer << std::endl;
            int n = answer.length();
            char c[n+1];
            memset((void*) &c, 0, sizeof (c));
            strcpy(c, answer.c_str());
            sendto(sck, c, sizeof(c), 0, &client, clientLen);  
        }
           
    }
    close(sck);
    freeaddrinfo(res); //liberar memoria
    return 0;
}