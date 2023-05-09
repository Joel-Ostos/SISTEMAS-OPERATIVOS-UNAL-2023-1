#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

#define MAX_ID 10000000
#define MAX_HOD 24
#define INPUT_PIPE_NAME "input_pipe"
#define OUTPUT_PIPE_NAME "output_pipe"

typedef struct {
  int sourceid;
  int dstid;
  int hod;
  float mean_travel_time;
} TravelData;

void search_travel_time(FILE *file, int origin, int destination, int hod,
    float *result) {
  *result = -1;
  TravelData data;
  rewind(file);
  while (fread(&data, sizeof(TravelData), 1, file) == 1) {
    if (data.sourceid == origin && data.dstid == destination &&
	data.hod == hod) {
      *result = data.mean_travel_time;
      break;
    }
  }
}

void child_process_code() {
  int input_pipe = open(INPUT_PIPE_NAME, O_RDONLY);
  int output_pipe = open(OUTPUT_PIPE_NAME, O_WRONLY);

  FILE *file = fopen("prueba.bin", "rb");
  int origin, destination, hod;

  while (1) {
    read(input_pipe, &origin, sizeof(int));
    read(input_pipe, &destination, sizeof(int));
    read(input_pipe, &hod, sizeof(int));

    float result;
    search_travel_time(file, origin, destination, hod, &result);

    write(output_pipe, &result, sizeof(float));
  }

  close(input_pipe);
  close(output_pipe);
}

void parent_process_code() {
  int input_pipe = open(INPUT_PIPE_NAME, O_WRONLY);
  int output_pipe = open(OUTPUT_PIPE_NAME, O_RDONLY);

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
      write(input_pipe, &origin, sizeof(int));
      write(input_pipe, &destination, sizeof(int));
      write(input_pipe, &hod, sizeof(int));

      float result;
      read(output_pipe, &result, sizeof(float));

      if (result >= 0) {
	printf("Tiempo de viaje medio: %.2f\n", result);
      } else {
	printf("No encontrado");
      }
    } else if (option == 5) {
      break;
    } else {
      printf("Opción no válida, por favor intente de nuevo.\n");
    }
  }

  close(input_pipe);
  close(output_pipe);
}

int main(int argc, char *argv[]) {
  mkfifo(INPUT_PIPE_NAME, 0666);
  mkfifo(OUTPUT_PIPE_NAME, 0666);

  pid_t pid = fork();

  if (pid < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    // Proceso hijo
    setsid(); // Desvincular el proceso hijo del proceso padre
    child_process_code();
  } else {
    // Proceso padre
    sleep(1); // Dar tiempo al proceso hijo para iniciar
    parent_process_code();
    // Limpieza al salir
    unlink(INPUT_PIPE_NAME);
    unlink(OUTPUT_PIPE_NAME);
  }

  return 0;
}

