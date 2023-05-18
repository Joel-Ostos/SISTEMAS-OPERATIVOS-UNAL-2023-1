#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char *argv[]){
  FILE *archivo;
  char buffer[256];

  archivo = fopen(argv[1], "r");
  
  while(fgets(buffer, 256, archivo)){
    puts(buffer);
  }
  fclose(archivo);
}
