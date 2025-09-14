#include "decode.h"

OperationType read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
    if(argv[2]!=NULL && strstr(argv[2],".bmp")!=NULL)
    decInfo->stego_img_fname = argv[2];

    else 
    return e_failure;

    if(argv[3]!=NULL)
    decInfo->secret_fname =argv[3];
    
    else
    decInfo->secret_fname ="decode";

    return e_success;
}
Status do_decoding(DecodeInfo *decInfo)
{
    if(open_files_decode(decInfo)==e_success)
    {
        printf("File opened successfully\n");
        if(decode_magic_string(MAGIC_STRING,decInfo)==e_success)
        {
            printf("Decoded magic string successfully\n");
            if(decode_secret_file_extn(decInfo)==e_success)
            {
                printf("Decoded secret file extension successfully\n");
                char secret_filename[256];
                sprintf(secret_filename,"%s%s",decInfo->secret_fname,decInfo->extn_secret_file);
                //printf("%s",secret_filename);
                decInfo->fptr_secret = fopen(secret_filename,"w");
                if(decInfo->fptr_secret==NULL)
                {
                    printf("Error Can't open the file: %s\n",secret_filename);
                    return e_failure;
                }
                if(decode_secret_file_size(decInfo)==e_success)
                {
                    printf("Decoded secrete file size successfully\n");
                    if(decode_secret_file_data(decInfo)==e_success)
                    {
                        printf("Decoded secret file data successfully\n");
                        return e_success;
                    }
                }
            }

        }
    }
}

Status open_files_decode(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_img = fopen(decInfo->stego_img_fname,"r");
    if(decInfo->fptr_stego_img == NULL)
    {
        printf("Error: we can't find the file name %s",decInfo->stego_img_fname);
        return e_failure;
    }
    fseek(decInfo->fptr_stego_img,54,SEEK_SET);
    return e_success;

}

Status decode_magic_string(const char *magic_string,DecodeInfo *decInfo)
{
    char decoded_magic_string[strlen(magic_string)+1];// the one is for adding '\0' at the end
    decode_data_from_image(decoded_magic_string,strlen(magic_string),decInfo->fptr_stego_img);
    decoded_magic_string[strlen(magic_string)]='\0';
    if(strcmp(magic_string,decoded_magic_string)==0)
    return e_success;
    else
    return e_failure;
}

Status decode_data_from_image(char *data,int size,FILE *fptr_stego_img)
{
    char image_buffer[8];
    for(int i=0;i<size;i++)
    {
        fread(image_buffer,1,8,fptr_stego_img);
        decode_byte_from_lsb(&data[i],image_buffer);
    }
    return e_success;
}

Status decode_byte_from_lsb(char *data,char *image_buffer)
{
    *data=0;
    for(int i=0;i<8;i++)
    {
        char bit = (image_buffer[i] & 1);
        *data = (*data << 1) | bit;
    }
    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    int extn_size = 0;
    decode_data_from_image((char *)&extn_size,sizeof(int),decInfo->fptr_stego_img);
    if(extn_size>0 && extn_size < MAX_FILE_SUFFIX)
    {
        decode_data_from_image(decInfo->extn_secret_file,extn_size,decInfo->fptr_stego_img);
        decInfo->extn_secret_file[extn_size]='\0';
        //printf("%d %s ",extn_size,decInfo->extn_secret_file);
        return e_success;
    }
    return e_failure;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char size_buffer[sizeof(long)];
    decode_data_from_image(size_buffer,sizeof(long),decInfo->fptr_stego_img);
    memcpy(&decInfo->size_secret_file,size_buffer,sizeof(long));
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[MAX_SECRET_BUF_SIZE];
    for(int i=0;i<decInfo->size_secret_file;i++)
    {
        decode_data_from_image(buffer,MAX_SECRET_BUF_SIZE,decInfo->fptr_stego_img);
        fwrite(buffer,1,MAX_SECRET_BUF_SIZE,decInfo->fptr_secret);
    }
    return e_success;
}