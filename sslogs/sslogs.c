/*
BSD 3-Clause License

Copyright (c) 2020, manexport<manexport@yeah.net>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>  
#include <time.h>
#include "pthread.h"
#include "sslogs.h"


#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <pthread.h>
#endif


typedef struct
{
	ssize_t glog_level;
	ssize_t total_size;
	char * glog_buf;
	FILE * glog_file;
	char str_tag[64];
	pthread_mutex_t mutex;
}sslog_block;



static ssize_t sslogs_lock(llong_t hd)
{
	int iRet = 0;
	sslog_block* in = (sslog_block*)hd;
	pthread_mutex_lock(&in->mutex);
	return iRet;
}

static ssize_t sslogs_unlok(llong_t hd)
{
	int iRet = 0;
	sslog_block* in = (sslog_block*)hd;
	pthread_mutex_unlock(&in->mutex);
	return iRet;
}

char * time_stamp()
{
static char time_buffer[128];
#ifdef _WIN32
	SYSTEMTIME	time;
	GetLocalTime(&time);
	_snprintf_s(time_buffer,sizeof(time_buffer),_TRUNCATE,"%04d-%02d-%02d %02d:%02d:%02d.%02d",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,time.wMilliseconds);
#else
	struct timeval tv;  
	struct tm *     time_ptr;  
	
	memset(&tv, 0, sizeof(struct timeval));  
	gettimeofday(&tv, NULL);  
	time_ptr = localtime(&tv.tv_sec);  
	sprintf(time_buffer, "%d%02d%02d %02d:%02d:%02d.%.04d", time_ptr->tm_year + 1900,time_ptr->tm_mon + 1,time_ptr->tm_mday,time_ptr->tm_hour,time_ptr->tm_min,time_ptr->tm_sec,tv.tv_usec);  
#endif 
	return time_buffer;  
}

llong_t sslogs_init(int level, int max_size, const char * out_path)
{
	int iRet = 0;
	sslog_block* in = (sslog_block*)malloc(sizeof(sslog_block));

	if (max_size > 0){
		in->total_size = max_size;
	}
	pthread_mutex_init(&in->mutex,NULL);
	in->glog_buf = malloc(sizeof(char), in->total_size);

	if (out_path){
		char * log_file = malloc(sizeof(char), 
			strlen(out_path) + 1 + strlen(LOG_FILE_NAME) + 1 + strlen(in->str_tag) + 1);
		if (log_file){
			if (in->str_tag[0] != 0){
				sprintf(log_file, "%s/%s_%s", out_path, in->str_tag, LOG_FILE_NAME);
			}else{
				sprintf(log_file, "%s/%s", out_path, LOG_FILE_NAME);
			}
			in->glog_file = fopen(log_file, "wb");
			free(log_file);
		}
	}

	in->glog_level = level;
	return iRet;
}

SSLOGS_EXTERN ssize_t sslogs_uninit(llong_t hd)
{
	int iRet = 0;
	sslog_block* in = (sslog_block*)hd;

	if (in->glog_file){
		fclose(in->glog_file);
		in->glog_file = NULL;
	}

	if (in->glog_buf){
		free(in->glog_buf);
		in->glog_buf = NULL;
	}
	return iRet;
}

SSLOGS_EXTERN ssize_t sslogs_setlevel(llong_t hd, int iLevel)
{
	sslog_block* in = (sslog_block*)hd;

	int iOld = in->glog_level;
	in->glog_level = iLevel;
	return iOld;
}

SSLOGS_EXTERN ssize_t sslogs_setpath(llong_t hd, const char * strWorkPath)
{
	int iRet = 0;
	sslog_block* in = (sslog_block*)hd;

	return iRet;
}

ssize_t sslogs_print(llong_t hd, int iLevel, char * strFmt, ...)
{
	int iPrintfRet = 0;
	sslog_block* in = (sslog_block*)hd;

	if(in->glog_level== SSLOGS_CLOSE)
	{
		return iPrintfRet;
	}
	sslogs_lock(hd);
	va_list marker;

	if (!in->glog_buf){
		sslogs_unlok(hd);
		return 0;
	}

	va_start(marker, strFmt);
	while((iPrintfRet = vsnprintf(in->glog_buf, in->total_size, strFmt, marker)) == -1){
		if (in->total_size >= 1024 * 100){
			break;
		}
		printf ("INLOG : realloc to %d\n", in->total_size * 2);
		char * pTmpBuf = malloc(sizeof(char), in->total_size * 2);
		if (pTmpBuf){
			free(in->glog_buf);
			in->glog_buf = pTmpBuf;
			in->total_size *= 2;
		}else{
			break;
		}
	}
	va_end(marker);
		
	if(iLevel >= in->glog_level)
	{
		if (iPrintfRet != -1){
			printf("[%s][%s]%s", time_stamp(), in->str_tag, in->glog_buf);
		}
	}

	if (iPrintfRet != -1){
		if (in->glog_file){
			fprintf(in->glog_file, "[%s][%s]%s", time_stamp(), in->str_tag, in->glog_buf);
			fflush(in->glog_file);
		}
	}else{
		
	}

	sslogs_unlok(hd);
	return iPrintfRet;
}

ssize_t sslogs_setlogtag(llong_t hd, char * tag)
{
	int iRet = 0;
	sslog_block* in = (sslog_block*)hd;

	memset (in->str_tag, 0, sizeof(in->str_tag));
	strncpy(in->str_tag, tag, MIN(sizeof(in->str_tag)-1, strlen(tag)));
	return iRet;
}

