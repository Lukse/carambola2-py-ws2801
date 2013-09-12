CROSS_COMPILE = ~/carambola2/staging_dir/toolchain-mips_r2_gcc-4.7-linaro_uClibc-0.9.33.2/bin/mips-openwrt-linux-

# Pretty print control stuff.
V       = @
Q       = $(V:1=)

# Pretty print stuff.
#
QCC     = $(Q:@=@echo    '     CC       '$@;)
QLD     = $(Q:@=@echo    '     LD       '$@;)
QAR     = $(Q:@=@echo    '     LD       '$@;)
QAR     = $(Q:@=@echo    '     AR       '$@;)
QAS     = $(Q:@=@echo    '     AS       '$@;)
QRANLIB = $(Q:@=@echo    '     RANLIB   '$@;)
QSTRIP  = $(Q:@=@echo    '     STRIP    '$@;)

CC      = $(QCC)$(CROSS_COMPILE)gcc
LD      = $(QLD)$(CROSS_COMPILE)gcc
AR      = $(QAR)$(CROSS_COMPILE)ar
AS      = $(QAS)$(CROSS_COMPILE)as
RANLIB  = $(QRANLIB)$(CROSS_COMPILE)ranlib
STRIP   = $(QSTRIP)$(CROSS_COMPILE)strip


OBJ = rgb_led_strip.o

CFLAGS  += -I $(STAGING_DIR)/usr/include -O1
LDFLAGS +=

all: python

python:
	swig -python rgb_led_strip.i
	$(CC) -Wall -Os -fpic -c rgb_led_strip.c rgb_led_strip_wrap.c -I/usr/include/python2.7/
	$(CC) -shared rgb_led_strip.o rgb_led_strip_wrap.o -o _rgb_led_strip.so

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(OBJ)
	$(CC) $(LDFLAGS) -o rgb_led_strip $(OBJ)

clean:
	rm -f *.o *.so *.py rgb_led_strip rgb_led_strip_wrap.c
	
