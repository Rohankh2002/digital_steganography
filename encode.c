/*
    Discription: Digital steganography is the practice of concealing information within digital files, such as images
                 audio, or video, to prevent detection. It uses techniques to embed secret data in a way that is
                 imperceptible to the human eye or ear. This method enhances privacy and security by hiding
                 messages in plain sight. Common applications include secure communications and digital watermarking
*/


#include <stdio.h>
#include "encode.h"
#include "types.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);
    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}


/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    printf("INFO: Opening required files\n");
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }
    printf("INFO: Opened %s\n",encInfo->src_image_fname);

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }
    printf("INFO: Opened %s\n",encInfo->secret_fname);

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }
    printf("INFO: Opened %s\n",encInfo->stego_image_fname);
    printf("INFO: DONE\n");

    // No failure return e_success
    return e_success;
}

OperationType check_operation_type(char *argv[])
{
    // for encoding
    if(!strcmp(argv[1],"-e"))
    return e_encode;
    // for decoding 
    else if(!strcmp(argv[1],"-d"))
    return e_decode;
    // default return value 
    return e_unsupported;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // checking arguiment 2 is having . extension or not 
    if(strchr(argv[2],'.') == NULL)
    {
        printf("INFO: argv[2] must contain <.bmp> extension\n");
        return e_failure;
    }
    else if(!strcmp(strstr(argv[2],"."),".bmp"))
    {
        printf("INFO: argv[2] is having .bmp extention\n");

        // storing source file name 
        encInfo->src_image_fname = argv[2];
        printf("INFO: %s file validation is done\n",encInfo->src_image_fname);

        // checking arguiment 3 is having . extension or not
        if(strchr(argv[3],'.') == NULL)
        {
            printf("INFO: argv[3] must contain <.txt> extension\n");
            return e_failure;
        }
        else if((!strcmp(strstr(argv[3],"."),".txt"))||(!strcmp(strstr(argv[3],"."),".c"))||(!strcmp(strstr(argv[3],"."),".sh")))
        {
            printf("INFO: argv[3] is having '.txt','.c' or '.sh' extension\n");

            //storing secret file name
            encInfo->secret_fname = argv[3];
            printf("INFO: %s file validation is done\n",encInfo->secret_fname);
            if(argv[4] != NULL)
            {
                if(!strcmp(strstr(argv[4],"."),".bmp"))
                {
                    // stroing destination file name 
                    encInfo -> stego_image_fname = argv[4];
                    printf("INFO: output file %s validation is done\n",encInfo->stego_image_fname);
                    return e_success;
                }
                else
                {
                    // error message 
                    printf("argv[4] is not having '.bmp' extension\n");
                    return e_failure;
                }
            }
            else
            {
                // storing dafault destination name for encoding 
                encInfo -> stego_image_fname = "stego_img.bmp";
                printf("INFO: output file %s validation is done\n",encInfo->stego_image_fname);
                return e_success;
            }
        }
        else
        {
            // error message 
            printf("argv[3] is not having '.txt','.c' or '.sh' extension\n");
            printf("Please try with '.txt','.c' or '.sh'\n");
            return e_failure;
        }
    }
    else
    {
        // error message 
        printf("argv[2] is not having .bmp extension\n");
        printf("Please try with argv[2] as .bmp extension\n");
        return e_failure;
    }
}


Status do_encoding(EncodeInfo *encInfo)
{
    // Test open_files
   if (open_files(encInfo) == e_failure)
    {
    	printf("ERROR: %s function failed\n", "open_files" );
    	return 1;
    }
    printf("INFO: ## Encoding Procedure Started ##\n");

    //char MAGIC_STRING[100];
    printf("INFO: Enter magic string : ");
    scanf("%s",encInfo->magic_string);

    // function call to check the capacity of source and destination file 
    if(check_capacity(encInfo) != e_success)
    {
        return e_failure;
    }
    printf("INFO: Done, Founded Greater Image Size\n");

    // function call to copy 54 bytes of data from .bmp file to destiation file 
    printf("INFO: Copying image header\n");
    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
    {
        return e_failure;
    }
    printf("INFO: Copied successfully\n");

    // function call to encode magic string entered by the user    
    if(encode_magic_string( encInfo->magic_string , encInfo) != e_success)
    {
        return e_failure;
    }
    printf("INFO: Encoded magic string\n");
    
    // function call to encode extension of secret file 
    if(encode_secreate_file_extn_size(MAX_FILE_SUFFIX,encInfo) != e_success)
    {
        return e_failure;
    }
    
    const char *file_extn = strstr(encInfo->secret_fname,".");

    //  function call to encode secret file extension
    if(encode_secret_file_extn(file_extn, encInfo) != e_success)
    {
        return e_failure;
    }
    printf("INFO: Encoded %s file extension\n",encInfo->secret_fname);

    // function call to encode size of secret file 
    if(encode_secret_file_size(encInfo) != e_success)
    {
        return e_failure;
    }

    //function call to encode secret file data 
    if(encode_secret_file_data(encInfo) != e_success)
    {
        return e_failure;
    }
    printf("INFO: Encoded %s file data\n",encInfo->secret_fname);

    // function call to copy reamining data of .bmp file
    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
    {
        return e_failure;
    }
    printf("INFO: Encoded successfully\n");
    return e_success;

}

uint get_file_size(FILE *fptr)
{
    //setting poiter to the end to check the size of data 
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}


Status check_capacity(EncodeInfo *encInfo)
{
    //Getting size of bmp image in bytes
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    printf("INFO: Image capacity size is: %d\n",encInfo -> image_capacity);

    //Getting size of file in bytes
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);

    printf("INFO: Encoding  data size is: %ld\n",(54 + (strlen(encInfo->magic_string) + MAX_FILE_SUFFIX + 4 + 4 + encInfo -> size_secret_file)*8));

    //Checking wether the encoding size is lesser than image size is not 
    if((encInfo -> image_capacity) > (54 + (strlen(encInfo->magic_string) + MAX_FILE_SUFFIX + 4 + 4 + encInfo -> size_secret_file)*8))
    {
        return e_success;
    }
    return e_failure;

}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char beffer[54];
    rewind(fptr_src_image);
    rewind(fptr_stego_image);

    // reading 54 bytes of data from src_image;
    fread(beffer,sizeof(char),54,fptr_src_image);

    // writing 54 bytes of header to destination image
    fwrite(beffer,sizeof(char),54,fptr_stego_image);

    return e_success;
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    //encoding lsb bits of bmp image and storing in destination image
    encode_data_to_image(magic_string, strlen(magic_string), encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer[8];
    for(int i=0;i<size;i++)
    {
        fread(buffer,8,1,fptr_src_image);

        //encoding lsb bits
        encode_byte_to_lsb(data[i], buffer);
        
        fwrite(buffer,8,1,fptr_stego_image);
    }
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
        //encoding lsb bits of image_buffer
        image_buffer[i] = image_buffer[i] & 0xfe | ((data&(1<<i))>>i); 
    }
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[32];
    int size= sizeof(buffer);

    // reading 32 bytes of data from src_image;
    fread(buffer,1,32,encInfo->fptr_src_image);

    encode_secret_file_extn_to_lsb(size, buffer,encInfo->secret_fname);

    // writing 32 bytes of header to destination image
    fwrite(buffer,1,32,encInfo->fptr_stego_image);

    return e_success;
}

void encode_secret_file_extn_to_lsb(int size, char *buffer, char *secret_fname)
{

    char *extn = strstr(secret_fname,".");

    for(int i=0;i<size;i++)
    {
        //encoding lsb bits of buffer
        buffer[i] = (buffer[i] & 0xfe) | (((extn[i/8])>>(i%8))&0x01);
    }

}

Status encode_secreate_file_extn_size(int extn_size, EncodeInfo *encInfo)
{
    char buffer[32];

    // reading 32 bytes of data from src_image;
    fread(buffer,sizeof(char),32,encInfo->fptr_src_image);

    //function call for encoding data of secret file
    encode_secret_file_extn_size(extn_size, buffer);

    // writing 32 bytes of header to destination image
    fwrite(buffer,sizeof(char),32,encInfo->fptr_stego_image);

    return e_success; 
}

void encode_secret_file_extn_size(int file_size, char *buffer)
{
    for(int i=0;i<file_size;i++)
    {
        //encoding data of secret file to lsb of buffer
        buffer[i] = buffer[i] & ((0x04)>>i);
    }
}

Status encode_secret_file_size(EncodeInfo *encInfo)
{
    long int size = 32;
    char buffer[size];

    // reading size bytes of data from src_image;
    fread(buffer,sizeof(char),size,encInfo->fptr_src_image);

    for(int i=0;i<size;i++)
    {
        //encoding lsb of buffer
        buffer[i] = (buffer[i] & 0xfe) | ((0x04)>>size);
    }

    // writing size bytes of header to destination image
    fwrite(buffer,sizeof(char),size,encInfo->fptr_stego_image);

    return e_success; 

}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    //size of secret file data in bytes foe encoding
    int size = 8*encInfo->size_secret_file;

    char buffer[size];

    rewind(encInfo->fptr_secret);
    
    //secret data 
    char sec_data[encInfo->size_secret_file];

    //termonating secret data by null character
    sec_data[encInfo->size_secret_file] = '\0';

    //reading data from secret file
    fread(sec_data,1,encInfo->size_secret_file,encInfo->fptr_secret);

    //reading data from .bmp file for encoding 
    fread(buffer,sizeof(char),size,encInfo->fptr_src_image);

    int k=0;

    for(int i=0;i<encInfo->size_secret_file;i++)
    {
        for(int j=0;j<8;j++)
        {
            //encoding lsb of buffer
            buffer[k] = (buffer[k] & 0xfe) | (((sec_data[i]) & (0x01<<j))>>j);
            k++;
        }
    }

    //writing encoded data to destination image 
    fwrite(buffer,sizeof(char),size,encInfo->fptr_stego_image);

    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char buffer[1];
    char ch;

    while((ch = fread(buffer,sizeof(char),1,fptr_src)) >0)
    {
        //writing reamining data to destination image 
        fwrite(buffer,sizeof(char),1,fptr_dest);

    }

    return e_success;
}
