/*Jorge Bello Martín, Redes y Videojuegos en Red, Universidad Complutense de Madrid
2021*/

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>

//---------------------------------------------------------------------------------//
//                              SERVIDOR TCP
//---------------------------------------------------------------------------------//
int main(int argc, char**argv){

    //RECEPCION DE IP Y PUERTO
    struct addrinfo hints;
    struct addrinfo * res;

    memset((void*) &hints, 0, sizeof (struct addrinfo)); //inicializa a 0 el struct
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

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
    //LISTEN
    listen(sck, 16);
    //ACCEPT DE LÑA CONEXION CON EL CLIENTE
    struct sockaddr client;  //al ser un parametro de salida no hace falta inicializarlo
    socklen_t clientLen = sizeof(struct sockaddr); //al ser un parametro de e/s hay que inicializarlo
    int client_sck = accept(sck, &client, &clientLen);
    //COMUNICACIÓN
    char host[NI_MAXHOST]; //contenedores de la información sacada por getnameinfo
    char serv[NI_MAXSERV];
    int nameInfo = getnameinfo(&client, clientLen , 
                                host, NI_MAXHOST,
                                serv, NI_MAXSERV, NI_NUMERICHOST+NI_NUMERICSERV);
    std::cout << "CONEXIÓN DESDE: "<< host << ", Port: " << serv << std::endl;

    while (true){       
        //RECEPCION DEL MENSAJE
        int len = 80;                                          
        char buffer[len];        //buffer de 80 posiciones de memoria
        int bytes = recv(client_sck, buffer, len-1, 0); //no necesita saber el cliente ya que se establece con el accept()
        buffer[bytes] = '\0';
        if(bytes == 0){
            std::cout << "FIN CONEXIÓN\n";
            return -1;
        }
        //std::cout << "\tData: "<< buffer ;
        //ENVÍO DE RESPUESTA
        send(client_sck, buffer, bytes, 0);           
    }
    close(sck);
    freeaddrinfo(res); //liberar memoria
    return 0;
}