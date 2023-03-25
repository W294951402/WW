#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "crc32.h"
#include "png.h"
#define PNG_SIG_CAP 8
const char png_sig[PNG_SIG_CAP] = "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A";

const int ERROR_INVALID_PARAMS = 1;
const int ERROR_INVALID_FILE = 2;
const int ERROR_INVALID_CHUNK_DATA = 3;
const int ERROR_NO_UIUC_CHUNK = 4;


/**
 * Opens a PNG file for reading (mode == "r" or mode == "r+") or writing (mode == "w").
 * 
 * (Note: The function follows the same function prototype as `fopen`.)
 * 
 * When the file is opened for reading this function must verify the PNG signature.  When opened for
 * writing, the file should write the PNG signature.
 * 
 * This function must return NULL on any errors; otherwise, return a new PNG struct for use
 * with further fuctions in this library.
 */
PNG * PNG_open(const char *filename, const char *mode) {
  FILE* file = fopen(filename, mode);
  if (file == NULL) {
    return NULL;
  }
  PNG *png = malloc(sizeof(PNG));
  png->file = file;
  if (strcmp(mode, "r") == 0 || strcmp(mode, "r+") == 0) {
     char sig[PNG_SIG_CAP];
    fread(sig, 1, PNG_SIG_CAP, png->file);
    if (strncmp(sig, png_sig, 8) != 0) {
      free(png);
      fclose(file);
      return NULL;
  }
} if (strcmp(mode, "w") == 0) {
   fwrite(png_sig, 1, PNG_SIG_CAP, png->file);
  }
  return png;
}



/**
 * Reads the next PNG chunk from `png`.
 * 
 * If a chunk exists, a the data in the chunk is populated in `chunk` and the
 * number of bytes read (the length of the chunk in the file) is returned.
 * Otherwise, a zero value is returned.
 * 
 * Any memory allocated within `chunk` must be freed in `PNG_free_chunk`.
 * Users of the library must call `PNG_free_chunk` on all returned chunks.
 */
size_t PNG_read(PNG *png, PNG_Chunk *chunk) {
 FILE *file = png->file;
 if (file == NULL) {
  return 0;
 }
  uint32_t length = 0;
  fread(&length, 1, 4, file);
  uint32_t len = ntohl(length);
  char *type = calloc(5, 1);
  fread(type, 1, 4, file);
  strcpy(chunk->type, type);
  free(type);
if(len != 0) {
chunk->data = calloc(1, len);
fread(chunk->data,1,len,png->file);
}
uint32_t crc;
fread(&crc, 4, 1, file);
chunk->crc = ntohl(crc);
chunk->len = len;
return 12 + len;
}


/**
 * Writes a PNG chunk to `png`.
 * 
 * Returns the number of bytes written. 
 */
size_t PNG_write(PNG *png, PNG_Chunk *chunk) {
FILE* file = png->file;
if(file == NULL){
  return 0;
}
uint32_t len = chunk->len;
uint32_t chunkLength = htonl(chunk->len);
fwrite(&chunkLength, 4, 1, png->file);
fwrite(chunk->type, 1, 4, png->file);
if (len != 0) {
  fwrite(chunk->data, 1, chunk->len, png->file);

}
uint32_t crc = 0;
char *str = calloc(1,4 + chunkLength + 1);
strcat(str, chunk->type);
if (len != 0) {
  strcat(str, chunk->data);
}
crc32(str, chunk->len+4, &crc);
free(str);
uint32_t temp = htonl(crc);
chunk->crc = crc;
fwrite(&temp, 4, 1, png->file);
return chunk->len + 12;

}


/**
 * Frees all memory allocated by this library related to `chunk`.
 */
void PNG_free_chunk(PNG_Chunk *chunk) {
  if (chunk->len!= 0) {
    free(chunk->data);
  }
  chunk->crc = 0;
  chunk->len = 0;
  
}


/**
 * Closes the PNG file and frees all memory related to `png`.
 */
void PNG_close(PNG *png) {
  fclose(png->file);
  free(png);

}