#ifndef __RGB_LED_STRIP_H__
#define __RGB_LED_STRIP_H__

#include <sys/types.h>
#include <stdint.h>

struct mmio {
	unsigned long 	iobase;		/* getpagesize() aligned, see mmap(2) */
	unsigned long	offset;		/* additional offset from iobase */
	unsigned long	range;		/* N * uint32_t read/write ops. */

	void		*iomem;
	size_t		iosize;
	int			kmem;			/* 0 - /dev/mem, 1 - /dev/kmem */
};

#endif //__MMIO_H__
