#include <argp.h>
#include"terrain.h"

const char* argp_program_bug_address = "rascheel@iastate.edu";

typedef struct 
{
    int arg_count;
    char outputInWav;
    char* outputFileName;
    char** inputFileNames;
}arguments_t;

static int parse_opt(int key, char* arg, struct argp_state* state)
{
    arguments_t* arguments = state->input;

    switch(key)
    {
        case 'w':
            {
                arguments->outputInWav = 1;
                break;
            }
        case 'o': 
            {
                arguments->outputFileName = arg;
                break;
            }
        case ARGP_KEY_ARG:
            {
                (arguments->arg_count)++;
                if(arguments->arg_count == 1)
                {
                    arguments->inputFileNames = malloc(sizeof(char*));
                }
                else
                {
                    arguments->inputFileNames = 
                        realloc(arguments->inputFileNames,
                                arguments->arg_count*sizeof(char*));
                }

                arguments->inputFileNames[arguments->arg_count-1] = arg;
                break;
            }
        case ARGP_KEY_END:
            {
                break; 
            }
    }
    return 0;
}

int main(int argc, char** argv)
{
    char helpString[] = "--help"; //argp uses --help as the help flag.
    //this converts any -h flags to --help.
    int i;
    for(i=0; i < argc; i++)
    {
        if(!strcmp(argv[i], "-h"))
        {
            argv[i] = helpString;
        }
    }

    struct argp_option options[] = 
    {
        {0, 'h', 0, 0, "Give this help list"},
        {0, 'g', "INT", 0, "Output to FILE instead of stdout"},
        {0, 'a', 0, 0, "Output in .wav format, otherwise .cs229"},
        {0, 'a', 0, 0, "Output in .wav format, otherwise .cs229"},
        { 0 }
    };



    arguments_t arguments; 
    arguments.outputFileName = NULL;
    arguments.arg_count = 0; //maximum arguments is one
    arguments.outputInWav = 0;//by default print .cs229 format


    struct argp argp = {options, parse_opt, "[SOUND FILE [SOUND FILE [SOUND FILE...]]]",
        "sndcat is a program that reads in sound file(s) "
            "of type .wav or .cs229 and writes a single sound file "
            "that is a concatenation of the inputs.\v"
            "If no files are "
            "given as input then sndcat reads from stdin."};
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
