/*Jorge Bello Martín, Redes y Videojuegos en Red, Universidad Complutense de Madrid
2021*/

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*MAN
struct addrinfo {
               int              ai_flags;
               int              ai_family;
               int              ai_socktype;
               int              ai_protocol;
               socklen_t        ai_addrlen;
               struct sockaddr *ai_addr; 
               char            *ai_canonname;
               struct addrinfo *ai_next;
           };
int getaddrinfo(const char *node, const char *service,
                       const struct addrinfo *hints,
                       struct addrinfo **res);
return 0 si no da error. -->const char *gai_strerror(int errcode); devuelve el codigo de error pasandole el return de getaddrinfo
devuelve en **res el resultado.

int getnameinfo(const struct sockaddr *addr, socklen_t addrlen,
                       char *host, socklen_t hostlen,
                       char *serv, socklen_t servlen, int flags);
*/
int main(int argc, char**argv){

    struct addrinfo hints;
    struct addrinfo * res;

    memset((void*) &hints, 0, sizeof (struct addrinfo)); //inicializa a 0 el struct
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int returnCode = getaddrinfo(argv[1], argv[2],  //node, service
                       &hints, &res);               //hints, resultado

    //Manejo de errores:
    if (returnCode !=0){
        std::cerr << "getaddrinfo: " << gai_strerror(returnCode) << std::endl; 
        return -1;
    }

    //recorre todos los addrinfo que ha devuelto hasta que no queda ninguno
    for (auto it = res ; it !=nullptr; it = it->ai_next){
        char host[NI_MAXHOST]; //contenedores de la información sacada por getnameinfo
        char serv[NI_MAXSERV];
        int nameInfo = getnameinfo(it->ai_addr, it->ai_addrlen , 
                                    host, NI_MAXHOST,
                                    serv, NI_MAXSERV, NI_NUMERICHOST+NI_NUMERICSERV);
        
        std::cout << "Host: "<< host << " Port: " << serv << std::endl;
    }

    freeaddrinfo(res); //liberar memoria
    return 0;
}