#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
#include<string.h>

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* decode Source Image info */
    char *d_src_image_fname;  //store original image file name
    FILE *d_fptr_src_image;   //file pointer to acccess original file
    uint d_image_capacity;    //size of the original file

    /* decode Secret File Info */
    char *d_secret_fname;   //store secrete file name
    FILE *d_fptr_secret;     // file pointer to access secrete file
    char d_extn_secret_file[MAX_FILE_SUFFIX];  //secreate file extn
    char d_secret_data[MAX_SECRET_BUF_SIZE];    //secteate info, which is present in secrete file
    long d_size_secret_file; //size of the secrete file
    char magic_string[100]; //magic string of max length 100 bytes


    /* decode Image Info */
    char *decode_image_fname; //o/p file name
    FILE *fptr_decode_image; // file pointer to access o/p file

} DecodeInfo;

/* decoding function prototype */



/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status decode_open_files(DecodeInfo *decInfo);

/* Get file size */
uint decode_get_file_size(FILE *d_fptr_secret);

/* Copy bmp image header */
Status skip_bmp_header(FILE *d_fptr_src_image, FILE *fptr_decode_image);

/* Store Magic String */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

/* decode secreate file extn size*/
Status decode_secreate_file_extn_size(int extn_size, DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo);

/* Decode bytes to lsb */
void decode_byte_to_lsb(char *buffer, char *decode_file_extn);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif
