// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}

uint64
count_all(void)
{
	/*
	uint64 n = 0, count = 1000;
	uint64 *start = 0, *end = 0, *end_t = 0;
	start = kalloc();
	printf("PGSIZE%d\n", PGSIZE);
	//if(start)
	//	n = 4096;
	do{
		end_t = end;
		if(start)
			n += 4096;
		end = kalloc();
	}while(count--);
	//freerange(start, end_t);
	if(start != 0 && end != 0)
		for(; start <= end; end -= 512)
			kfree(end);
	printf("MEM:%d,start:0x%x,end:0x%x\n", n, start, end_t);
	return n;
	*/
	struct run *r;
	uint64 n = 4096;
	acquire(&kmem.lock);
	r = kmem.freelist;
	if(!r)
	{
		release(&kmem.lock);
		printf("MEM:0\n");
		return 0;
	}
	do
	{
		n += PGSIZE;
		r = r->next;
	}while(r->next);
	release(&kmem.lock);
	printf("MEM:%d\n", n);
	return n;	
}
