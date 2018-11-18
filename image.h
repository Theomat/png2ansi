struct image_data {
  int width;
  int height;
  png_bytep* content;
};
void free_image_data(struct image_data*);
struct image_data* read_image(char*);
