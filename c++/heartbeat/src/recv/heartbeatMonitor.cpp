/* add 2017-09-08 for heartbeat BEGIN */
#include <stdio.h>
#include <unistd.h>

#include "heartbeat.h"

#define LOG_HEARTBEAT_FILE_PATH "./log_heartbeat"

extern void hb_usleep(int usecs);

bool HeartbeatPkgUT_ON() { return false; }
bool HeartbeatDbg_ON() { return false; }

HeartbeatMonitor heartbeatMonitor;

int main(int /*argc*/, char * /*argv*/[])
{
	Log::log.start((char*)LOG_HEARTBEAT_FILE_PATH);
	
	Log::log.print("===========start heartbeatMonitor===============");

	heartbeatMonitor.start();	
	
	while(1)
	{
		hb_usleep(1000 * 1000);
	}
	
	return 0;
}
/* add 2017-09-08 for heartbeat END */

