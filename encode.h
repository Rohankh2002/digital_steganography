#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types
#include <string.h>

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname;  //store original image file name
    FILE *fptr_src_image;   //file pointer to acccess original file
    uint image_capacity;    //size of the original file
    char magic_string[100]; //magic string of max length 100 bytes

    /* Secret File Info */
    char *secret_fname;   //store secrete file name
    FILE *fptr_secret;     // file pointer to access secrete file
    char extn_secret_file[MAX_FILE_SUFFIX];  //secreate file extn
    char secret_data[MAX_SECRET_BUF_SIZE];    //secteate info, which is present in secrete file
    long size_secret_file; //size of the secrete file

    /* Stego Image Info */
    char *stego_image_fname; //o/p file name
    FILE *fptr_stego_image; // file pointer to access o/p file

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image);

/* Store Magic String */
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo);

/* Encode secreate file extn size*/
Status encode_secreate_file_extn_size(int extn_size, EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

/*Encoding secret file extension to lsb*/
void encode_secret_file_extn_to_lsb(int size, char *buffer, char *secret_fname);

/*Encoding secret file extn size*/
void encode_secret_file_extn_size(int file_size, char *buffer);
#endif
