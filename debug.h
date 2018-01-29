#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef __cplusplus 
extern "C" {
#endif

#define ERROR    -1
#define SUCCESS  0

/* enable/disable global debug */
#define DBG_EN 				0 	/* 0 - disable, 1 - enable */

typedef enum
{
    JL_LOG_ERROR = 0,
    JL_LOG_WARNING,
    JL_LOG_DEBUG,
    JL_LOG_NOTICE,
    JL_LOG_INFO,
}PRINTF_LEVEL;

#define SYS_XXX_DBG_EN          (DBG_EN && 1)   /* 0 - disable, 1 - enable */

typedef enum
{
    LEVEL_LOG_ERROR = 2,
    LEVEL_LOG_WARNIG = 4,
    LEVEL_LOG_DEBUG = 6,
    LEVEL_LOG_NOTICE = 8,
    LEVEL_LOG_INFO = 10,
}LOG_LEVEL;

int JL_printf(LOG_LEVEL _logLevel, char* pszfmt, ...);

#ifdef __cplusplus
}
#endif 

#endif
