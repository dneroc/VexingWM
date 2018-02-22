#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

//Max function for setting minimum window size
#define MAX(a, b) ((a) > (b) ? (a) : (b))

//XK_Alt set from keysym 
#define XK_Alt	0xFFE9

int main(void)
{
    Display * disp;			//main display
    XWindowAttributes attr;	//attributes of a window
    XButtonEvent start; 	//save pointers state at the beginning
    XEvent ev;				//event variable

    if(!(disp = XOpenDisplay(0x0))) return 1; //fail if can't connect

//Send events when this combination of buttons/keys/modifiers is pressed
//Alt + F1
    XGrabKey(disp, XKeysymToKeycode(disp, XK_F1), Mod1Mask, DefaultRootWindow(disp), True, GrabModeAsync, GrabModeAsync);

//Alt + F2
	XGrabKey(disp, XKeysymToKeycode(disp, XK_F2), Mod1Mask, DefaultRootWindow(disp), True, GrabModeAsync, GrabModeAsync);

//Alt + Left mouse click
    XGrabButton(disp, 1, Mod1Mask, DefaultRootWindow(disp), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

//Alt + Right mouse click
    XGrabButton(disp, 3, Mod1Mask, DefaultRootWindow(disp), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);


    start.subwindow = None;
    while(True)
    {
		//basic X event loop
		//TODO: Create an event handler funtion and run in the main function, use switch(event.type){case event}. each case calls another function to do with the event type. 
        XNextEvent(disp, &ev);

		//Event to raise focused window with Alt+F2
		//TODO: Change to F1, put in function. Future will have to run by clicking window, when in focus
        if((ev.xkey.state == Mod1Mask) && (ev.type == KeyPress && ev.xkey.subwindow != None) && (ev.xkey.keycode == XKeysymToKeycode(disp,XK_F2)))
		{
            XRaiseWindow(disp, ev.xkey.subwindow);
		}

		//Sets the start of the pointer for moving it
        else if(ev.type == ButtonPress && ev.xbutton.subwindow != None)
        {
            XGetWindowAttributes(disp, ev.xbutton.subwindow, &attr);
            start = ev.xbutton;
        }

		//If window moved, depending on button pressed, left click move = move, right click + move = resize
        else if(ev.type == MotionNotify && start.subwindow != None)
        {
            int xdiff = ev.xbutton.x_root - start.x_root;
            int ydiff = ev.xbutton.y_root - start.y_root;
            XMoveResizeWindow(disp, start.subwindow,
                attr.x + (start.button==1 ? xdiff : 0),
                attr.y + (start.button==1 ? ydiff : 0),
                MAX(100, attr.width + (start.button==3 ? xdiff : 0)),
                MAX(100, attr.height + (start.button==3 ? ydiff : 0)));
        }
		//set the start to none when pointer released
        else if(ev.type == ButtonRelease)
            start.subwindow = None;
    }
}