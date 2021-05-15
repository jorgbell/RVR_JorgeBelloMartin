/*Jorge Bello Martín, Redes y Videojuegos en Red, Universidad Complutense de Madrid
2021*/

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
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
    int bnd = bind(sck, res->ai_addr, res->addrlen);     

    //------------------ 
    //COMUNICACIÓN
    while (true){                                                   
        int len = 80;                                          
        char buffer[len];        //buffer de 80 posiciones de memoria
        struct sockaddr client;  //al ser un parametro de salida no hace falta inicializarlo
        socklen_t clientLen = sizeof(struct sockaddr); //al ser un parametro de e/s hay que inicializarlo

        int bytes = recvfrom(sck, (void * ) buffer, len, 0 , &client, &clientLen); 
        if(bytes == -1){
            return -1;
        }
        print(&client, &clientLen);
        std::cout << "\tData: "<< buffer << std::endl;

        sendto(sck, buffer, bytes, 0, &client, clientLen);        
    }
    close(sck);
    freeaddrinfo(res); //liberar memoria
    return 0;
}

void print(const struct sockaddr *addr, socklen_t addrlen){
    char host[NI_MAXHOST]; //contenedores de la información sacada por getnameinfo
    char serv[NI_MAXSERV];
    int nameInfo = getnameinfo(addr, addrlen , 
                                host, NI_MAXHOST,
                                serv, NI_MAXSERV, NI_NUMERICHOST+NI_NUMERICSERV);
        
    std::cout << "Host: "<< host << " Port: " << serv << std::endl;
}