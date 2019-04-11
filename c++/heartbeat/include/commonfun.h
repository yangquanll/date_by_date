#pragma once

#include "Log.h"
#include <stdio.h>


//#ifndef _SYKG
//#define _SYKG
//#endif

#ifndef _KML3
#define _KML3
#endif

//#ifndef _BJ100TRAM
//#define _BJ100TRAM
//#endif


#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
#define INVALID_DEVICE	-1

#ifndef MAX_CHANNEL
#define MAX_CHANNEL	24
#endif

# define cfprintf(log_type, str, args...) do { string format; format.append("%s,%s,").append(str);	\
				switch(log_type)	\
				{	\
				case DEBUG_LOG:	Log::log.print(DEBUG_LOG, format.c_str(), "[DBG]", "[commonfun]", ## args);break;	\
				case OPERATE_LOG:Log::log.print(OPERATE_LOG, format.c_str(), "[OPR]", "[commonfun]", ## args);break;	\
				case ERROR_LOG:Log::log.print(ERROR_LOG, format.c_str(), "[ERR]", "[commonfun]", ## args);break;	\
				default :	Log::log.print(DEBUG_LOG, format.c_str(), "[DBG]", "[commonfun]", ## args);break;	\
				}}while(0)

#define SAFE_DELETE(p) {if((p) != NULL) delete [] (p); (p) = NULL;}


bool readLocalEth(char *interface, unsigned int *addr, unsigned int *netmask, unsigned char *mac);

void usleep_safe(int usecs); /* by lq 2016-05-06 for camera time sync. */

