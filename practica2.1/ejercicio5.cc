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
//                              CLIENTE TCP
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
    char* input = argv[3];
    
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
    //ESTABLECER CONEXIÓN
    struct sockaddr *serv = res->ai_addr;                        //al ser un parametro de salida no hace falta inicializarlo
    socklen_t servLen = sizeof(struct sockaddr);                //al ser un parametro de e/s hay que inicializarlo
    int serv_sck = connect(sck, serv, servLen);
    //CONEXIÓN
    bool quit = false;
    while(!quit){
        //preparacion del mensaje de envío
        char* m;
        std::cin>>m;
        send(sck, m, sizeof(m), 0);
        //recepción de la respuesta echo
        int len = 80;                                          
        char buffer[len];                            //buffer de 80 posiciones de memoria
        int bytes = recv(sck, &buffer, len-1, 0); 
        buffer[bytes] = '\0';
        if(bytes == -1){
            return -1;
        }
        std::cout<<"ECHO: "<< buffer<<std::endl;
        if(buffer[0] == 'Q')
            quit = true;
    }
    
    close(sck);
    freeaddrinfo(res); //liberar memoria
    return 0;
}