/*
    Discription: Digital steganography is the practice of concealing information within digital files, such as images
                 audio, or video, to prevent detection. It uses techniques to embed secret data in a way that is
                 imperceptible to the human eye or ear. This method enhances privacy and security by hiding
                 messages in plain sight. Common applications include secure communications and digital watermarking
*/

#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo; // structure variable for encoding 
    DecodeInfo decInfo; // structure variable  for decoding 
    uint img_size; // structure variable for return size 


    ////////////////////////////////////////////////////////////////////////////
        // checking 1 arguiment is valid or not 
        if(argc == 1 && (strcmp(argv[0],"./a.out") == 0))
        {
            // error message 
            printf("./lsb_stego: Encoding : ./lsb_stego -e <.bmp_file> <.txt_file> [output file]\n");
            printf("./lsb_stego: Decoding: ./lsb_stego -d <.bmp_file> [output file]\n");
            return 0;
        }
        else if(argc == 2 && (strcmp(argv[1],"-e")==0))
        {
            // error message
            printf("INFO: Enter valid number of arguments\n");
            printf("./lsb_stego: Encoding : ./lsb_stego -e <.bmp_file> <.txt_file> [output file]\n");
            return 0;
        }
        else if(argc == 2 && (strcmp(argv[1],"-d") == 0))
        {
            // error message
            printf("INFO: Enter valid number of arguments\n");
            printf("./lsb_stego: Decoding: ./lsb_stego -d <.bmp_file> [output file]\n");
            return 0;
        }
        else if(strchr(argv[1],'e') != NULL)
        {
            if(argc < 4)
            {
                // error message
                printf("Error : Insufficient arguments\n");
                printf("./lsb_stego: Encoding : ./lsb_stego -e <.bmp_file> <.txt_file> [output file]\n");
                return e_failure;
            }
            else
            {
                // function call to check arguments for encoding 
                if(check_operation_type(argv) == e_encode)
                {
                    printf("---------------------\n");
                    printf("User option is encode\n");
                    printf("---------------------\n");
                    EncodeInfo encInfo;
                    if( read_and_validate_encode_args(argv, &encInfo) == e_success) // function call to validate arguments for encoding
                    {
                        printf("INFO: read and validation for encoding is success\n");
                        if( do_encoding(&encInfo) == e_success)
                        {
                            printf("---------------------\n");
                            printf("encoding is completed\n");
                            printf("---------------------\n");
                        }
                    }
                }
            }
        }
        else if(strchr(argv[1],'d') != NULL)
        {
            if(argc < 3)
            {
                // error message
                printf("Error : Insufficient arguments\n");
                printf("./lsb_stego: Decoding: ./lsb_stego -d <.bmp_file> [output file]\n");
                return e_failure;
            }
            else if(check_operation_type(argv) == e_decode) // function call to check arguments for decoding
            {
                    printf("---------------------\n");
                    printf("User option is decode\n");
                    printf("---------------------\n");
                    DecodeInfo decInfo;
                    if(read_and_validate_decode_args(argv, &decInfo) == e_success) // function call to validate arguments for decoding
                    {
                        printf("INFO: read and validation for decoding is success\n"); 
                        if(do_decoding(&decInfo) == e_success)
                        {
                            printf("---------------------\n");
                            printf("decoding is complete\n");
                            printf("---------------------\n");
                        }
                    }
            }
        }
        else
        {
            // error message
            printf("INFO: Enter valid arguiments for encoding and decoding\n");
        }

    return 0;

}
