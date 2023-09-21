#include "Common.h"

void* File_Writing_Thread_codigo(void * arg)
{
    struct local_writer_arg_struct *argumentos = arg;
    FILE* dest_file;
    
    dest_file = fopen(argumentos->Write_File_Name, "w"); 
    fclose(dest_file);

    struct Info_t t1;
    t1.status = Start;
    strcpy(t1.filename,argumentos->Write_File_Name);
    t1.destFile = dest_file;
    t1.TotalBytesRcv = argumentos->bytes_recv_total;
    t1.LastBytesRcv = argumentos->bytes_recv_ult;
    t1.cont = 1;
    t1.lock = argumentos->lock;
    TimerInit(&t1);


    while(*(argumentos->salir) == 0)
    {
    }

    return NULL;
}
