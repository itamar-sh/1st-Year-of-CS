#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int main(int argc, char *argv[]) {
    int gap = 26;
    if(argc==1){
        fprintf(stderr, "The given command is invalid\n");
        return EXIT_FAILURE;
    }
    if(strcmp(argv[1], "encode") != 0 && strcmp(argv[1], "check") != 0 && strcmp(argv[1], "decode") != 0){
        fprintf(stderr, "The given command is invalid\n");
        return EXIT_FAILURE;
    }
    if(strcmp(argv[1], "encode") == 0 || strcmp(argv[1], "decode") == 0){
        if(argc!=5) {
            fprintf(stderr, "Usage: cipher <encode|decode> <k> <source path file> <output path file>\n");
            return EXIT_FAILURE;
        }
        FILE *fsource = fopen(argv[3], "r");
        FILE *foutput = fopen(argv[4], "w");
        if(fsource==NULL){
            fprintf(stderr, "The given file is invalid\n");
            return EXIT_FAILURE;
        }
        int k = atoi(argv[2]);
        k = k%gap;
        if(strcmp(argv[1], "decode") == 0){
            k = -k;
        }
        int char_in, char_out;
        while((char_in = fgetc(fsource))!=EOF){
            if(char_in >= 'A' && char_in <= 'Z'){
                char_out = char_in + k;
                if(char_out<'A'){
                    char_out = char_out + gap;
                }
                else if(char_out>'Z'){
                    char_out = char_out - gap;
                }
                fputc(char_out, foutput);
            }
            else if(char_in >= 'a' && char_in <= 'z'){
                char_out = char_in + k;
                if(char_out<'a'){
                    char_out = char_out + gap;
                }
                else if(char_out>'z'){
                    char_out = char_out - gap;
                }
                fputc(char_out, foutput);
            }
            else{
                fputc(char_in, foutput);
            }
        }

    }
    if(strcmp(argv[1], "check") == 0){
        if(argc!=4){
            fprintf(stderr, "Usage: cipher <check> <source path file> <output path file>\n");
            return EXIT_FAILURE;
        }
        FILE *fsource = fopen(argv[2], "r");
        FILE *foutput = fopen(argv[3], "r");
        if(fsource==NULL || foutput==NULL){
            fprintf(stderr, "The given file is invalid\n");
            return EXIT_FAILURE;
        }
        int char_in, char_out;
        int k=-1;
        while((char_in = fgetc(fsource))!=EOF && (char_out=fgetc(foutput))!=EOF){
            if(char_in >= 'A' && char_in <= 'Z'){
                if(char_out >= 'A' && char_out <= 'Z'){
                    if(k==-1){
                        if((char_out-char_in)<0){
                            k = char_out-char_in+gap;
                        }
                        else{
                            k = char_out-char_in;
                        }
                    }
                    else if((char_out-char_in)!=k && (char_out-char_in+gap)!=k){
                        fprintf(stdout, "Invalid encrypting\n");
                        return EXIT_SUCCESS;
                    }
                }
            }
            else if(char_in>='a' && char_in<= 'z'){
                if(char_out>='a' && char_out<= 'z'){
                    if(k==-1){
                        if((char_out-char_in)<0){
                            k = char_out-char_in+gap;
                        }
                        else{
                            k = char_out-char_in;
                        }
                    }
                    else if((char_out-char_in)!=k && (char_out-char_in+gap)!=k){
                        fprintf(stdout, "Invalid encrypting\n");
                        return EXIT_SUCCESS;
                    }
                }
            }
            else if(char_in != char_out){
                fprintf(stdout, "Invalid encrypting\n");
                return EXIT_SUCCESS;
            }
        }
        if(k!=-1){
            fprintf(stdout, "Valid encrypting with k = %d\n", k);
            return EXIT_SUCCESS;
        }
        else{
            fprintf(stdout, "Valid encrypting with k = 0\n");
            return EXIT_SUCCESS;
        }
    }
    return 0;
}
