%module rgb_led_strip

%include "carrays.i"
%array_class(int, intArray);

unsigned long mmio_read(unsigned long iobase);
unsigned long mmio_write(unsigned long iobase, unsigned long data2);
void * mmiof_init(unsigned long iobase);
unsigned long mmiof_read(void * iomem, unsigned long offset);
void mmiof_write(void * iomem, unsigned long offset, unsigned long value);
void mmiof_close(void * iomem);

void init(void);

void ws2801_send_buffer(int *buffer, unsigned int len);
void ws2812_send_buffer(int *buffer, unsigned int len);

void ws2812_send_bit(unsigned char bit);
