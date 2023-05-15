// Ficheros de Cabecera
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

typedef struct {
  int sourceid;
  int dstid;
  int hod;
  float mean_travel_time;
} TravelData;

void busqueda(int socket) {

FILE *file = fopen("prueba.bin", "rb");
char buffer[256];

TravelData data;
/* bzero(buffer, 256); */
int n = read(socket, buffer, 256);
int hod,dstid,sourceid;
float mean_travel_time;

sscanf(buffer, "%d,%d,%d", &sourceid,&dstid,&hod);

while (fread(&data, sizeof(data), 1, file)){
  if(data.hod == hod && data.dstid == dstid && data.sourceid == sourceid ){
    mean_travel_time = data.mean_travel_time;
    break;
  }else{
    mean_travel_time = -1;
  }
}
fclose(file);

    // Envía el resultado al cliente.
    bzero(buffer, 256);
    sprintf(buffer, "%f", mean_travel_time);
    n = write(socket, buffer, strlen(buffer));
    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
}


int main(int argc, char *argv[]){
  int socketfd, socket_cliente, pid;
  struct sockaddr_in server_direccion, cliente_direccion;

  int numero_puerto = atoi(argv[1]);
  // argc nos indica la cantidad de parametros que se pasan por terminal, si la cantidad es menor a 2 significa que el puerto no sé especificó
  if (argc == 0){
    printf("No se especificó el puerto");
    exit(1);
  }

  // Creación del Socket
  socketfd = socket(AF_INET, SOCK_STREAM, 0);

  // Se evalua si el socket se creó con exito xD 
  if (socketfd == -1){
    printf("Hubo un fallo en la creación del socket");
  }
  
  // Aquí se modifican los "atributos" de la instancia server_dirección, vease sockaddr para mayor información XD
  server_direccion.sin_port = htons(numero_puerto);
  server_direccion.sin_family = AF_INET;
  server_direccion.sin_addr.s_addr = INADDR_ANY;
  
  // Se asigna al socket socketfd la direccion server_dirección, para más tarde hacer la conexión con el cliente
  if (bind(socketfd, (struct sockaddr *) &server_direccion, sizeof(server_direccion)) == -1){
    printf("Hubo un error en la asignacion de dirección al socket");
    exit(-1);
  }

  listen(socketfd, 5);
  socklen_t tamano_cliente = sizeof(cliente_direccion);
  
  while (1) {
    socket_cliente = accept(socketfd, (struct sockaddr *) &cliente_direccion, &tamano_cliente);

    if (socket_cliente == -1) {
      perror("Error al aceptar al cliente");
      exit(1);
    }
    pid = fork();
    if (pid == -1) {
      perror("No se logro hacer fork");
      exit(1);
    }
    if (pid == 0) {
      close(socketfd);
      busqueda(socket_cliente); // Esto debería ser en el proceso hijo.
      exit(0);
    } else {
      close(socket_cliente); // Esto debería ser en el proceso padre.
    }

  } 
  close(socketfd);
  return 0;
}

