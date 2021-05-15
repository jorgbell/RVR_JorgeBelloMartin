/*Jorge Bello Martín, Redes y Videojuegos en Red, Universidad Complutense de Madrid
2021*/

#include <iostream>
#include <string.h>
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
int main(int argc, char**argv){

    struct addrinfo hints;
    struct addrinfo * res;

    memset((void*) &hints, 0, sizeof (struct addrinfo)); //inicializa a 0 el struct
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int returnCode = getaddrinfo(argv[1], argv[2],  //node, service
                       &hints, &res);               //hints, resultado

    //Manejo de errores:
    if (returnCode !=0){
        std::cerr << "getaddrinfo: " << gai_strerror(returnCode) << std::endl; 
        return -1;
    }

    int socket(res-)
    freeaddrinfo(res); //liberar memoria
    return 0;
}