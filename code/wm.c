#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdlib.h>

//Max function for setting minimum window size
#define MAX(a, b) ((a) > (b) ? (a) : (b))

Display * disp;			//main display
XWindowAttributes attr;	//attributes of a window
XButtonEvent start; 	//save pointers state at the beginning
XEvent ev;				//event variable

//EventMasks, only sends events of this type
//Send events when this combination of buttons/keys/modifiers is pressed
void setMasks(){
	//Alt + F1
    XGrabKey(disp, XKeysymToKeycode(disp, XK_F1), Mod1Mask, DefaultRootWindow(disp), True, GrabModeAsync, GrabModeAsync);

	//Alt + F2
	XGrabKey(disp, XKeysymToKeycode(disp, XK_F2), Mod1Mask, DefaultRootWindow(disp), True, GrabModeAsync, GrabModeAsync);
	
	//Alt + Tab
	XGrabKey(disp, XKeysymToKeycode(disp, XK_Tab), Mod1Mask, DefaultRootWindow(disp), True, GrabModeAsync, GrabModeAsync);
	
	//Alt + Esc
	XGrabKey(disp, XKeysymToKeycode(disp, XK_Escape), Mod1Mask, DefaultRootWindow(disp), True, GrabModeAsync, GrabModeAsync);

	//Alt + Left mouse click
    XGrabButton(disp, 1, Mod1Mask, DefaultRootWindow(disp), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

	//Alt + Right mouse click
    XGrabButton(disp, 3, Mod1Mask, DefaultRootWindow(disp), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
}

void handleButtRelease(XButtonReleasedEvent ev){
	//reset start
	if(ev.type == ButtonRelease)
    	start.subwindow = None;
}

void handleMotion(XMotionEvent ev)
{
	//If window moved, depending on button pressed, left click move = move, right click + move = resize using alt mod
	if(start.subwindow != None){
        int xdiff = ev.x_root - start.x_root;
        int ydiff = ev.y_root - start.y_root;
        XMoveResizeWindow(disp, start.subwindow,
        attr.x + (start.button==1 ? xdiff : 0),
        attr.y + (start.button==1 ? ydiff : 0),
        MAX(100, attr.width + (start.button==3 ? xdiff : 0)),
        MAX(100, attr.height + (start.button==3 ? ydiff : 0)));
    }
}

void handleButton(XButtonEvent ev)
{
	//Sets the start of the pointer for moving it
	if(ev.subwindow != None){
            XGetWindowAttributes(disp, ev.subwindow, &attr);
            start = ev;
        }
}


void handleKey(XKeyEvent ev)
{
	//Event to raise focused window with Alt+F1
	//TODO:Future will have to run by clicking window, when in focus

	if(ev.state == Mod1Mask && ev.subwindow != None && ev.keycode == XKeysymToKeycode(disp,XK_F1)){
        XRaiseWindow(disp, ev.subwindow);
	}
	
	//Alt + Tab creates new xclock
	if(ev.state == Mod1Mask && ev.subwindow != None && ev.keycode == XKeysymToKeycode(disp,XK_Tab)){
		system("xclock &");
	}

	//Alt + Escape quits window manager
	if(ev.state == Mod1Mask && ev.subwindow != None && ev.keycode == XKeysymToKeycode(disp,XK_Escape)){
		XCloseDisplay(disp);
	}

}

//Event loop for intercepting different types of events
void eventLoop()
{
	XNextEvent(disp, &ev);
	
	switch(ev.type){
		case KeyPress:			handleKey(ev.xkey);			
								
		case ButtonPress:		handleButton(ev.xbutton);		
								
		case ButtonRelease:		handleButtRelease(ev.xbutton);	
								
		case MotionNotify: 		handleMotion(ev.xmotion);	
	}
}


int main(void)
{

    if(!(disp = XOpenDisplay(0x0))) return 1; //fail if can't connect


	setMasks();
    start.subwindow = None;
    while(True){
		//basic X event loop
       eventLoop();

    }
} 
