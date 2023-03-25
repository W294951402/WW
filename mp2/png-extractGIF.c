#include "png-extractGIF.h"




int png_extractGIF(const char *png_filename, const char *gif_filename) {
  //Check png_file
FILE *png = PNG_open(png_filename, "r");
if (!png){
  return ERROR_INVALID_FILE;
}
while(1) {
  PNG_Chunk chunk;
  if (PNG_read(png, &chunk) == 0) {
    PNG_close(png);
    return ERROR_INVALID_CHUNK_DATA;
  }
  if (strcmp(chunk.type, "uiuc") == 0) {
    FILE* fp = fopen(gif_filename, "w");
    fwrite(chunk.data, 1, chunk.len, fp);
    fclose(fp);
    PNG_close(png);
    PNG_free_chunk(&chunk);
    return 0;
  }
   if ( strcmp(chunk.type,"IEND") ==  0) {
     PNG_free_chunk(&chunk);
     break;
   }
  PNG_free_chunk(&chunk);
}
PNG_close(png);
return 255;
}




