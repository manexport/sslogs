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
#ifndef __SSLOGS_H__
#define __SSLOGS_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SSLOGS_STATICLIB
#  define SSLOGS_EXTERN
#elif defined(_WIN32)					 
#  ifdef SSLOGS_EXPORTS
#    define SSLOGS_EXTERN __declspec(dllexport)
#  else /* !SSLOGS_EXPORTS */
#    define SSLOGS_EXTERN __declspec(dllimport)
#  endif /* !SSLOGS_EXPORTS */
#else    /* !defined(_WIN32) */
#  ifdef SSLOGS_EXPORTS
#    define SSLOGS_EXTERN __attribute__((visibility("default")))
#  else /* !SSLOGS_EXPORTS */
#    define SSLOGS_EXTERN
#  endif /* !SSLOGS_EXPORTS */
#endif   /* !defined(_WIN32) */

#define LOG_FILE_NAME	           "example.log"
#define MIN                        (x, y)  (x) < (y) ? (x) : (y)
#define ssize_t                    int
#define SSLOGS_CLOSE               (0x0001)
extern sslogs g_Logger;

#define SSLOG_INIT(_s,_c)		g_Logger.Init(_s, _c)
#define SSLOG_REINIT(_s, _c)		g_Logger.ReInit(_s, _c)

#define __SHORT_FILE__ (strrchr(__FILE__, PATH_SEPERATOR) ? strrchr(__FILE__, PATH_SEPERATOR) + 1 : __FILE__)
#define MAKE_PREFIX(fmt, ...)		std::string("%d| ").append(fmt).append(" (%s:%d)").c_str(), PROCESS_ID,  ##__VA_ARGS__, __SHORT_FILE__, __LINE__
#define MAKE_PREFIX_ERROR(fmt, ...)	std::string("%d| [%s] ").append(fmt).append(" (%s:%d)").c_str(), PROCESS_ID, __FUNCTION__, ##__VA_ARGS__, __SHORT_FILE__, __LINE__
#define MAKE_PREFIX_DEBUG(fmt, ...)	std::string("%d| ").append(fmt).c_str(), PROCESS_ID, ##__VA_ARGS__

#define SSLOG_INFO(fmt, ...)			do{if(g_Logger.IsReady())g_Logger.m_CategoryInfo->info  (MAKE_PREFIX(fmt, ##__VA_ARGS__));}while(0)
#define SSLOG_WARN(fmt, ...)			do{if(g_Logger.IsReady())g_Logger.m_CategoryError->warn (MAKE_PREFIX_ERROR(fmt, ##__VA_ARGS__));}while(0)
#define SSLOG_ERROR(fmt, ...)			do{if(g_Logger.IsReady())g_Logger.m_CategoryError->error(MAKE_PREFIX_ERROR(fmt, ##__VA_ARGS__));}while(0)
#define SSLOG_DEBUG(fmt, ...)			do{if(g_Logger.IsReady())g_Logger.m_CategoryDebug->debug(MAKE_PREFIX(fmt, ##__VA_ARGS__));}while(0)

#define SSLOG_ENTER						SSLOG_INFO("[%s|BEG]", __FUNCTION__)
#define // SSLOG_ENTER_ARGS(fmt, ...)		SSLOG_INFO(std::string("[%s|BEG] ").append(fmt).c_str(), __FUNCTION__, ##__VA_ARGS__)
#define SSLOG_LEAVE						do{SSLOG_INFO("[%s|RET]", __FUNCTION__);}while(0)
#define // SSLOG_LEAVE_ARGS(fmt, ...)		do{SSLOG_INFO(std::string("[%s|RET] ").append(fmt).c_str(), __FUNCTION__, ##__VA_ARGS__);}while(0)
#define SSLOG_RETURN(rc)					do{SSLOG_INFO("[%s|RET|%d]", __FUNCTION__, rc); return rc;}while(0)
#define SSLOG_RETURN_ARGS(rc, fmt, ...)	do{SSLOG_INFO(std::string("[%s|RET|%d] ").append(fmt), __FUNCTION__, rc, ##__VA_ARGS__); return rc;}while(0)

/*-----------------------------------------------------------*/
#define SSLOG_INIT(_s,_c)		g_jlogger.Init(_s, _c)

#define MAKE_PREFIX(fmt) std::string("%d : (%s:%d) ").append(fmt).c_str(), getpid(), __SHORT_FILE__, __LINE__
#define MAKE_SHORTPREFIX(fmt) std::string("%d : ").append(fmt).c_str(), getpid()

#define SSLOG_DEBUG(fmt, ...)	do{if(g_jlogger.is_init())g_jlogger.category_->debug(MAKE_PREFIX(fmt),     ##__VA_ARGS__);}while(0)
#define SSLOG_NOTICE(fmt, ...)   do{if(g_jlogger.is_init())g_jlogger.category_->notice(MAKE_PREFIX(fmt),    ##__VA_ARGS__);}while(0)
#define SSLOG_INFO(fmt, ...)	    do{if(g_jlogger.is_init())g_jlogger.category_->info(MAKE_PREFIX(fmt),      ##__VA_ARGS__);}while(0)

#define SSLOG_WARN(fmt, ...)		do{if(g_jlogger.is_init())g_jlogger.category_wf_->warn(MAKE_PREFIX(fmt),	 ##__VA_ARGS__);}while(0)
#define SSLOG_ERROR(fmt, ...)	do{if(g_jlogger.is_init())g_jlogger.category_wf_->error(MAKE_PREFIX(fmt),  ##__VA_ARGS__);}while(0)
#define SSLOG_FATAL(fmt ,...)	do{if(g_jlogger.is_init())g_jlogger.category_wf_->fatal(MAKE_PREFIX(fmt),  ##__VA_ARGS__);}while(0)

#define SSLOG_RETURN(rt)					do{SSLOG_INFO("%s return %d", __FUNCTION__, rt); return rt;}while(0)
#define SSLOG_RETURN_ARGS(rt, fmt, ...)	do{SSLOG_INFO(std::string("%s return %d ").append(fmt), __FUNCTION__, rt, ##__VA_ARGS__); return rt;}while(0)


#define SSLOG_INIT(_s,_c)

#define MAKE_PREFIX(fmt) std::string("(%s:%d) ").append(fmt).append("\n").c_str(), __SHORT_FILE__, __LINE__
#define MAKE_SHORTPREFIX(fmt) std::string("%d : ").append(fmt).c_str(), getpid()

#define SSLOG_DEBUG(fmt, ...)     printf(MAKE_PREFIX(fmt),	##__VA_ARGS__)
#define SSLOG_NOTICE(fmt, ...)    printf(MAKE_PREFIX(fmt),	##__VA_ARGS__)
#define SSLOG_INFO(fmt, ...)		 printf(MAKE_PREFIX(fmt),	##__VA_ARGS__)

#define SSLOG_WARN(fmt, ...)			printf(MAKE_PREFIX(fmt),	##__VA_ARGS__)
#define SSLOG_ERROR(fmt, ...)	    printf(MAKE_PREFIX(fmt),	##__VA_ARGS__)
#define SSLOG_FATAL(fmt ,...)	    printf(MAKE_PREFIX(fmt),	##__VA_ARGS__)

#define SSLOG_RETURN(rt)					do{SSLOG_INFO("%s return %d", __FUNCTION__, rt); return rt;}while(0)
#define SSLOG_RETURN_ARGS(rt, fmt, ...)	do{SSLOG_INFO(std::string("%s return %d ").append(fmt), __FUNCTION__, rt, ##__VA_ARGS__); return rt;}while(0)

#define JFUNC_TRACE() JFuncTracer jfunc_tracer(__FUNCTION__, __FILE__, __LINE__)


	typedef enum
	{
		SSLOGS_ERROR_NONE = 0x00,
		SSLOGS_INVALID_ARG,
		SSLOGS_BUFFER_ERROR,
		SSLOGS_ERR_FATAL,
		SSLOGS_ERR_NOMEM,
		SSLOGS_ERR_UNKNOWN = -1,
	}SSLOGS_ERR;

	typedef unsigned long long llong_t;
	SSLOGS_EXTERN llong_t sslogs_init(int level, int max_size, const char * out_path);
	SSLOGS_EXTERN ssize_t sslogs_uninit(llong_t hd);
	SSLOGS_EXTERN ssize_t sslogs_setlevel(llong_t hd, int iLevel);
	SSLOGS_EXTERN ssize_t sslogs_setpath(llong_t hd, const char * strWorkPath);
	SSLOGS_EXTERN ssize_t sslogs_print(llong_t hd, int iLevel, char * strFmt, ...);
	SSLOGS_EXTERN ssize_t sslogs_setlogtag(llong_t hd, char * tag);




#ifdef __cplusplus
}
#endif
#endif // __SSLOGS_H__