#include <net/if.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

#include "commonfun.h"

bool readLocalEth(char *interface, unsigned int *addr, unsigned int *netmask, unsigned char *mac)
{
	int fd;
	struct ifreq ifr;
	struct sockaddr_in *our_ip;

	memset(&ifr, 0, sizeof(struct ifreq));
	if((fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) >= 0) {
		ifr.ifr_addr.sa_family = AF_INET;
		strcpy(ifr.ifr_name, interface);

		if (addr) { 
			if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) {
				our_ip = (struct sockaddr_in *) &ifr.ifr_addr;
				*addr = our_ip->sin_addr.s_addr;
			} else {
				cfprintf(ERROR_LOG, "<readLocalEth> -- ioctrl(SIOCGIFADDR) failed, error(%s)", strerror(errno));
				close(fd);
				return false;
			}
		}

		ifr.ifr_addr.sa_family = AF_INET;
		strcpy(ifr.ifr_name, interface);
		if (netmask) { 
			if (ioctl(fd, SIOCGIFNETMASK, &ifr) == 0) {
				our_ip = (struct sockaddr_in *) &ifr.ifr_addr;
				*netmask = our_ip->sin_addr.s_addr;
			} else {
				cfprintf(ERROR_LOG, "<readLocalEth> -- ioctrl(SIOCSIFNETMASK) failed, error(%s)", strerror(errno));
				close(fd);
				return false;
			}
		}

		if (mac)
		{
			if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0) {
				memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
			} else {
				cfprintf(ERROR_LOG, "<readLocalEth> -- ioctrl(SIOCGIFHWADDR) failed, error(%s)", strerror(errno));
				close(fd);
				return false;
			}
		}
		
	} else {
		cfprintf(ERROR_LOG, "<readLocalEth> -- socket failed, error(%s)", strerror(errno));
		close(fd);
		return false;
	}
	close(fd);
	return true;
}

/* by lq 2016-05-06 for camera time sync. BEGIN */
void usleep_safe(int usecs)
{
  struct timespec delayTime, elaspedTime;
  
  delayTime.tv_sec  = usecs / 1000000;
  delayTime.tv_nsec = (usecs % 1000000) * 1000;

  nanosleep(&delayTime, &elaspedTime);
}
/* by lq 2016-05-06 for camera time sync. END */

