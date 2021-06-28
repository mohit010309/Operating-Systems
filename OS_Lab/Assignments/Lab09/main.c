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
int nframes;
int npages;
int frameTable[100]; /* Should use malloc() */
int aStart, bStart, bEnd, cStart, dStart, ptr=0;
int reads =0, faults =0, writes = 0, xfers = 0;
char *policy;
static int assignFrame(struct page_table *pt);

void page_fault_handler( struct page_table *pt, int page )
{
	int frameNo, bits, victim;
	page_table_get_entry( pt, page, &frameNo, &bits );
	if (!bits) { /* Page not in memory */
		faults++;
		victim = assignFrame(pt);
/* 12 lines deleted */
	} else if (!(bits & PROT_READ)) { /* Page in memory perhaps without READ permission */
		faults++;
/* A line deleted */
	} else if (!(bits & PROT_WRITE)) { /* Page in memory perhaps without WRITE permission */
		faults++;
/* A line deleted */
	} else printf ("Some unexplained problem!\n");

	/*
	printf("page fault on page #%d\n",page);
	exit(1);
	*/
}


static int assignFrame(struct page_table *pt) {
	int allocated = ptr;
	int bits, frame;
	if (frameTable[ptr] == -1) 
		ptr = (ptr + 1)%nframes;
	else if(!strcmp(policy,"fifo"))
		ptr = (ptr + 1)%nframes;
	else if (!strcmp(policy,"rand"))
		ptr = lrand48()%nframes;
	else { /* Custom */
		page_table_get_entry (pt, frameTable[ptr], &frame, &bits);
		if ((bits & PROT_READ) && (bits & PROT_WRITE)) {
			/* Is active for both read and write -- retain it for now */
/* A line deleted -- Clear PROT_READ bit for page */
			ptr = (ptr + 1)%nframes;
			allocated = ptr;
		} 
		ptr = (ptr + 1)%nframes;
	}
	return allocated;
}

int main( int argc, char *argv[] )
{
	if(argc!=5) {
		printf("use: virtmem <npages> <nframes> <rand|fifo|custom> <sort|scan|focus>\n");
		return 1;
	}

	npages = atoi(argv[1]);
	nframes = atoi(argv[2]);
	policy = argv[3];
	const char *program = argv[4];
	
	if (!strcmp(policy,"rand"))
		ptr = lrand48()%nframes;
	else if (!strcmp(policy,"custom"))
		ptr = 0;
	else ptr = 0;
	
	for (int f = 0; f<nframes; f++)
		frameTable[f] = -1;

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
//	page_table_print(pt);
	page_table_delete(pt);
	printf("Faults: %d\n  Reads: %d\n  Writes: %d\n", faults, reads, writes);
	printf("Total Transfers = %d\n",  xfers);
	disk_close(disk);

	return 0;
}
