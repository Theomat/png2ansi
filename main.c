#include <png.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "image.h"



static int alpha_threshold = 127;

static int parse_opts(int argc, char* argv[]){
  return 1;
}

static void process(struct image_data* data){
  png_bytep row;
  png_bytep px;
  unsigned char old[4];
  old[3] = 0;
  for(int j = 0; j < data -> height; j++){
    row = data -> content[j];
    for(int i = 0; i < data -> width; i++){
      px = &(row[i * 4]);
      
      int diff = 0;
      for(int k = 0; k < 3 && !diff; k++)
	diff += px[k] - old[k];
      
      if ((px[3] > alpha_threshold) == old[3] && !diff){
	printf("  ");
      } else {
	if(px[3] > alpha_threshold)
	  printf("\e[48;2;%d;%d;%dm  ", px[0], px[1], px[2]);
	else 
	  printf("\e[0m  ");
      }
      

      for(int k = 0; k < 3; k++)
	  old[k] = px[k];
      old[3] = px[3] > alpha_threshold;
    }
    printf("\e[0m\n");
    old[3] = 0;
  }
  printf("\e[0m");
}

int main(int argc, char* argv[]){
  int index = parse_opts(argc, argv);
  if(index == argc){
    printf("png2ansi: No file specified");
    return EXIT_SUCCESS;
  }
  char* source = argv[index];
  struct image_data* data = read_image(source);
  process(data);
  free_image_data(data);
  return EXIT_SUCCESS;
}
