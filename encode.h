#ifndef ENCODE_H
#define ENCODE_H

#include "types.h"
#include<stdio.h>
#include "common.h"
#include<string.h>



typedef struct _EncodeInfo
{
    char *scr_img_fname;
    FILE *fptr_scr_img;
    uint img_cap;
    uint bits_per_pixel;
    char img_data[MAX_IMG_BUF_SIZE];

    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;
    char *stego_img_fname;
    FILE *fptr_stego_img;
}EncodeInfo;

OperationType check_operation_type(char **);
OperationType read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo);
Status do_encoding(EncodeInfo *encInfo);
Status open_files_encode(EncodeInfo *encInfo);
Status check_capacity(EncodeInfo *encInfo);
uint get_image_size_for_bmp(FILE *fptr_img);
uint get_file_size(FILE *fptr);
Status copy_bmp_header(FILE *fptr_scr,FILE *fptr_dest);
Status encode_magic_string(char magic_string[],EncodeInfo *encInfo);
Status encode_data_to_image(const char data[],int size,EncodeInfo *encInfo);
Status encode_byte_to_lsb(char data,char *img_buf);
Status encode_secret_file_extn(const char *file_extn,int ext_len,EncodeInfo *encInfo);
Status encode_secret_file_size(long file_size,EncodeInfo *encInfo);
Status encode_secret_file_data(EncodeInfo *encInfo);
Status copy_remaining_img_data(FILE *fptr_scr,FILE *fptr_dest);
void print_help();

#endif