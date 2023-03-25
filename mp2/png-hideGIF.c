#include "png-hideGIF.h" 

int png_hideGIF(const char *png_filename_source, const char *gif_filename, const char *png_filename_out) {
PNG *png = PNG_open(png_filename_source,"r");
if (!png){ return ERROR_INVALID_FILE; }
PNG *out = PNG_open(png_filename_out,"w");
size_t bytesWritten;
while(1) {
  PNG_Chunk chunk;
  if (PNG_read(png, &chunk) == 0) {
    PNG_close(png);
    PNG_close(out);
    return ERROR_INVALID_FILE;
  }
  bytesWritten = PNG_write(out, &chunk);
  if (strcmp(chunk.type, "IHDR") == 0) {
    FILE *gif = fopen(gif_filename,"rb");
    PNG_Chunk target_chunk;
    strcpy(target_chunk.type, "uiuc");
    fseek(gif, 0, SEEK_END);
    size_t target_len = ftell(gif);
    rewind(gif);
    target_chunk.len = target_len;
    unsigned char* target_data = malloc(target_len);
    fread(target_data, 1, target_len, gif);
    target_chunk.data = target_data;
    target_chunk.crc = 0;
    PNG_write(out, &target_chunk);
    free(target_data);
    fclose(gif);
  }
  if ( strcmp(chunk.type,"IEND") ==  0) {
     PNG_free_chunk(&chunk);
     break;
   }
   PNG_free_chunk(&chunk);
}
PNG_close(out);
PNG_close(png);
return 0;  // Change the to a zero to indicate success, when your implementaiton is complete.
}
