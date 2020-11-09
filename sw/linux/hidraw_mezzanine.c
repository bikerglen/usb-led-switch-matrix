// SPDX-License-Identifier: GPL-2.0
/*
 * Hidraw Userspace Example
 *
 * Copyright (c) 2010 Alan Ott <alan@signal11.us>
 * Copyright (c) 2010 Signal 11 Software
 *
 * The code may be used by anyone for any purpose,
 * and can serve as a starting point for developing
 * applications using hidraw.
 */

/* Linux */
#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

/*
 * Ugly hack to work around failing compilation on systems that don't
 * yet populate new version of hidraw.h to userspace.
 */
#ifndef HIDIOCSFEATURE
#warning Please have your distro update the userspace kernel headers
#define HIDIOCSFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x06, len)
#define HIDIOCGFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x07, len)
#endif

/* Unix */
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

const char *bus_str(int bus);

uint16_t leds[10] = {
	0x0004, 0x0002, 0x0001, 0x0020, 0x0010,
	0x0200, 0x0400, 0x0800, 0x2000, 0x1000
};

int main(int argc, char **argv)
{
	int fd;
	int i, j, k, n, res, desc_size = 0;
	uint8_t buf[256];
	struct hidraw_report_descriptor rpt_desc;
	struct hidraw_devinfo info;
	char *device = "/dev/hidraw0";

	if (argc > 1)
		device = argv[1];

	/* Open the Device with non-blocking reads. In real life,
	   don't use a hard coded path; use libudev instead. */
	fd = open(device, O_RDWR|O_NONBLOCK);

	if (fd < 0) {
		perror("Unable to open device");
		return 1;
	}

	memset(&rpt_desc, 0x0, sizeof(rpt_desc));
	memset(&info, 0x0, sizeof(info));
	memset(buf, 0x0, sizeof(buf));

	/* Get Report Descriptor Size */
	res = ioctl(fd, HIDIOCGRDESCSIZE, &desc_size);
	if (res < 0)
		perror("HIDIOCGRDESCSIZE");
	else
		printf("Report Descriptor Size: %d\n", desc_size);

	/* Get Report Descriptor */
	rpt_desc.size = desc_size;
	res = ioctl(fd, HIDIOCGRDESC, &rpt_desc);
	if (res < 0) {
		perror("HIDIOCGRDESC");
	} else {
		printf("Report Descriptor:\n");
		for (i = 0; i < rpt_desc.size; i++)
			printf("%hhx ", rpt_desc.value[i]);
		puts("\n");
	}

	/* Get Raw Name */
	res = ioctl(fd, HIDIOCGRAWNAME(256), buf);
	if (res < 0)
		perror("HIDIOCGRAWNAME");
	else
		printf("Raw Name: %s\n", buf);

	/* Get Physical Location */
	res = ioctl(fd, HIDIOCGRAWPHYS(256), buf);
	if (res < 0)
		perror("HIDIOCGRAWPHYS");
	else
		printf("Raw Phys: %s\n", buf);

	/* Get Raw Info */
	res = ioctl(fd, HIDIOCGRAWINFO, &info);
	if (res < 0) {
		perror("HIDIOCGRAWINFO");
	} else {
		printf("Raw Info:\n");
		printf("\tbustype: %d (%s)\n",
			info.bustype, bus_str(info.bustype));
		printf("\tvendor: 0x%04hx\n", info.vendor);
		printf("\tproduct: 0x%04hx\n", info.product);
	}

#if 0
	/* Set Feature */
	buf[0] = 0x9; /* Report Number */
	buf[1] = 0xff;
	buf[2] = 0xff;
	buf[3] = 0xff;
	res = ioctl(fd, HIDIOCSFEATURE(4), buf);
	if (res < 0)
		perror("HIDIOCSFEATURE");
	else
		printf("ioctl HIDIOCSFEATURE returned: %d\n", res);

	/* Get Feature */
	buf[0] = 0x9; /* Report Number */
	res = ioctl(fd, HIDIOCGFEATURE(256), buf);
	if (res < 0) {
		perror("HIDIOCGFEATURE");
	} else {
		printf("ioctl HIDIOCGFEATURE returned: %d\n", res);
		printf("Report data (not containing the report number):\n\t");
		for (i = 0; i < res; i++)
			printf("%hhx ", buf[i]);
		puts("\n");
	}
#endif

	/* Clear all lamps */
	buf[0] = 0x04;
	buf[1] = 0x00;
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = 0x00;
	
	res = write(fd, buf, 5);
	if (res < 0) {
		printf("Error: %d\n", errno);
		perror("write");
	} else {
		printf("write() wrote %d bytes\n", res);
	}

	k = 0;

	while (1) {
		for (i = 0; i < 10; i++) { 
			buf[0] = 0x01;
			buf[1] = leds[i] >> 8;
			buf[2] = leds[i];

			res = write(fd, buf, 3);
			if (res < 0) {
				printf("Error: %d\n", errno);
				perror("write");
			} else {
				// printf("write() wrote %d bytes\n", res);
			}



/*
			buf[0] = 0x01;
			buf[1] = k;
			buf[2] = 'M';
			buf[3] = 'O';
			buf[4] = 'A';
			buf[5] = 'R';
			buf[6] = 'L';
			buf[7] = 'E';
			buf[8] = 'D';
			buf[9] = 'S';
			res = write(fd, buf, 10);



*/
			for (n = 0; n < 5; n++) {
				usleep (50000);
				res = read(fd, buf, 17);
				if (res < 0) {
					// perror("read");
				} else if (res > 0) {
					//printf("read() read %d bytes:\n\t", res);
					for (j = 0; j < res; j++) {
						printf("%02x ", buf[j] & 0xff);
					}
					printf("\n");
				}
			}


/*
			buf[0] = 0x01;
			buf[1] = k;
			buf[2] = ' ';
			buf[3] = ' ';
			buf[4] = ' ';
			buf[5] = ' ';
			buf[6] = ' ';
			buf[7] = ' ';
			buf[8] = ' ';
			buf[9] = ' ';
			res = write(fd, buf, 10);

			if (++k >= 10) {
				k = 0;
			}
*/



		}
	}

	sleep (1);

	while (1) {
		res = read(fd, buf, 17);
		if (res < 0) {
			perror("read");
		} else {
			//printf("read() read %d bytes:\n\t", res);
			for (i = 0; i < res; i++) {
				printf("%02x ", buf[i] & 0xff);
			}
			puts("\n");

		}
	}

	close(fd);
	return 0;
}

const char *
bus_str(int bus)
{
	switch (bus) {
	case BUS_USB:
		return "USB";
		break;
	case BUS_HIL:
		return "HIL";
		break;
	case BUS_BLUETOOTH:
		return "Bluetooth";
		break;
	case BUS_VIRTUAL:
		return "Virtual";
		break;
	default:
		return "Other";
		break;
	}
}

