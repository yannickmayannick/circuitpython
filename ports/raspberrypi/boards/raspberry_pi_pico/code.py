import time
import usb_hid
from adafruit_hid.mouse import Mouse

mouse = Mouse(usb_hid.devices)

shift = 2
wait = 0.25

while True:
  mouse.move(x=shift, y=shift)
  time.sleep(wait)
  mouse.move(x=shift, y=-shift)
  time.sleep(wait)
  mouse.move(x=-shift, y=-shift)
  time.sleep(wait)
  mouse.move(x=-shift, y=shift)
  time.sleep(wait)