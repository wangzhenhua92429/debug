#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdarg.h>
#include <dirent.h>
#include <pthread.h>

#include "debug.h"

#define LINE_SIZE 256
#define FILE_SIZE 3*1024*1024L

#define FILE_PATH "./log.txt"

pthread_mutex_t lock;
char* logPath = "/";   //日志文件目录

static int printf2terminal(char* _str)
{
    if(NULL == _str)
        return ERROR;
    
    //LOGE(_str);
    return SUCCESS;
}

int delFirstLine()
{
    int ret = ERROR;
    FILE* fp = NULL;
    char fileContent[FILE_SIZE] = "";
    char* p = NULL;
    
    fp = fopen(FILE_PATH, "r+b");
    if(fp < 0)
    {
        printf("open error\n");
        return ERROR;
    }

    pthread_mutex_lock(&lock);
    ret = fread(fileContent, sizeof(char), FILE_SIZE, fp);
    if(ret <= 0)
    {
        printf("read error\n");
        pthread_mutex_unlock(&lock);
        return ERROR;
    }
    pthread_mutex_unlock(&lock);
    
    p = strstr(fileContent, "\n");
    strcpy(fileContent, p + 1);

    ftruncate(fileno(fp), 0);   //清空文件内容
    fseek(fp, 0L, SEEK_SET);
    
    pthread_mutex_lock(&lock);
    fwrite(fileContent, sizeof(char), strlen(fileContent), fp);
    pthread_mutex_unlock(&lock);
    
    fclose(fp);
    return SUCCESS;
}

void write2file(char* _str)
{
    time_t now;
    struct tm tm_now;
    int ret = ERROR;
    FILE* fd = NULL;
    char result[LINE_SIZE] = "";
    long pos = 0;

    pthread_mutex_init(&lock, NULL);
    fd = fopen(FILE_PATH, "ab+");
    if(fd < 0)
        return;
    
    //行头
    now = time(&now);
    localtime_r(&now, &tm_now);
    sprintf(result, "[%04d/%02d/%02d %02d:%02d:%02d]: %s",
                    tm_now.tm_year+1900,
                    tm_now.tm_mon+1,
                    tm_now.tm_mday,
                    (tm_now.tm_hour)%24,
                    tm_now.tm_min,
                    tm_now.tm_sec,
                    _str);
    //计算文件大小
    fseek(fd, 0L, SEEK_END);
    pos = ftell(fd);
    //预留一行的空间
    if(pos > (FILE_SIZE - LINE_SIZE))
    {
        delFirstLine();
    }
    
    pthread_mutex_lock(&lock);
    fwrite(&result, sizeof(char), strlen(result), fd);
    pthread_mutex_unlock(&lock);
    
    fclose(fd);
}

static int send2server()
{
    return 0;
}

int JL_printf(LOG_LEVEL _logLevel, char* pszfmt, ...)
{
    va_list struAp;
    char* format = NULL;
    char content[128] = {0};
    LOG_LEVEL logLevel = _logLevel;

    if(NULL == pszfmt)
        return -1;

    format = calloc(strlen(pszfmt)+3, 1);
    if(NULL == format)
        return -2;

    strcpy(format, pszfmt);
    if('\n' == format[strlen(pszfmt)-1])
    {
        format[strlen(pszfmt)] = format[strlen(pszfmt)-2] == '\r'?'\0':'\r';
    }
    else if('\r' == format[strlen(pszfmt)-1])
    {
        format[strlen(pszfmt)] = format[strlen(pszfmt)-2] == '\n'?'\0':'\n';
    }
    else
    {
        format[strlen(pszfmt)] = '\r';
        format[strlen(pszfmt)+1] = '\n';
    }
    
    va_start(struAp, pszfmt);
    vsprintf(content, format, struAp);
    va_end(struAp);

    //printf2terminal(content);
    if(logLevel <= LEVEL_LOG_WARNIG)
        write2file(content);
    
    free(format);
    return 0;
}

