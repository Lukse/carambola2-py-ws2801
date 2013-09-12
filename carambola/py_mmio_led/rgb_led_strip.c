#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include "rgb_led_strip.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>


static inline uint32_t readl(void *ptr)
{
	uint32_t *data = ptr;
	return *data;
}

static inline void writel(uint32_t value, void *ptr)
{
	uint32_t *data = ptr;
	*data = value;
}

uint32_t mmio_readl(const struct mmio *io, unsigned int offset)
{
	void *addr;

	addr = io->iomem + io->offset + offset;
	return readl(addr);
}

void mmio_writel(const struct mmio *io, unsigned int offset, uint32_t value)
{
	void *addr;

	addr = io->iomem + io->offset + offset;
	writel(value, addr);
}

static void mmio_normalize(struct mmio *mo)
{
	int npages = 0;

	mo->iobase += mo->offset;
	mo->offset = mo->iobase & (getpagesize() - 1);
	mo->iobase = mo->iobase & ~(getpagesize() - 1);

	npages += (mo->range * sizeof(uint32_t)) / getpagesize();
	npages += 1;
	mo->iosize = npages * getpagesize();
}

static void mmio_init(struct mmio *mo)
{
	char *device;
	int iofd;

	if (!mo->kmem)
		device = "/dev/mem";
	else
		device = "/dev/kmem";

	iofd = open(device, O_RDWR);
	if (iofd < 0)
	{
		//ie_errno("open() failed");
		printf("open failed\n");
		exit(-1);
		// TODO:
	}

	mo->iomem = mmap(NULL, mo->iosize,
			 PROT_READ|PROT_WRITE,
			 MAP_SHARED,
			 iofd, mo->iobase);

	if (mo->iomem == MAP_FAILED)
	{
		printf("can't map\n");
		exit(-1);
		//die_errno("can't map @ %0lX", mo->iobase);
	}

	close(iofd);
}

int mmio_map(struct mmio *io, unsigned long base, size_t length)
{
	memset(io, 0, sizeof(*io));

	io->iobase = base;
	io->offset = 0;
	io->range  = length;

	mmio_normalize(io);
	mmio_init(io);

	return 0;
}

void mmio_unmap(struct mmio *io)
{
	if (munmap(io->iomem, io->iosize))
	{
		exit(-1);
		//die_errno("can't unmap @ %lX", io->iobase);
	}

	memset(io, 0, sizeof(*io));
}

/*
void mmio_hexdump(const struct mmio *io, size_t length, size_t flags)
{
	__hexdump(io->iobase + io->offset,
		  io->iomem + io->offset,
		  length, flags);
}
*/


unsigned long mmio_read(unsigned long iobase)
{
    struct mmio io;
    uint32_t rdata;

    mmio_map(&io, iobase, 0);
    rdata = mmio_readl(&io, 0);
    mmio_unmap(&io);
    return rdata;
}

unsigned long mmio_write(unsigned long iobase, unsigned long data2)
{
    struct mmio io;
    uint32_t data;

    mmio_map(&io, iobase, 0);
    mmio_writel(&io, 0, data2);
    data = mmio_readl(&io, 0);
    mmio_unmap(&io);
    if (data != data2)
        return -1;
    else
        return 0;
}

unsigned long mmio_write2(unsigned long iobase, unsigned long data2)
{
    struct mmio io;
    uint32_t data;

    mmio_map(&io, iobase, 0);
    mmio_writel(&io, 0, data2);
    data = mmio_readl(&io, 0);
    mmio_unmap(&io);
    if (data != data2)
        return -1;
    else
       return 0;
}

/*
void led_init(void)
{
  uint32_t gpio_base = 0x18040000;
  uint32_t oe_mask = mmio_read(gpio_base);
  oe_mask = oe_mask | (1 << 18) | (1 << 19);
  mmio_write(gpio_base, oe_mask);

  mmio_write(gpio_base + 0x10, (1 << 19)); usleep(1000);
}

void send_bit(unsigned char bit)
{
  uint32_t gpio_base = 0x18040000;

  mmio_write(gpio_base + 0x10, (1 << 19)); //usleep(100);
  if(bit == 0)
  {
    mmio_write(gpio_base + 0x10, (1 << 18)); //usleep(100);
  }
  else
  {
    mmio_write(gpio_base + 0x0C, (1 << 18)); //usleep(100);
  }
  mmio_write(gpio_base + 0x0C, (1 << 19)); // usleep(100);
  mmio_write(gpio_base + 0x10, (1 << 19)); usleep(10);
}

void send_byte(unsigned char byte)
{
  unsigned int i;
  for (i = 0; i <= 7; i++)
  {
    send_bit((byte & ( 1 << (7-i))));
  }
}


void led_send_buffer(int *buffer, unsigned int len)
{
  uint32_t gpio_base = 0x18040000;
  unsigned int i;
  for(i = 0; i< len; i++)
  {
    send_byte(buffer[i]);
  }
}

void led_print_buffer(int *lst, unsigned int len)
{
  unsigned int i;
  for(i = 0; i< len; i++)
  {
    printf("%d\n", lst[i]);
  }
}
*/








void * mmiof_init(unsigned long iobase)
{
	struct 			    mmio io1;
	unsigned long 	offset;
	unsigned long 	range;
	size_t    			iosize;
	void		      	*iomem;

	range  = 0;

	int npages = 0;

	//iobase += offset;
	offset = iobase & (getpagesize() - 1);
	iobase = iobase & ~(getpagesize() - 1);

	npages += (range * sizeof(uint32_t)) / getpagesize();
	npages += 1;
	iosize = npages * getpagesize();

	//mmio_init(&io1);
	char *device;
	int iofd;

	device = "/dev/mem";
	//device = "/dev/kmem";

	iofd = open(device, O_RDWR);
	if (iofd < 0)
	{
		//ie_errno("open() failed");
		printf("open failed\n");
		exit(-1);
		// TODO:
	}

	iomem = mmap(NULL, iosize, PROT_READ|PROT_WRITE, MAP_SHARED, iofd, iobase);

	if (iomem == MAP_FAILED)
	{
		printf("can't map\n");
		exit(-1);
		//die_errno("can't map @ %0lX", mo->iobase);
	}

	close(iofd);
	return iomem;

}

unsigned long mmiof_read(void * iomem, unsigned long offset)
{
	void *addr;
	addr = iomem + offset;
	unsigned long read_data = readl(addr);
	return read_data;
}

void mmiof_write(void * iomem, unsigned long offset, unsigned long value)
{
	void *addr;
	addr = iomem + offset;
	uint32_t *data = addr;
	*data = value;
}



void mmiof_close(void * iomem)
{
	if (munmap(iomem, 4096))
	{
		exit(-1);
		//die_errno("can't unmap @ %lX", io->iobase);
	}
}









void init(void)
{
  uint32_t gpio_base = 0x18040000;
  uint32_t oe_mask = mmio_read(gpio_base);
  oe_mask = oe_mask | (1 << 18) | (1 << 19);
  mmio_write(gpio_base, oe_mask);

  mmio_write(gpio_base + 0x10, (1 << 19)); usleep(1000);
}



void ws2801_send_bit(unsigned char bit)
{
  uint32_t gpio_base = 0x18040000;
  unsigned long base = 0x18040000;
  void * iomem = mmiof_init(base);
  unsigned long read = mmiof_read(iomem, 0);
  //mmiof_write(iomem, 0, 0x00046cff);

  //mmio_write(gpio_base + 0x10, (1 << 19)); //usleep(100);
  mmiof_write(iomem, 0x10, (1<<19));
  if(bit == 0)
  {
    mmiof_write(iomem, 0x10, (1<<18));
    //mmio_write(gpio_base + 0x10, (1 << 18)); //usleep(100);
  }
  else
  {
    mmiof_write(iomem, 0x0C, (1<<18));
    //mmio_write(gpio_base + 0x0C, (1 << 18)); //usleep(100);
  }
  mmiof_write(iomem, 0x0C, (1<<19));
//  mmiof_write(iomem, 0x10, (1<<19));
  //mmio_write(gpio_base + 0x0C, (1 << 19)); // usleep(100);
  //mmio_write(gpio_base + 0x10, (1 << 19)); usleep(10);

//  mmiof_write(iomem, 0x10, 0x00042605);
  mmiof_close(iomem);

}

void ws2801_send_byte(unsigned char byte)
{
  unsigned int i;
  uint32_t gpio_base = 0x18040000;
  unsigned long base = 0x18040000;
  void * iomem = mmiof_init(base);
  unsigned long read = mmiof_read(iomem, 0);

  for (i = 0; i <= 7; i++)
  {
    //mmio_write(gpio_base + 0x10, (1 << 19)); //usleep(100);
    mmiof_write(iomem, 0x10, (1<<19));
    if((byte & ( 1 << (7-i))) == 0)
    {
      mmiof_write(iomem, 0x10, (1<<18));
      //mmio_write(gpio_base + 0x10, (1 << 18)); //usleep(100);
    }
    else
    {
      mmiof_write(iomem, 0x0C, (1<<18));
      //mmio_write(gpio_base + 0x0C, (1 << 18)); //usleep(100);
    }
    mmiof_write(iomem, 0x0C, (1<<19));
  //  mmiof_write(iomem, 0x10, (1<<19));
  //mmio_write(gpio_base + 0x0C, (1 << 19)); // usleep(100);
  //mmio_write(gpio_base + 0x10, (1 << 19)); usleep(10);
  }
 mmiof_close(iomem);
}

void ws2801_send_buffer(int *buffer, unsigned int len)
{
  uint32_t gpio_base = 0x18040000;
  unsigned int i, j;
  unsigned long base = 0x18040000;
  void * iomem = mmiof_init(base);
  unsigned long read = mmiof_read(iomem, 0);

  mmiof_write(iomem, 0x10, (1<<19));
  mmiof_write(iomem, 0x10, (1<<18));
  usleep(10);


  for(j = 0; j< len; j++)
    for (i = 0; i <= 7; i++)
    {
      if(((buffer[j]) & (1 << (7-i))) == 0)
      {
        mmiof_write(iomem, 0x10, (1<<19));
        mmiof_write(iomem, 0x10, (1<<18));
      }
      else
      {
        mmiof_write(iomem, 0x10, (1<<19));
        mmiof_write(iomem, 0x0C, (1<<18));
      }
      mmiof_write(iomem, 0x0C, (1<<19));
      mmiof_write(iomem, 0x10, (1<<19));
      mmiof_write(iomem, 0x10, (1<<19));
      mmiof_write(iomem, 0x10, (1<<19));
  }

mmiof_write(iomem, 0x10, (1<<19));
mmiof_write(iomem, 0x10, (1<<18));
mmiof_close(iomem);

}


void ws2812_send_buffer(int *buffer, unsigned int len)
{
  uint32_t gpio_base = 0x18040000;
  unsigned int i, j;
  unsigned long base = 0x18040000;
  void * iomem = mmiof_init(base);
  unsigned long read = mmiof_read(iomem, 0);

  mmiof_write(iomem, 0x10, (1<<18));
  usleep(10);

  for(j = 0; j< len; j++)
    for (i = 0; i <= 7; i++)
    {
      if(((buffer[j]) & (1 << (7-i))) == 0)
      {
        mmiof_write(iomem, 0x0C, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
      
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
      }
      else
      {
        mmiof_write(iomem, 0x0C, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
       
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x10, (1<<18));
        mmiof_write(iomem, 0x0C, (1<<18));
      }
    }
  mmiof_close(iomem);
}





void ws2812_send_bit(unsigned char bit)
{
  uint32_t gpio_base = 0x18040000;
  unsigned long base = 0x18040000;
  void * iomem = mmiof_init(base);
  unsigned long read = mmiof_read(iomem, 0);

  int z;

  mmiof_write(iomem, 0x10, (1<<18));
  usleep(50);


  for(z = 0; z<10000; z++)
  {
/*    
    mmiof_write(iomem, 0x0C, (1<<18));
    mmiof_write(iomem, 0x0C, (1<<18));
    mmiof_write(iomem, 0x0C, (1<<18));
    mmiof_write(iomem, 0x0C, (1<<18));
    mmiof_write(iomem, 0x0C, (1<<18));
  
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
*/


    mmiof_write(iomem, 0x0C, (1<<18));
    mmiof_write(iomem, 0x0C, (1<<18));
    mmiof_write(iomem, 0x0C, (1<<18));
    mmiof_write(iomem, 0x0C, (1<<18));
    mmiof_write(iomem, 0x0C, (1<<18));
    mmiof_write(iomem, 0x0C, (1<<18));
    mmiof_write(iomem, 0x0C, (1<<18));
    mmiof_write(iomem, 0x0C, (1<<18));
    mmiof_write(iomem, 0x0C, (1<<18));
    mmiof_write(iomem, 0x0C, (1<<18));
    mmiof_write(iomem, 0x0C, (1<<18));
  
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));
    mmiof_write(iomem, 0x10, (1<<18));

  }

/*
void mmiof_write(void * iomem, unsigned long offset, unsigned long value)
{
  void *addr;
  addr = iomem + offset;
  uint32_t *data = addr;
  *data = value;
}
*/


//  mmiof_write(iomem, 0x10, 0x00042605);
  mmiof_close(iomem);

}









int main(int argc, char *argv[])
{



// slow read
// =========
/*
	uint32_t read_data = mmio_read(0x18040004);
	printf("Read2: %08X\n", read_data);
*/


// slow write
// ==========
/*
	mmio_write(0x18040000, 0x00046cff);

	while(1)
	{
		mmio_write(0x1804000C, 0x00042605);
		mmio_write(0x18040010, 0x00042605);
	}
*/


// fast
// ====
	unsigned long base = 0x18040000;

	void * iomem = mmiof_init(base);
	
	unsigned long read = mmiof_read(iomem, 0);
	printf("Read: %08X\n", read);

	mmiof_write(iomem, 0, 0x00046cff);

	while(1)
	{
		mmiof_write(iomem, 0x0C, 0x00042605);
		usleep(10);
		mmiof_write(iomem, 0x10, 0x00042605);
		usleep(10);
	}

	mmiof_close(iomem);

    return 0;
}
