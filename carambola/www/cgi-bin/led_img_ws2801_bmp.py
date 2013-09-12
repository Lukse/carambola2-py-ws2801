#!/usr/bin/python

import os
#import subprocess
#import gpio
import rgb_led_strip as rgb
import time
import random
#import cPickle
import sys
import struct

f = open("/www/rgb_strip.conf", "r")
filename =  '/www/pictures/'+f.read()
print filename
f.close()

gamma = 0.5


header_fmt = '<2sihhiiiihhiiiiii'
pixel_fmt = '=BBB'

def gamma_correction(value, gamma):
  return int((255.0 * ((value / 255.0) ** (1.0 / gamma)))+0.5)

rgb.mmio_write(0x18040000+0x0C, 1<<20)

print 'Reading BMP image...'

f = open(filename, 'rb')
s_header = f.read(54)
header = struct.unpack(header_fmt, s_header)

#print header

width = header[6]
height = header[7]
print width, height

print header

row_size = ((24*width+31)/32)*4
print 'Row size:', row_size
padding = row_size - width*3

#pixel_header = f.read(1) # dummy read
#pixels_offset = 55
pixels = []
i = 0
for y in range(0, height):
        for y in range(0, width):
                pixel_header = f.read(3)
                pixel = struct.unpack(pixel_fmt, pixel_header)
                pixels.append(pixel)
        pixel_header = f.read(padding) # dummy read
f.close()






#if len(sys.argv) == 3:
#print 'Adjusting gamma...'
#print len(pixels)
#for i in range (0, len(pixels)):
#       (r, g, b) = pixels[i]
#       r = gamma_correction(r, gamma)
#       g = gamma_correction(g, gamma)
#       b = gamma_correction(b, gamma)
#       pixels[i] = (r, g, b)
#       rgb.mmio_write(0x18040000+0x10, 1<<20)
#       time.sleep(1)
#       rgb.mmio_write(0x18040000+0x0C, 1<<20)

print 'Waiting for button...'
while((rgb.mmio_read(0x18040000+0x04) & 1<<21) ==0 ):
  rgb.mmio_write(0x18040000+0x0C, 1<<20)
  time.sleep(0.05)
  rgb.mmio_write(0x18040000+0x10, 1<<20)
  time.sleep(0.05)

rgb.mmio_write(0x18040000+0x10, 1<<20)

print 'Painting...'

buffer = []

def update_pixels(buffer):
  a = rgb.intArray(len(buffer))
  for i in xrange(len(buffer)):
    a[i] = buffer[i]
  rgb.ws2801_send_buffer(a, len(buffer))

rgb.init()


# set all black leds
for i in range(0, height):
  buffer.append(0x0)
  buffer.append(0x0)
  buffer.append(0x0)
update_pixels(buffer)

for x in range(0, width):
  for y in range(0, height):
  #for y in range(0, 55):
    #cnt = (height-y-1)*width + x
    cnt = y*width + x
    #print cnt, pixels[cnt]
    (b, g, r) = pixels[cnt]
    buffer[y*3+0] = int(r)
    buffer[y*3+1] = int(g)
    buffer[y*3+2] = int(b)
  update_pixels(buffer)


for i in range(0, height*3):
  buffer[i] = 0
update_pixels(buffer)

print 'Done'
rgb.mmio_write(0x18040000+0x0C, 1<<20)
time.sleep(0.5)
rgb.mmio_write(0x18040000+0x10, 1<<20)

