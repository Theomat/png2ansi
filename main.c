#include <png.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "image.h"

#define USAGE "Usage: png2ansi [-a <alpha_threshold>] <png_file>\nWhere:\n\tpng_file: is the PNG file to be converted to ANSI\n\talpha_threshold: is the minimum alpha value in the range 0-255 that a pixel must have to be colored in the output, it defaults to 127\n"

static int alpha_threshold = 127;

static int parse_opts(int argc, char* argv[]){
  int index = 1;
  int opt;
  while((opt = getopt(argc, argv, "a:")) != -1){
    switch(opt) {
    case 'a':
      alpha_threshold = atoi(optarg);
      if(alpha_threshold < 0 || alpha_threshold > 255)
	fprintf(stderr, "png2ansi: Warning: alpha_threshold is outside of expected range (0-255)\n");
      index += 2;
      break;
    default:
      fprintf(stderr, USAGE);
      exit(EXIT_FAILURE);
    }
  }
  return index;
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
    fprintf(stderr, "png2ansi: Aborted: no file was specified\n");
    fprintf(stderr, USAGE);
    return EXIT_FAILURE;
  }
  char* source = argv[index];
  struct image_data* data = read_image(source);
  process(data);
  free_image_data(data);
  return EXIT_SUCCESS;
}
