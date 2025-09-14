#include "encode.h"


OperationType check_operation_type(char *argv[])
{
    if(argv[1]!=NULL)
    {
        if(strcmp(argv[1],"-e")==0)
        return e_encode;
        else if(strcmp(argv[1],"-d")==0)
        return e_decode;
        else if(strcmp(argv[1],"-h")==0)
        return e_help;
    }
    return e_unsupported;
}
OperationType read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
    if(argv[2]!=NULL && strstr(argv[2],".bmp") !=NULL)
    {
        encInfo->scr_img_fname = argv[2];
    }
    else
    return e_failure;
    if(argv[3] !=NULL)
    {
        encInfo->secret_fname=argv[3];
        const char *ext = strchr(encInfo->secret_fname,'.');
        if(ext != NULL)
        {
            int ext_len = strlen(ext);
            if(ext_len >= MAX_FILE_SUFFIX)
            {
                ext_len = MAX_FILE_SUFFIX-1;
            }
            strncpy(encInfo->extn_secret_file,ext,ext_len);
            encInfo->extn_secret_file[ext_len]='\0';
        }
        else
        return e_failure;
    }
    else
    return e_failure;

    if(argv[4] !=NULL)
    {
        encInfo->stego_img_fname = argv[4];
    }
    else
    encInfo->stego_img_fname = "stego_img.bmp";
    return e_success;

}
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files_encode(encInfo)==e_success)
    {
        printf("File opened successfully :)\n");
        if(check_capacity(encInfo)==e_success)
        {
            printf("Capacity check successful\n");
            if(copy_bmp_header(encInfo->fptr_scr_img,encInfo->fptr_stego_img)==e_success)
            {
                printf("Copied BMP header successfully :)\n");
                if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
                {
                    printf("Encoded magic string successfully :)\n");
                    int ext_len = strlen(encInfo->extn_secret_file);
                    if(encode_data_to_image((char *)&ext_len,sizeof(int),encInfo)==e_success)//if ext_len=4; the arrya value = 04 00 00 00(little edian)
                    {
                        if(encode_secret_file_extn(encInfo->extn_secret_file,ext_len,encInfo)==e_success)
                        {
                            //printf("%s",encInfo->extn_secret_file);
                            printf("Encoded secret file extension succefully :)\n");
                            if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
                            {
                                printf("Secrete file size encoded successfullu :)\n");
                                if(encode_secret_file_data(encInfo)==e_success)
                                {
                                    printf("Encoded secrete file data successfully :)\n");
                                    if(copy_remaining_img_data(encInfo->fptr_scr_img,encInfo->fptr_stego_img)==e_success)
                                    {
                                        printf("Copied remaining image data successfully\n");
                                    }
                                }
                            }
                        }
                    }
                }
                
            }
        }
    }
    
}

Status open_files_encode(EncodeInfo *encInfo)
{
    encInfo->fptr_scr_img = fopen(encInfo->scr_img_fname,"r");
    if(encInfo->fptr_scr_img==NULL)
    {
        printf("Error:can't open the file : %s",encInfo->scr_img_fname);
        return e_failure;
    }
    encInfo->fptr_secret = fopen(encInfo->secret_fname,"r");
    if(encInfo->fptr_secret==NULL)
    {
        printf("Error:Can't open the file named: %s",encInfo->scr_img_fname);
        return e_failure;
    }
    encInfo->fptr_stego_img =  fopen(encInfo->stego_img_fname,"w");
    if(encInfo->fptr_stego_img==NULL)
    {
        printf("Error Can't create the file called : %s",encInfo->stego_img_fname);
        return e_failure;
    }
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->img_cap = get_image_size_for_bmp(encInfo->fptr_scr_img);
    encInfo->size_secret_file =get_file_size(encInfo->fptr_secret);
    long required_bytes_for_encoding = (strlen(MAGIC_STRING)+sizeof(int)+strlen(encInfo->extn_secret_file)+sizeof(long)+encInfo->size_secret_file)*8;
    //printf("%ld,%d",required_bytes_for_encoding,encInfo->img_cap);
    if(encInfo->img_cap>=required_bytes_for_encoding)
    return e_success;
    else
    return e_failure;
}

uint get_image_size_for_bmp(FILE *fptr_img)
{
    uint width,height;
    fseek(fptr_img,18,SEEK_SET);
    fread(&width,sizeof(int),1,fptr_img);
    fread(&height,sizeof(int),1,fptr_img);
    return width * height *3;
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    uint size = ftell(fptr);
    rewind(fptr);
    return size;
}

Status copy_bmp_header(FILE *fptr_scr,FILE *fptr_dest)
{
    char buffer[54];
    rewind(fptr_scr);
    fread(buffer,1,54,fptr_scr);
    fwrite(buffer,1,54,fptr_dest);
    return e_success;
}

Status encode_magic_string(char magic_string[],EncodeInfo *encInfo)
{
    return encode_data_to_image(magic_string,strlen(magic_string),encInfo);
}

Status encode_data_to_image(const char data[],int size,EncodeInfo *encInfo)
{
    for(int i =0;i<size;i++)
    {
        fread(encInfo->img_data,1,8,encInfo->fptr_scr_img);
        encode_byte_to_lsb(data[i],encInfo->img_data);
        fwrite(encInfo->img_data,1,8,encInfo->fptr_stego_img);
    }
    return e_success;
}

Status encode_byte_to_lsb(char data,char *img_buf)
{
    for(int i = 0;i<8;i++)
    {
        char bit = (data>>(7-i)) &1;
        img_buf[i]=(img_buf[i]&0xFE)|bit;
    }
    return e_success;
}
Status encode_secret_file_extn(const char *file_extn,int ext_len,EncodeInfo *encInfo)
{
    return encode_data_to_image(file_extn,ext_len,encInfo);
}
Status encode_secret_file_size(long file_size,EncodeInfo *encInfo)
{
    char str_size[sizeof(long)];
    memcpy(str_size,&file_size,sizeof(long));//str_size = { D2, 04, 00, 00, 00, 00, 00, 00 } if file size =0x00000000000004D2
    return encode_data_to_image(str_size,sizeof(long),encInfo);
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buffer[MAX_SECRET_BUF_SIZE];
    rewind(encInfo->fptr_secret);
    for(int i = 0;i<encInfo->size_secret_file;i++)
    {
        fread(buffer,1,MAX_SECRET_BUF_SIZE,encInfo->fptr_secret);
        encode_data_to_image(buffer,MAX_SECRET_BUF_SIZE,encInfo);
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_scr,FILE *fptr_dest)
{
    char ch;
    while(fread(&ch,1,1,fptr_scr))
    {
        fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;
}

void print_help()
{
    printf("Usage:\n");
    printf("  For encoding: ./a.out -e <source_image.bmp> <secret_file> [stego_image.bmp]\n");
    printf("  For decoding: ./a.out -d <stego_image.bmp> [output_file_name]\n");
    printf("\nExamples:\n");
    printf("  ./a.out -e input.bmp secret.txt output.bmp\n");
    printf("  ./a.out -d output.bmp secret_out\n");
}