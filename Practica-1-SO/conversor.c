#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int sourceid;
  int dstid;
  int hod;
  float mean_travel_time;
} TravelData;

int main() {
  FILE *csv_file = fopen("prueba.csv", "r");
  FILE *bin_file = fopen("prueba.bin", "wb");
  char line[256];

  // Saltar la línea de encabezado si es necesario
  fgets(line, sizeof(line), csv_file);

  TravelData data;

  while (fgets(line, sizeof(line), csv_file) != NULL) {
    sscanf(line, "%d,%d,%d,%f", &data.sourceid, &data.dstid, &data.hod,
           &data.mean_travel_time);
    fwrite(&data, sizeof(TravelData), 1, bin_file);
  }

  fclose(csv_file);
  fclose(bin_file);

  return 0;
}

