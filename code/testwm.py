from Xlib.display import Display
import time
from subprocess import call 
from Xlib import X, XK

dpy = Display()
root = dpy.screen().root
call("~/.fehbg",shell=True)

dpy.screen().root.grab_key(dpy.keysym_to_keycode(XK.string_to_keysym("F1")), X.Mod1Mask, 1, X.GrabModeAsync, X.GrabModeAsync)


def handle_event(event):
    if event.type == X.KeyPress: 
        dpy.close()


    

while 1:
    event = dpy.next_event()
    handle_event(event)

