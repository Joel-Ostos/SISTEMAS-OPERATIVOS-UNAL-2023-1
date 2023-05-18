#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
int main(int argc, char *argv[]){
  int archivo, tamano_archivo;
  char *buffer;
  archivo = open(argv[1], O_RDWR);
  tamano_archivo = lseek(archivo,0,SEEK_END);
  
  buffer = mmap(NULL, 1, PROT_READ, MAP_SHARED, archivo, (off_t)0);
 
  for (int i = 0; i < tamano_archivo; i++){
    printf("%c",buffer[i]);
  }
  return 0;
}
