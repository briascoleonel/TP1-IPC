#include "Common.h"

void* File_Writing_Thread_codigo(void * arg)
{
    struct local_writer_arg_struct *arguments = arg;
    FILE* destFile;
    
    destFile = fopen(arguments->Write_File_Name, "w"); 
    fclose(destFile);

    while(*(arguments->salir) == 0)
    {
    }

    return NULL;
}
