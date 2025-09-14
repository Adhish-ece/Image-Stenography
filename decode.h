#ifndef DECODE_H
#define DECODE_H
#include <stdio.h>
#include "common.h"
#include "types.h"
#include<string.h>
typedef struct _DecodeInfo
{
    char *stego_img_fname;
    FILE *fptr_stego_img;
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    long size_secret_file;
    
}DecodeInfo;

OperationType read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);
Status do_decoding(DecodeInfo *decInfo);
Status open_files_decode(DecodeInfo *decInfo);
Status decode_magic_string(const char *magic_string,DecodeInfo *decInfo);
Status decode_data_from_image(char *data,int size,FILE *fptr_stego_img);
Status decode_byte_from_lsb(char *data,char image_buffer[]);
Status decode_secret_file_extn(DecodeInfo *decInfo);
Status decode_secret_file_size(DecodeInfo *decInfo);
Status decode_secret_file_data(DecodeInfo *encInfo);
#endif