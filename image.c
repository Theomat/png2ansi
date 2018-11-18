#include <png.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "image.h"


static void fatal_error (const char * message, ...)
{
    va_list args;
    va_start (args, message);
    vfprintf (stderr, message, args);
    va_end (args);
    exit (EXIT_FAILURE);
}

struct image_data* new_image_data(png_structp png_ptr, png_infop info_ptr){
  struct image_data* data = malloc(sizeof(struct image_data));
  int width = png_get_image_width(png_ptr, info_ptr);
  int height = png_get_image_height(png_ptr, info_ptr);
  png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for (int y = 0; y < height; y++) {
    row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
  }
  png_read_image(png_ptr, row_pointers);
  data -> width = width;
  data -> height = height;
  data -> content = row_pointers;
  return data;
}
void free_image_data(struct image_data* data){
  for(int i = 0; i < data -> height; i++){
    free(data -> content[i]);
  }
  free(data -> content);
  free(data);
}

struct image_data* read_image(char* png_file){
  FILE* file = fopen(png_file, "rb");
  if(! file)
    fatal_error ("Cannot open '%s': %s\n", png_file, strerror (errno));
  
  png_structp png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (! png_ptr) 
    fatal_error ("Cannot create PNG read structure");
  png_infop info_ptr = png_create_info_struct (png_ptr);
  if (! info_ptr)
    fatal_error ("Cannot create PNG info structure");
  if (setjmp(png_jmpbuf(png_ptr))) fatal_error("Error");
  png_init_io(png_ptr, file);
  png_read_info(png_ptr, info_ptr);
  int color_type = png_get_color_type(png_ptr, info_ptr);
  int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  /* Read any color_type into 8bit depth, RGBA format. */
  /* See http://www.libpng.org/pub/png/libpng-manual.txt */
  if (bit_depth == 16)
    png_set_strip_16(png_ptr);
  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png_ptr);
  /* PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth. */
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png_ptr);
  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png_ptr);
  /* These color_type don't have an alpha channel then fill it with 0xff. */
  if (color_type == PNG_COLOR_TYPE_RGB ||
      color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
  if (color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png_ptr);
  png_read_update_info(png_ptr, info_ptr);
  struct image_data* data = new_image_data(png_ptr, info_ptr);
  fclose(file);
  return data;
}
