#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

int main(int argc, char *argv[]){
  int origin, destination, hod;
  printf("Bienvenido\n");
  while (1) {
    printf("1. Ingresar origen\n");
    printf("2. Ingresar destino\n");
    printf("3. Ingresar hora\n");
    printf("4. Buscar tiempo de viaje medio\n");
    printf("5. Salir\n");

    int option;
    scanf("%d", &option);

    if (option == 1) {
      printf("Ingrese ID del origen: ");
      scanf("%d", &origin);
    } else if (option == 2) {
      printf("Ingrese ID del destino: ");
      scanf("%d", &destination);
    } else if (option == 3) {
      printf("Ingrese hora del día: ");
      scanf("%d", &hod);
    } else if (option == 4) {
      int socketfd;
      struct sockaddr_in server_direccion;
      struct hostent *server;
      char buffer[256];

      int numero_puerto = atoi(argv[2]);
      
      // Creación del Socket
      socketfd = socket(AF_INET, SOCK_STREAM, 0);

      // Se evalua si el socket se creó con exito 
      if (socketfd == -1){
        printf("Hubo un fallo en la creación del socket");
        exit(1);
      }

      server = gethostbyname(argv[1]);
      if (server == NULL){
        printf("El server que se indicó no es válido");
        exit(1);
      }

      server_direccion.sin_family = AF_INET;

      bcopy((char *) server->h_addr, (char *) &server_direccion.sin_addr.s_addr, server->h_length);

      server_direccion.sin_port = htons(numero_puerto);
      

      if (connect(socketfd, (struct sockaddr *)&server_direccion, sizeof(server_direccion)) < 0){
        printf("No se logró conectar con el servidor");
        exit(1);
      }

      sprintf(buffer, "%d,%d,%d", origin, destination, hod);

      int n = write(socketfd, buffer, strlen(buffer));

      if (n < 0) {
	perror("No se pudo escribir en el socket");
	exit(1);
      } 

      bzero(buffer, 256);
      n = read(socketfd, buffer, 255);
      if (n < 0) {
          perror("No se pudo leer desde el socket");
          exit(1);
      }

      float result;
      sscanf(buffer, "%f", &result);
      
      if (result < 0){
	printf("Tiempo medio no encontrado\n"); 
      }else {
	printf("Resultado de la busqueda: %f\n", result);
      }


      close(socketfd);
    } else if (option == 5) {
      break;
    } else {
      printf("Opción no válida. Intente de nuevo.\n");
    }
  }

  return 0;
}

