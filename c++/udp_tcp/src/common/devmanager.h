#ifndef _DEV_MANAGER_H_
#define _DEV_MANAGER_H_

#define MAX_NAME_LEN                250
#define MAX_PART_NUM                6   // max part num
#define MAX_DISK_NUM				2   // max disk num
#pragma pack(1)
typedef struct s_scsi_usb_dev
{
	int devType;									 /* 1 usb device, 2 sd card ,3 HDD */
    int type;                                        /* 1 cdrom 2 usb memory 3 usb hdd 4 hdd 5 sd_card */
    int index;                                       /*like host0 host1*/
	int part_num;                                    /* part number */
	long long freeDiskSize[MAX_PART_NUM];			 /* free size */
    char devfile_parts[MAX_PART_NUM][MAX_NAME_LEN];  /* part path */
    char mount_path[MAX_PART_NUM][MAX_NAME_LEN];     /* mount path */
    struct s_scsi_usb_dev *next_dev;                 /* next device */
} SCSI_USB_DEV;
#pragma pack()
int startGetDevInfo();
SCSI_USB_DEV *getDevInfo();
SCSI_USB_DEV *getUSBDevInfo();
void stopGetDevInfo();
int umountDev(int devType);
int getHddExitFlag();
int getUSBMountFlag();
void setUSBEjectFlag(int ejectFlag);
void setCdromEjectFlag(int ejectFlag);
#endif //_DEV_MANAGER_H_
