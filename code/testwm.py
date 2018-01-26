from Xlib.display import Display
import time
from subprocess import call 

dpy = Display()
root = dpy.screen().root
call("~/.fehbg",shell=True)
time.sleep(5)
dpy.close()

