/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or rearrange any code or data as you need.
The header files page_table.h and disk.h explain
how to use the page table and disk interfaces.
*/

#include "page_table.h"
#include "disk.h"
#include "program.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct disk *mydisk;
struct page_table *mypt;
char * myphysmem;



void page_fault_handler( struct page_table *pt, int page )
{
	int frameNo, why;
	page_table_get_entry( pt, page, &frameNo, &why );
	if (!why) {
		printf("Page %d not in memory. Now will read it into memory.\n", page);
		page_table_set_entry(pt, page, page, PROT_READ);
		disk_read (mydisk, page, &myphysmem[page*PAGE_SIZE]);
	} else if (!(why & PROT_WRITE)) {
		printf("Clean page %d is in frame %d in memory. Will write into it next.\n", page, frameNo);
		page_table_set_entry(pt, page, page, PROT_READ|PROT_WRITE);
	} else printf ("Some unexplained problem!\n");

	/*
	printf("page fault on page #%d\n",page);
	exit(1);
	*/
}

int main( int argc, char *argv[] )
{
	if(argc!=5) {
		printf("use: virtmem <npages> <nframes> <rand|fifo|custom> <sort|scan|focus>\n");
		return 1;
	}

	int npages = atoi(argv[1]);
	int nframes = atoi(argv[2]);
	const char *program = argv[4];

	struct disk *disk = disk_open("myvirtualdisk",npages);
	if(!disk) {
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}
	mydisk = disk;


	struct page_table *pt = page_table_create( npages, nframes, page_fault_handler );
	if(!pt) {
		fprintf(stderr,"couldn't create page table: %s\n",strerror(errno));
		return 1;
	}
	mypt = pt;

	char *virtmem = page_table_get_virtmem(pt);

 	char *physmem = page_table_get_physmem(pt);
 	myphysmem = physmem;

	if(!strcmp(program,"sort")) {
		sort_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"scan")) {
		scan_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"focus")) {
		focus_program(virtmem,npages*PAGE_SIZE);

	} else {
		fprintf(stderr,"unknown program: %s\n",argv[3]);

	}
	
	printf("\n Page Table as at program end\n");
	page_table_print(pt);
	page_table_delete(pt);
	disk_close(disk);

	return 0;
}
