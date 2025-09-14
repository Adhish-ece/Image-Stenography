#include "encode.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    OperationType op_type = check_operation_type(argv);
    if(op_type == e_encode)
    {
        EncodeInfo encInfo;
        printf("Encoding is selected :)\n");
        if(read_and_validate_encode_args(argv,&encInfo)==e_success)
        {
            printf("Reading and validating arguments successful\n");
            {
                if(do_encoding(&encInfo)==e_success)
                {
                    printf("Encoding successful\n");
                    printf("\n well done my boi :) !!\n");
                }
            }
        }
        else
        printf("Error: Encoding failed\n");
    }


    else if(op_type==e_decode)
    {
        DecodeInfo decInfo;
        printf("Decoding option is selected\n");
        if(read_and_validate_decode_args(argv,&decInfo) == e_success)
        {
            printf("Reading and validating decode argument successful\n");
            if(do_decoding(&decInfo)==e_success)
            {
                printf("Decoding successful\n");
            }
        }
        else
        printf("Sorry bro you file can't be decoded\n");
    }
    else if(op_type==e_help)
    print_help();
    else
    printf("type -h to view help menu");
}
