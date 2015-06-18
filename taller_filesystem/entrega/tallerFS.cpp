#include <iostream>

#include "hdd.h"
#include "mbr.h"
#include "pentry.h"
#include "ext2fs.h"

using namespace std;

int main(int argc, char ** argv)
{
	HDD hdd(argv[1]);

	Ext2FS * fs = new Ext2FS(hdd, 1);

	fd_t file = fs->open("/grupos/g9/nota.txt", "r");

	int pos = fs->seek(file, 13999);
	printf("Seek pos:%d\n", pos);

	unsigned char buffer[17];
	int leidos = fs->read(file, buffer, 17);
	printf("Leidos: %d\n", leidos);

	printf("Texto: %s\n", buffer);

	fs->close(file);

	return 0;
}