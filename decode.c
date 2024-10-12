/*
    Discription: Digital steganography is the practice of concealing information within digital files, such as images
                 audio, or video, to prevent detection. It uses techniques to embed secret data in a way that is
                 imperceptible to the human eye or ear. This method enhances privacy and security by hiding
                 messages in plain sight. Common applications include secure communications and digital watermarking
*/

#include <stdio.h>
#include "decode.h"
#include "types.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // checking arguiment 2 is having . extension or not
    if(strchr(argv[2],'.') == NULL)
    {
        printf("INFO: argv[2] must contain <.bmp> extension\n");
        return e_failure;
    }
    else if(!strcmp(strstr(argv[2],"."),".bmp"))
    {
        decInfo->d_secret_fname = "secret.txt";
        printf("INFO: argv[2] is having .bmp extention\n");

        // storing source file name
        decInfo->d_src_image_fname = argv[2];
        if(argv[3] != NULL)
        {
            if((!strcmp(strstr(argv[3],"."),".txt"))||(!strcmp(strstr(argv[3],"."),".c"))||(!strcmp(strstr(argv[3],"."),".sh")))
            {
                printf("INFO: argv[3] is having '.txt','.c' or '.sh' extension\n");

                // storing secret  file name
                decInfo->d_secret_fname = argv[3];
                return e_success;
            }
            else 
            {
                printf("argv[3] must have '.txt','.c' or '.sh' extension\n");
            }
        }
        else
        {
            // storing  destination file name
            decInfo->decode_image_fname = "decode_secret";
            return e_success;
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

// function call to decoding 
Status do_decoding(DecodeInfo *decInfo)
{
    printf("INFO: Opening required file\n");

    // function call to open required files 
    if(decode_open_files(decInfo) != e_success)
    {
        return e_failure;
    }
    printf("INFO: DONE\n");

    // function call to skip 54 bytes of header from stego image 
    if(skip_bmp_header(decInfo->d_fptr_src_image, decInfo->fptr_decode_image) != e_success)
    {
        return e_failure;
    }
    printf("INFO: Decoding magic string\n");

    // function call to decode magic string 
    if(decode_magic_string(decInfo->magic_string , decInfo) != e_success)
    {
        return e_failure;
    }
    printf("INFO: Decoded successfullt\n");
    
    // function call to decode secret file extension size
    if(decode_secreate_file_extn_size(MAX_FILE_SUFFIX, decInfo) !=e_success)
    {
        return e_failure;
    }

    printf("INFO: Decoding output file extension\n");
    char *extn_size = strstr(decInfo->d_secret_fname,".");

    // function call to decode secret file extension
    if(decode_secret_file_extn(extn_size, decInfo) != e_success)
    {
        return e_failure;
    }
    printf("INFO: Decoded successfullt\n");

    printf("INFO: Decoding output file size\n");

    // function call to decode secret file extension size 
    if(decode_secret_file_size(decInfo) != e_success)
    {
        return e_failure;
    }
    printf("INFO: Decoded successfullt\n");
    
    printf("INFO: Decoding output file data\n");

    // function call to decode secret meassage 
    if(decode_secret_file_data(decInfo) != e_success)
    {
        return e_failure;
    }
    printf("INFO: Decoded successfullt\n");
    return e_success;
}

uint decode_get_file_size(FILE *fptr)
{
    // function call to decode secret file data size 
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    // storing data size of secret data in data_size variable 
    int data_size = decode_get_file_size(decInfo->d_fptr_secret);

    char enc_data[data_size*8];
    char data[data_size];
    
    // reading secret data size from source image 
    fread(enc_data,data_size*8,1,decInfo->d_fptr_src_image);

    int k=0;
    for(int i=0;i<data_size;i++)
    {
        data[i] = 0;
        for(int j=0;j<8;j++)
        {
            // decoding secret data to data buffer
            data[i] = data[i] | ((enc_data[k++] & 0x01)<<j);
        }
    }

    int num1 = ftell(decInfo->d_fptr_src_image);
    
    // writing secret data to destination file 
    fwrite(data,data_size,1,decInfo->fptr_decode_image);


    // closing all files 
    fclose(decInfo->fptr_decode_image);
    fclose(decInfo->d_fptr_src_image);
    fclose(decInfo->d_fptr_secret);
   
    return e_success;
}


Status decode_secret_file_size(DecodeInfo *decInfo)
{

    decInfo->d_size_secret_file = 100;

    long int size = decInfo->d_size_secret_file;

    // setting file pointer
    fseek(decInfo->d_fptr_src_image,32,SEEK_CUR);
    long int num = ftell(decInfo->d_fptr_src_image);

    return e_success;
}

Status decode_secreate_file_extn_size(int extn_size, DecodeInfo *decInfo)
{
    int size = 54 + strlen(decInfo->magic_string)*8 + MAX_FILE_SUFFIX*8;
    // setting file pointer 
    fseek(decInfo->d_fptr_src_image,size,SEEK_SET);

    return e_success;
}

Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo)
{
    char buffer[32];
    char decode_file_extn[5];

    // reading secret file extension from source image 
    fread(buffer,1,32,decInfo->d_fptr_src_image);

    // function call to decode data from lsb
    decode_byte_to_lsb(buffer,decode_file_extn);

    char file_name[100];
    char file_ex[4];
    strcpy(file_name,decInfo->decode_image_fname);
    strcpy(file_ex,decode_file_extn);
    
    // storing destination file for decoding in name array
    char name[strlen(decInfo->decode_image_fname) + 4];
    strcpy(name,strcat(file_name,file_ex));

    int len;
    len = strlen(name);

    // terminating file name by null character
    name[len] = '\0';

    // storing secret file name for decoding
    decInfo->decode_image_fname = name;

    // opening secret file for decoding 
    decInfo->fptr_decode_image = fopen(decInfo->decode_image_fname, "w");

    // Do Error handling
    if (decInfo->fptr_decode_image == NULL)
    {
    	perror("fopen");
        // error message 
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->decode_image_fname);

    	return e_failure;
    }

    return e_success;
}

void decode_byte_to_lsb(char *buffer, char *decode_file_extn)
{
    int k=0;
    for(int i=0;i<4;i++)
    {
        decode_file_extn[i] = 0;
        for(int j=0;j<8;j++)
        {
            // decoding lsb to butter 
            decode_file_extn[i] = decode_file_extn[i] | ((buffer[k++] & 0x01)<<j);
        }
    }
    decode_file_extn[4] = '\0';
}

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{

    printf("INFO: Enter magic string : ");

    // scanning magic string from the user for decoding 
    scanf("%s",decInfo->magic_string);

    char buffer[8];
    char magic_str[strlen(decInfo->magic_string)];

    int k=0;
    for(int i=0;i<strlen(decInfo->magic_string);i++)
    {
        // reading data from the encoded image 
        fread(buffer,8,1,decInfo->d_fptr_src_image);
        magic_str[i] = 0;
        for(int j=0;j<8;j++)
        {
            // decoding lsb from encoded img to the buffer
            magic_str[i] = magic_str[i] | ((buffer[j] & 0x01)<<j);
        }
    }
    magic_str[strlen(decInfo->magic_string)] = '\0';

    // checking magic string matching or not 
    if(strcmp(magic_str,decInfo->magic_string) == 0)
    {
        return e_success;
    }
    // error message 
    printf("ERROR: Invalid magic string\n");
    return e_failure;
}

Status skip_bmp_header(FILE *d_fptr_src_image, FILE *fptr_decode_image)
{
    // skipping first 54 bytes of data from the encoded image 
    fseek(d_fptr_src_image,54,SEEK_SET);

    return e_success;
}

Status decode_open_files(DecodeInfo *decInfo)
{
    // Src Image file
    decInfo->d_fptr_src_image = fopen(decInfo->d_src_image_fname, "r");

    // Do Error handling
    if (decInfo->d_fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->d_src_image_fname);

    	return e_failure;
    }

    // Secret file
    decInfo->d_fptr_secret = fopen(decInfo->d_secret_fname, "r");

    // Do Error handling
    if (decInfo->d_fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->d_secret_fname);

    	return e_failure;
    }


    return e_success;
}
