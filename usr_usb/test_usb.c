#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

static void memdump(char *buf, int size)
{
#define LINE_SZ	16
	int len, i;
	while (size) {
		len = (size > LINE_SZ) ? LINE_SZ : size;
		for (i = 0; i < len; i++)
			fprintf(stdout, " %02x", (unsigned char)buf[i]);
		for (; i < LINE_SZ; i++)
			fprintf(stdout, "   ");
		fprintf(stdout, " : ");
		for (i = 0; i < len; i++) {
			if ((buf[i] >= '0') && (buf[i] <= 'z'))
				fprintf(stdout, "%c", buf[i]);
			else
				fprintf(stdout, ".");
		}
		fprintf(stdout, "\n");
		size -= len;
		buf  += len;
	}
}

#define DEVICE	"/dev/skel0"
#define BLK_SZ	512
#define CBW_SZ	31
#define CSW_SZ	13

/* CBW with SCSI READ(10), read 1 block from block 0 */
static char cbw_read10[CBW_SZ] = {
	0x55, 0x53, 0x42, 0x43, 0x74, 0x65, 0x73, 0x74,
	0x00, 0x02, 0x00, 0x00, 0x80, 0x00, 0x0A,
	0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

int main(void)
{
	char buf[BLK_SZ], *pbuf;
	int fd, ret, done = 0;

	fd = open(DEVICE, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "Failed to open '%s': %s\n", DEVICE, strerror(errno));
		return -1;
	}
	fprintf(stdout, "%s opened\n", DEVICE);

	/* send CBW with READ(10) command */
	memcpy(buf, cbw_read10, CBW_SZ);
	if (write(fd, buf, CBW_SZ) != CBW_SZ) {
		fprintf(stderr, "Failed to send CBW: %s\n", strerror(errno));
		close(fd);
		return -1;
	}
	fprintf(stdout, "Sent CBW with READ(10):\n");
	memdump(buf, CBW_SZ);

	/* read block 0 */
	pbuf = buf;
	while (done < BLK_SZ) {
		ret = read(fd, pbuf, (BLK_SZ - done));
		if (ret <= 0) {
			fprintf(stderr, "Failed to read block 0 (done %d): %s\n",
				done, strerror(errno));
			close(fd);
			return -1;
		}
		done += ret;
		pbuf += ret;
	}
	fprintf(stdout, "Read block 0:\n");
	memdump(buf, BLK_SZ);

	/* get CSW */
	if (read(fd, buf, CSW_SZ) != CSW_SZ) {
		fprintf(stderr, "Failed to get CSW: %s\n", strerror(errno));
		close(fd);
		return -1;
	}
	fprintf(stdout, "Got CSW:\n");
	memdump(buf, CSW_SZ);

	close(fd);
	return 0;

}
