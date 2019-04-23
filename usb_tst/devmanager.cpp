#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/vfs.h>
#include <sys/mount.h>
#include <sys/types.h> 
#include <dirent.h> 
#include <time.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/cdrom.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "devmanager.h"

#define UEVENT_BUFFER_SIZE 2048
#define FILE_DEVICE					"/proc/scsi/scsi"  //all device info
#define FILE_DEV_PART_TEMPL     	"/dev/sr" //cd device
#define USB_CDROM_MP				"/ssa/Samba/Share/USB-ODD"
#define FILE_DEVICE_PARTS			"/proc/partitions" //device part info
#define USB_MEM_MP                	"/ssa/Samba/Share/USBMemory"
#define USB_DISK_MP                 "/ssa/Samba/Share/USB-HDD"
#define DISK_MP                 	"/ssa/Samba/Share/HDD"
#define FILE_MOUNT_CHECK        	"/proc/mounts"   //check mount status
#define USB_SDCARD_MP               "/ssa/Samba/Share/sdcard"
#define REC_BUS_PATH_CMD			"ls /sys/bus/scsi/devices/0:0:0:0/block"
#define REC_MP						"/ssa/record"
#define CDROM_MP					"/ssa/cdrom"
#define DIR_PATH 					"/ssa/Samba/Share/HDD/HDD"

// global parameter
pthread_mutex_t devInfolock;
static SCSI_USB_DEV *pFirstDev = NULL;
static SCSI_USB_DEV *pOldDev = NULL;
static pthread_t threadId;
static int stopFlag = 0;
static char recHdd[10] = {0};
static int sdEjectFlag = 0;
static int usbHddEjectFlag = 0;
static int usbEjectFlag = 0;
static int cdromEjectFlag = 0;
static int recordMountStatus = 0;
static int cdromMountStatus = 0;
static pthread_t mediaThreadId;
//static pthread_t netLinkThreadId;
static int stopNetLinkFlag = 0;
static int stopMediaFlag = 0;
static int cdromMountFlag = 0;
static int cdromMountOldFlag = 0;
static int usbHddFlag = 0;
static int usbExistFlag = 0;
static int usbMountFlag = 0;
static int hddExitFlag = 0;

// function declaration
static int initDev(SCSI_USB_DEV *dev, int index, char *type, int *devIndex);
static int calcDeviceParts(SCSI_USB_DEV *dev, int *devIndex);
static int addDev(SCSI_USB_DEV *dev);
static int findDevice();
static void clearDev(void);
static void *getDevInfoThread(void *pPara);
static int doUmount(SCSI_USB_DEV *dev);
static void doMount();
static int isMount(char *pParts);
static void searchSdcard();
static int isUsbDisk(char * devpath);
static char *getDir(int type);
static long long getDiskInfo(char *pPath);
static void getRecHddName();
static void mountDev(SCSI_USB_DEV *pDevInfo, int index);
static void mountCdrom();
static void umountCdrom();
//static void netLinkThread();
static void *mediaCheckThread(void *pPara);
int isNtfsFromat(char *pPath);

// get dev info
SCSI_USB_DEV *getDevInfo()
{
#if 1
	SCSI_USB_DEV *pDevInfo = pOldDev;
	while(NULL != pDevInfo)
	{
		printf("****** pDevInfo devType = %d\n", pDevInfo->devType);
		printf("****** pDevInfo type = %d\n", pDevInfo->type);
		printf("****** pDevInfo part_num = %d\n", pDevInfo->part_num);
	
		printf("****** pDevInfo mount_path = %s\n", pDevInfo->mount_path[0]);
		printf("****** pDevInfo freeDiskSize = %lld\n", pDevInfo->freeDiskSize[0]);
		pDevInfo = pDevInfo->next_dev;
	}
#endif
	return pOldDev;
}

SCSI_USB_DEV *getUSBDevInfo()
{
	SCSI_USB_DEV *pDevInfo = pOldDev;
	while(NULL != pDevInfo)
	{
		if(pDevInfo->type == 2)
			return pDevInfo;
		
		pDevInfo = pDevInfo->next_dev;
	}
	return pOldDev;
}

// get dev info stop
void stopGetDevInfo()
{
	stopFlag = 1;
	stopNetLinkFlag = 1;
	stopMediaFlag = 1;
	pthread_mutex_lock(&devInfolock);
	SCSI_USB_DEV *pDevInfo = pOldDev;
	printf("stopGetDevInfo start\n");
	while (NULL != pDevInfo)
	{
		//printf("stopGetDevInfo doUmount\n");
		doUmount(pDevInfo);

		pDevInfo = pDevInfo->next_dev;
	}
	clearDev();
	pthread_mutex_unlock(&devInfolock);
	pthread_mutex_destroy(&devInfolock);

	if (access(REC_MP, R_OK) == 0)
	{
		if (umount(REC_MP) == 0)
		{
			remove(REC_MP);
		}
	}
}

// get dev info start
int startGetDevInfo()
{
	pthread_mutex_init(&devInfolock, NULL);
	getRecHddName();

    if (0 != pthread_create(&threadId, NULL, getDevInfoThread, NULL))
    {
        printf("[%s] Thread getDevInfoThread create failed\n", __FUNCTION__);
		return -1;
    }
	
	/*if (0 != pthread_create(&netLinkThreadId, NULL, (void *)netLinkThread, NULL))
    {
        printf("[%s] Thread netLincThread create failed\n", __FUNCTION__);
		return -1;
    }*/

	/*
	if (0 != pthread_create(&mediaThreadId, NULL, mediaCheckThread, NULL))
	{
		printf("[%s] Thread mediaCheckThread create failed\n", __FUNCTION__);
		return -1;
	}
	*/

	return 0;
}

static void *getDevInfoThread(void *pPara)
{
	int delFlag = 1;

	while(stopFlag == 0)
	{
		pFirstDev = NULL;
		usbHddFlag = 0;
		usbExistFlag = 0;
		findDevice();
        searchSdcard();
		
		if (NULL == pOldDev && pFirstDev != NULL)
		{
			pthread_mutex_lock(&devInfolock);
			pOldDev = pFirstDev;
			pthread_mutex_unlock(&devInfolock);
		}
		else
		{
			pthread_mutex_lock(&devInfolock);
			SCSI_USB_DEV *pDevInfo = pOldDev;

			while (NULL != pDevInfo)
			{
				SCSI_USB_DEV *pTempDev = pFirstDev;
				
				while (NULL != pTempDev)
				{
					if (0 == strcmp(pTempDev->devfile_parts[0], pDevInfo->devfile_parts[0]))
					{
						delFlag = 0;
						break;
					}
					pTempDev = pTempDev->next_dev;
				}

				if (delFlag == 1)
				{
					doUmount(pDevInfo);
				}
				else
				{
					delFlag = 1;
				}
				pDevInfo = pDevInfo->next_dev;
			}
			if(pFirstDev != NULL)
			{
				clearDev();
				pOldDev = pFirstDev;
			}
			pthread_mutex_unlock(&devInfolock);
		}
		
		doMount();
		if (usbHddFlag == 0)
		{
			usbHddEjectFlag = 0;
		}
		if (usbExistFlag == 0)
		{
			usbEjectFlag = 0;
		}
		usleep(300 * 1000);
		
	}
}

// delete old dev info
static void clearDev(void)
{
    SCSI_USB_DEV *cur_dev = pOldDev;
    SCSI_USB_DEV *tmp_dev;

    while (NULL != cur_dev)
    {
        tmp_dev = cur_dev;
        cur_dev = cur_dev->next_dev;
        free(tmp_dev);
    }
    pOldDev = NULL;
}

// search device
static int findDevice()
{
    FILE *fd = NULL;
    int len = 0;
    int dev_num = 0;
    char buf[1024] = {0};
    char *seek = NULL;
    int usbDevIndex = 1;

    // open device info file
    fd = fopen(FILE_DEVICE, "r");

    if (fd > 0)
    {
		// read dev info
        len = fread(buf, 1, sizeof(buf), fd);

        fclose(fd);
		pFirstDev = NULL;

		// no device
        if (len < 20)
        {
            return -1;
        }

        seek = buf;
		if (strstr(seek, "CD-ROM") == NULL)
		{
			cdromEjectFlag = 0;
		}
        while (seek != NULL && ((strlen(seek)) > 0))
        {
            seek = strstr(seek, "Host: scsi");
            if (seek && (seek[0] == 'H'))
            {
                seek += strlen( "Host: scsi");
                seek = strstr(seek, "Type:");
                if (seek && (seek[0] == 'T'))
                {
                    seek += strlen("Type:");
                    while (*seek == ' ')
                    {
                        seek++;
                    }
					SCSI_USB_DEV *new_dev = NULL;
                    new_dev = (SCSI_USB_DEV *)malloc(sizeof(SCSI_USB_DEV));
					if (new_dev != NULL)
					{
						int initRet = 0;
						memset(new_dev, 0, sizeof(SCSI_USB_DEV));
		                initRet = initDev(new_dev, dev_num, seek, &usbDevIndex);
						if (initRet == 0)
						{
							if ((usbHddEjectFlag == 1 && new_dev->type == 3)
								|| (usbEjectFlag == 1 && new_dev->type == 2)
								|| (cdromEjectFlag == 1 && new_dev->type == 1))
							{
								free(new_dev);
								new_dev = NULL;
								continue;
							}
							else
							{
								addDev(new_dev);
				            	dev_num++;
							}
						}
						else
						{
							free(new_dev);
							new_dev = NULL;
						}
					}
					else
					{
						printf("malloc error\n");
					}
                }
            }
        }
    }
    else
    {
		pFirstDev = NULL;
		printf("open %s error\n", FILE_DEVICE);
    }

    return dev_num;
}

static int initDev(SCSI_USB_DEV *dev, int index, char *type, int *devIndex)
{
	dev->devType = 1;
    if (0 == strncmp(type, "CD-ROM", 6))//cd-rom
    {
	    dev->type = 1;
	    dev->part_num = 1;
		char cdRompath[10] = {0};
		int index = 0;

		for(index = 0; index < 10; index++)
		{
			sprintf(cdRompath, FILE_DEV_PART_TEMPL"%d", index);
			if (access(cdRompath, R_OK) == 0)
			{
				break;
			}
		}
		if (index == 10)
		{
			return -1;
		}
	    sprintf(dev->devfile_parts[0], cdRompath);
		strcpy(dev->mount_path[0], USB_CDROM_MP);
    }
    else//usb or hdd
    {
        dev->part_num = calcDeviceParts(dev, devIndex);
		if (dev->part_num <= 0)
		{
			return -1;
		}
    }

    return 0;
}

static int calcDeviceParts(SCSI_USB_DEV *dev, int *devIndex)
{
    int len = 0;
    int part_num = 0;
    char buf[1024] = {0};     //1024 is enough for save information of FILE partitions
    char *delim="\n";
    char *line = NULL;
    char *strtok_tmp_ptr = NULL;
    char *seek_sd = NULL;      //USED FOR DEVICE MOUNTED ON SD*
    FILE *fd = NULL;
	int endflag = 0;
	int serchIndex = 0;
	char devTypePath[64] = {0};
	int devType = 0;
	FILE *devfd = NULL;
	char typebuf[15] = {0};
	int usbFlag = 0;
	char devBuf[5] = {0};

    fd = fopen(FILE_DEVICE_PARTS, "r"); // open dev part file
    if (fd != NULL)
    {
        len = fread(buf, 1, sizeof(buf), fd);
        fclose(fd);

        if(len <= 0)
        {
			return part_num;
		}
        line = strtok_r(buf, delim, &strtok_tmp_ptr);//split string by "\n"
        while(line)
        {
            seek_sd = strstr(line, "sd");

            if (seek_sd)
            {
                if(seek_sd[2] >= 'a' && seek_sd[2] <= 'z')
                {
                    if(seek_sd[3] >= '1' && seek_sd[3] < '9')
                    {
						if (*devIndex != serchIndex)
						{
							line = strtok_r(NULL, delim, &strtok_tmp_ptr);
							continue;
						}
						memset(devBuf, 0, 5);
						memcpy(devBuf, seek_sd, 4);
                        sprintf(dev->devfile_parts[part_num], "/dev/%s", devBuf);
						if (devType == 1)
						{
							if (part_num > 0)
							{
								break;
							}
							sprintf(dev->mount_path[part_num], USB_MEM_MP"/USBMemory%d", part_num);
						}
						else
						{
							if (0 == usbFlag)
							{
								if (part_num > 0)
								{
									break;
								}
								sprintf(dev->mount_path[part_num], USB_DISK_MP"/USB-HDD%d", part_num);
							}
							else
							{
								dev->devType = 3;
								if (strstr(seek_sd, recHdd) != NULL)
								{
									if (part_num == 1)
									{
										// mount
										if (0 == recordMountStatus)
										{
											char mount_cmd[256] = {0};
											mkdir(REC_MP, 0777);
											sprintf(mount_cmd, "mount %s %s", dev->devfile_parts[part_num], REC_MP);
											if (system(mount_cmd) != -1)
											{
												recordMountStatus = 1;
											}
										}
										memset(dev->devfile_parts[part_num], 0, MAX_NAME_LEN);
										line = strtok_r(NULL, delim, &strtok_tmp_ptr);
										continue;
									}
									else
									{
										dev->index = 0;
										sprintf(dev->mount_path[part_num], DISK_MP"/HDD%d", part_num);
										hddExitFlag = 1;
									}
								}
								else
								{
									break;
								}
							}
						}
                        
                        part_num++;
                        if (part_num == MAX_PART_NUM)
                        {
                             break;//too many parts ignore
                        }
						endflag = 1;
                    }
					else
					{
						if (endflag == 1)
						{
							break;
						}

						serchIndex++;
						if (*devIndex == serchIndex)
						{
							memset(devBuf, 0, 5);
							memcpy(devBuf, seek_sd, 4);
							sprintf(devTypePath, "/sys/block/%s/removable", devBuf);
							devfd = fopen(devTypePath, "r"); // open dev type file
							if (devfd != NULL && fgets(typebuf, sizeof(typebuf), devfd))
							{
								typebuf[strlen(typebuf) - 1] = 0;
								devType = atoi(typebuf);
								memset(devTypePath, 0, 64);
								sprintf(devTypePath, "/dev/%s", devBuf);
								usbFlag = isUsbDisk(devTypePath);
								if (devType == 1)
								{
									dev->type = 2;
									usbExistFlag = 1;
								}
								else if (usbFlag == 0)
								{
									dev->type = 3;
									usbHddFlag = 1;
								}
								else
								{
									dev->type = 4;
								}
							}
							fclose(devfd);
						}
					}
                }
            }

            line = strtok_r(NULL, delim, &strtok_tmp_ptr);
        }
    }
    else
    {
        printf("open %s error\n", FILE_DEVICE_PARTS);
    }

	if (part_num == 1)
	{
		dev->mount_path[0][strlen(dev->mount_path[0]) - 1] = 0;
	}

	(*devIndex)++;

    return part_num;
}

static int isUsbDisk(char * devpath)
{
	struct stat buf;
	char link[512] = {0};
	char link_path[512]={0};
	char *pUsbLink = NULL;

	if (lstat(devpath,&buf) < 0)
	{
		printf("stat error\n");
		return -1;
	}

	if (S_ISBLK(buf.st_mode))
	{
		sprintf(link_path,"/sys/dev/block/%d:%d", major(buf.st_rdev), minor(buf.st_rdev));
		if (access(link_path, F_OK) >= 0 )
		{
			int i = readlink(link_path, link, 512);
			if (i == 0 )
			{
				return -1;
			}

			pUsbLink = strstr(link, "usb");
			if (NULL != pUsbLink)
			{
				return 0;
			}
		}
	}
	return -1;
}

static int addDev(SCSI_USB_DEV *dev)
{
    if (pFirstDev != NULL)
    {
        dev->next_dev = pFirstDev;
        pFirstDev = dev;
    }
    else
    {
        pFirstDev = dev;
        dev->next_dev = NULL;
    }
    return 0;
}

static void mountCdrom()
{
	int ret = 0;
	char mount_cmd[256] = {0};
	char cdRompath[10] = {0};
	int index = 0;

	for(index = 0; index < 10; index++)
	{
		sprintf(cdRompath, FILE_DEV_PART_TEMPL"%d", index);
		if (access(cdRompath, R_OK) == 0)
		{
			break;
		}
	}

	sprintf(mount_cmd, "mount %s %s", cdRompath, CDROM_MP);
	if (cdromMountStatus != 0)
	{
		if (cdromMountFlag != cdromMountOldFlag)
		{
			if (cdromMountFlag == 1)
			{
				if (access(CDROM_MP, R_OK) != 0)
				{
					mkdir(CDROM_MP, 0777);
				}
				ret = system(mount_cmd);
				if (ret == 0)
				{
					umountCdrom();
					memset(mount_cmd, 0, 256);
					sprintf(mount_cmd, "mount -t overlayfs -o lowerdir=%s,upperdir=%s overlay %s", CDROM_MP, REC_MP, USB_CDROM_MP);
					if (system(mount_cmd) == 0)
					{
						cdromMountOldFlag = cdromMountFlag;
						cdromMountStatus = 1;
					}
				}
				else
				{
					remove(CDROM_MP);
				}
			}
			else
			{
				cdromMountOldFlag = 0;
				umountCdrom();
			}
		}
		return;
	}
	
	mkdir(USB_CDROM_MP, 0777);
	mkdir(CDROM_MP, 0777);

	ret = system(mount_cmd);
	memset(mount_cmd, 0, 256);
	if (ret == 0)
	{
		sprintf(mount_cmd, "mount -t overlayfs -o lowerdir=%s,upperdir=%s overlay %s", CDROM_MP, REC_MP, USB_CDROM_MP);
	}
	else
	{
		remove(CDROM_MP);
		sprintf(mount_cmd, "mount %s %s", REC_MP, USB_CDROM_MP);
	}

	system(mount_cmd);
	cdromMountStatus = 1;
}

static void mountDev(SCSI_USB_DEV *pDevInfo, int index)
{
	char mount_cmd[256] = {0};
	char hddDir[256] = {0};

	char *pTmpdir = getDir(pDevInfo->type);
	int status = 0;
	if (strcmp(pTmpdir, DISK_MP) == 0)
	{
		if (access(DISK_MP, R_OK) != 0)
		{
			mkdir(DISK_MP, 0777);
		}
	}
	else
	{
		if (access(pTmpdir, R_OK) != 0)
		{
			mkdir(pTmpdir, 0777);
		}
	}
	memset(mount_cmd, 0, 256);
	
	if(mkdir(pDevInfo->mount_path[index], 0777) == -1)
	{
		printf("mkdir err %d.\n", errno);
	}

	if (isNtfsFromat(pDevInfo->devfile_parts[index]) == 1)
	{
		sprintf(mount_cmd, "ntfs-3g %s %s", pDevInfo->devfile_parts[index], pDevInfo->mount_path[index]);
	}
	else
	{
		sprintf(mount_cmd, "mount %s %s", pDevInfo->devfile_parts[index], pDevInfo->mount_path[index]);
	}
	status = system(mount_cmd);

	if (pDevInfo->type == 4)
	{
		pDevInfo->freeDiskSize[index] = getDiskInfo(pDevInfo->mount_path[index]);
		if (status == 0)
		{
			if (strstr(pDevInfo->devfile_parts[index], recHdd) == NULL)
			{
				memset(hddDir, 0, 256);
				sprintf(hddDir, "%s/AUTO", pDevInfo->mount_path[index]);
				if (access(hddDir, R_OK) != 0)
				{
					mkdir(hddDir, 0777);
				}

				if (access(DIR_PATH, R_OK) != 0)
				{
					mkdir(DIR_PATH, 0777);
				}
			}
			else
			{
				memset(hddDir, 0, 256);
				sprintf(hddDir, "%s/AUTO", pDevInfo->mount_path[index]);
				if (access(hddDir, R_OK) != 0)
				{
					mkdir(hddDir, 0777);
				}

				if (access(DIR_PATH, R_OK) != 0)
				{
					mkdir(DIR_PATH, 0777);
				}
				memset(hddDir, 0, 256);
				sprintf(hddDir, "%s/MANUAL", pDevInfo->mount_path[index]);
				//printf("hddDir = %s\n", hddDir);
				if (access(hddDir, R_OK) != 0)
				{
					mkdir(hddDir, 0777);
				}
			}
		}
	}
	else if(pDevInfo->type == 2)
	{
		usbMountFlag = 1;
		pDevInfo->freeDiskSize[index] = getDiskInfo(pDevInfo->mount_path[index]);
	}
}

void doMount()
{
	int index = 0;

	pthread_mutex_lock(&devInfolock);
	SCSI_USB_DEV *pDevInfo = pFirstDev;
	while (pDevInfo != NULL)
	{
		for (index = 0; index < pDevInfo->part_num; index++)
		{
			if (0 == isMount(pDevInfo->devfile_parts[index]))
			{
				if (pDevInfo->type == 4)
				{
					pDevInfo->freeDiskSize[index] = getDiskInfo(pDevInfo->mount_path[index]);
				}
				else if (pDevInfo->type == 2)
				{
					pDevInfo->freeDiskSize[index] = getDiskInfo(pDevInfo->mount_path[index]);
					usbMountFlag = 1;
				}
				else
				{
					break;
				}
			}
			else
			{
				if (pDevInfo->type == 1)
				{
					mountCdrom();
				}
				else
				{
					mountDev(pDevInfo, index);
				}
			}
		}
		
		pDevInfo = pDevInfo->next_dev;
	}

	pthread_mutex_unlock(&devInfolock);
}

static void umountCdrom()
{
	if (cdromMountStatus == 0)
	{
		return;
	}

	if (access(USB_CDROM_MP, R_OK) == 0)
	{
		char cmd[256] = {0};
		sprintf(cmd, "fuser -k "USB_CDROM_MP);
		system(cmd);
	    if (umount(USB_CDROM_MP) == 0)
	    {
			remove(USB_CDROM_MP);

			if (access(CDROM_MP, R_OK) == 0)
			{
				memset(cmd, 0, sizeof(cmd));
				sprintf(cmd, "fuser -k "CDROM_MP);
				system(cmd);
				if (umount(CDROM_MP) == 0)
				{
					remove(CDROM_MP);
				}
				else
				{
					printf("umount %s failed\n", CDROM_MP);
				}
			}
	    }
	    else
	    {
	        printf("umount %s failed\n", FILE_DEV_PART_TEMPL);
	    }
	}
	cdromMountStatus = 0;
}

static int doUmount(SCSI_USB_DEV *dev)
{
    int index = 0;

    if (dev->type == 1)//cdrom
    {
		cdromMountOldFlag = 0;
		umountCdrom();
    }
	else if (dev->devType == 2)
	{
		char cmd[256] = {0};
        for (index = 0; index < dev->part_num; index++)
        {
			if (access(dev->mount_path[index], R_OK) == 0)
			{
				sprintf(cmd, "fuser -k %s/SD", USB_SDCARD_MP);
				system(cmd);
				//printf("**** cmd = %s ****\n", cmd);
		        if (umount(USB_SDCARD_MP"/SD") == 0)
		        {
		            remove(USB_SDCARD_MP"/SD");
		        }
		        else
		        {
		            printf("umount %s failed\n", dev->mount_path[index]);
		        }
			}
        }
	}
    else
    {
		char cmd[256] = {0};
        for (index = 0; index < dev->part_num; index++)
        {
			if (access(dev->mount_path[index], R_OK) == 0)
			{
				sprintf(cmd, "fuser -k %s", dev->mount_path[index]);
				system(cmd);
		        if (umount(dev->mount_path[index]) == 0)
		        {
		            remove(dev->mount_path[index]);
					if(dev->type == 2)
						usbMountFlag = 0;
		        }
		        else
		        {
		            printf("umount %s failed\n", dev->mount_path[index]);
		        }
			}
        }
		char *pTmpdir = getDir(dev->type);
		if (strcmp(pTmpdir, DISK_MP) == 0)
		{
			if (access(DISK_MP, R_OK) == 0)
			{
				char cmd[256] = {0};
				sprintf(cmd, "fuser -k %s", dev->mount_path[0]);
				printf("dev->mount_path[0] = %s\n", dev->mount_path[0]);
				system(cmd);
				umount(dev->mount_path[0]);
				remove(DISK_MP);
			}

			if (access(REC_MP, R_OK) == 0)
			{
				if (umount(REC_MP) == 0)
				{
					remove(REC_MP);
				}
			}
			recordMountStatus = 0;
			hddExitFlag = 0;
		}
		else
		{
			if (access(pTmpdir, R_OK) == 0)
			{			
				remove(pTmpdir);
			}
		}
    }

    return 0;
}

static char *getDir(int type)
{
	switch (type)
	{
		case 2:
			return USB_MEM_MP;

		case 3:
			return USB_DISK_MP;

		case 4:
			return DISK_MP;

		case 5:
			return USB_SDCARD_MP;
		default:
			break;
	}
	return NULL;
}

static int isMount(char *pParts)
{
	FILE *fd = NULL;
    int len = 0;
    char buf[1024 * 4] = {0};
    char *seek = NULL;

    fd = fopen(FILE_MOUNT_CHECK, "r");
    if (fd > 0)
    {
       //printf("[%s]open file %s success\n",__FUNCTION__, FILE_MOUNT_CHECK);
        len = fread(buf, 1, sizeof(buf), fd);
        fclose(fd);
		if (len > 0)
        {
            buf[len] = '\0';
			//printf("buf = %s\n", buf);
            seek = strstr(buf, pParts);
            if (seek != NULL)
            {
                return 0; /* This part had mounted return ok */
            }
        }
	}
    else
    {
        printf("open %s error\n", FILE_MOUNT_CHECK);
    }

    return -1;
}

static void searchSdcard()
{
	char buf_ps[80] = {0};
    char ps[50] = {0};
    FILE *ptr;
    strcpy(ps, "ls /dev | grep mmcblk");
	
    if ((ptr = popen(ps, "r")) != NULL)
    {
		int index = 0;
		SCSI_USB_DEV *new_dev = NULL;
		
        while (fgets(buf_ps, 80, ptr)!=NULL)
        {
			if (buf_ps[8] != 0)
			{
				if (new_dev == NULL)
				{
					new_dev = (SCSI_USB_DEV *)malloc(sizeof(SCSI_USB_DEV));
					if (new_dev == NULL)
					{
						printf("malloc error\n");
						return;
					}
					memset(new_dev, 0, sizeof(SCSI_USB_DEV));
					new_dev->devType = 2;
					new_dev->type = 5;
					new_dev->index = 0;
				}
				buf_ps[strlen(buf_ps) - 1] = 0;
				sprintf(new_dev->devfile_parts[index], "/dev/%s", buf_ps);
				sprintf(new_dev->mount_path[index], "%s/SD%d", USB_SDCARD_MP, index);
				index++;
				break;
			}
			memset(buf_ps, 0, 80);
        }
		if (index == 1)
		{
			new_dev->mount_path[0][strlen(new_dev->mount_path[0]) - 1] = 0;
		}
		if (new_dev != NULL)
		{
			if (sdEjectFlag == 0)
			{
				new_dev->part_num = index;
				addDev(new_dev);
			}
			else
			{
				free(new_dev);
				new_dev = NULL;
			}
		}
		else
		{
			sdEjectFlag = 0;
		}

        pclose(ptr);
        ptr = NULL;
    }
    else
    {
        printf("popen %s error\n", ps);
    }
}

static long long getDiskInfo(char *pPath)
{
	struct statfs diskInfo;
	char tmp[128] = {0};

	sprintf(tmp,"%s/",pPath);
    statfs(tmp, &diskInfo);
	//long long tempValue = (long long)diskInfo.f_bavail * (long long)diskInfo.f_bsize / 1024;
	return (long long)diskInfo.f_bavail * (long long)diskInfo.f_bsize;
}

static void getRecHddName()
{
	char ps[50] = {0};
    FILE *ptr = NULL;
    strcpy(ps, REC_BUS_PATH_CMD);
	
    if ((ptr = popen(ps, "r")) != NULL)
    {
		fgets(recHdd, 10, ptr);
		recHdd[strlen(recHdd) - 1] = 0;
		pclose(ptr);
        ptr = NULL;
	}
}

int umountDev(int devType)
{
	int ret = 0;
	if (devType == 1)
	{
		pthread_mutex_lock(&devInfolock);
		SCSI_USB_DEV *cur_dev = pOldDev;
	
		while (NULL != cur_dev)
		{
			if (cur_dev->type == 2 || cur_dev->type == 1 )
			{
				char cmd[128] = {0};
				doUmount(cur_dev);

				if (cur_dev->type == 2)
				{
					sprintf(cmd, "eject -s %s", cur_dev->devfile_parts[0]);
					cmd[strlen(cmd) - 1] = 0;
					ret = system(cmd);
					usbEjectFlag = 1;
				}
				else
				{
					char cdRompath[10] = {0};
					int index = 0;
					for(index = 0; index < 10; index++)
					{
						sprintf(cdRompath, FILE_DEV_PART_TEMPL"%d", index);
						if (access(cdRompath, R_OK) == 0)
						{
							break;
						}
					}
					sprintf(cmd, "eject -s %s", cdRompath);
					ret = system(cmd);
				
					if (ret != -1)
					{
						cdromEjectFlag = 1;
						memset(cmd, 0, sizeof(cmd));
						sprintf(cmd, "fuser -k "REC_MP);
						system(cmd);
						umount(REC_MP);
						recordMountStatus = 0;
					}
				}
				
				break;
			}
			else if (cur_dev->type == 3)
			{
				doUmount(cur_dev);
				usbHddEjectFlag = 1;
				break;
			}
		    cur_dev = cur_dev->next_dev;
		}
		
		pthread_mutex_unlock(&devInfolock);
	}
	else if (devType == 2)
	{
		pthread_mutex_lock(&devInfolock);
		SCSI_USB_DEV *cur_dev = pOldDev;
		while (NULL != cur_dev)
		{
			if (cur_dev->type == 5)
			{
				doUmount(cur_dev);
				break;
			}
			cur_dev = cur_dev->next_dev;
		}
		sdEjectFlag = 1;
		pthread_mutex_unlock(&devInfolock);
	}
	return ret;
}
#if 0
static void netLinkThread()
{
	struct sockaddr_nl client;
    struct timeval tv;
    int ntSocket, rcvlen, ret;
    fd_set fds;
    int buffersize = 1024;
	char buf[UEVENT_BUFFER_SIZE] = { 0 };

    ntSocket = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
    memset(&client, 0, sizeof(client));
    client.nl_family = AF_NETLINK;
    client.nl_pid = getpid();
    client.nl_groups = 1; /* receive broadcast message*/
    setsockopt(ntSocket, SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize));
    bind(ntSocket, (struct sockaddr*)&client, sizeof(client));

    while (stopNetLinkFlag == 0)
	{
		memset(buf, 0, UEVENT_BUFFER_SIZE);

		FD_ZERO(&fds);
		FD_SET(ntSocket, &fds);
		tv.tv_sec = 0;
		tv.tv_usec = 100 * 1000;
		ret = select(ntSocket + 1, &fds, NULL, NULL, &tv);

		if(!(ret > 0 && FD_ISSET(ntSocket, &fds)))
		{
			continue;
		}

		rcvlen = recv(ntSocket, &buf, sizeof(buf), 0);
		if (rcvlen > 0)
		{
			//printf("%s\n", buf);
			//TO DO
		}
    }

    close(ntSocket);
}
#endif
static void *mediaCheckThread(void *pPara)
{
	int retErr = 0;
	int fd = 0;
	struct cdrom_tochdr	hdr;

	while(stopMediaFlag == 0)
	{
		char cdRompath[10] = {0};
		int index = 0;
		for(index = 0; index < 10; index++)
		{
			sprintf(cdRompath, FILE_DEV_PART_TEMPL"%d", index);
			if (access(cdRompath, R_OK) == 0)
			{
				break;
			}
		}
		if (index == 10)
		{
			usleep(2000 * 1000);
			continue;
		}
		fd = open(cdRompath, O_RDONLY | O_NONBLOCK);
		if (fd == -1)
		{
			cdromMountFlag = 0;
			printf("Failed opening %s.", cdRompath);
			usleep(2000 * 1000);
			continue;
		}

		memset(&hdr, 0, sizeof(hdr));
		retErr = ioctl(fd, CDROMREADTOCHDR, &hdr);

		if (ioctl(fd, CDROM_LOCKDOOR, 0) != 0)
		{
			printf("Failed locking %s.", cdRompath);
		}
		close(fd);

		if (retErr == 0)
		{
			cdromMountFlag = 1;
		}
		else
		{
			cdromMountFlag = 0;
		}

		usleep(2000 * 1000);
	}
}

int isNtfsFromat(char *pPath)
{
	char tmpBuf[256] = {0};
	char dataBuf[256] = {0};
	FILE *ptr = NULL;
	int ret = 0;

	sprintf(tmpBuf,"fdisk -l | grep %s", pPath);
    if ((ptr = popen(tmpBuf, "r")) != NULL)
    {
        while (fgets(dataBuf, 256, ptr)!=NULL)
        {
			if (strstr(dataBuf, "NTFS") != NULL)
			{
				ret = 1;
				break;
			}
		}
		pclose(ptr);
	}

	return ret;
}

int getHddExitFlag()
{
	return hddExitFlag;
}

int getUSBMountFlag()
{
	return usbExistFlag;
}

void setUSBEjectFlag(int ejectFlag)
{
	usbEjectFlag = ejectFlag;
}

void setCdromEjectFlag(int ejectFlag)
{
	cdromEjectFlag = ejectFlag;
}
