#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdlib.h>

//Max function for setting minimum window size
#define MAX(a, b) ((a) > (b) ? (a) : (b))

Display * disp;			//main display
XWindowAttributes attr;	//attributes of a window
XButtonEvent start; 	//save pointers state at the beginning
XEvent ev;				//event variable
::std::unordered_map<Window, Window> openClients;


//EventMasks, only sends events of this type
void setMasks(){

	//Send events when this combination of buttons/keys/modifiers is pressed
	
	//Alt + F1
    XGrabKey(disp, XKeysymToKeycode(disp, XK_F1), Mod1Mask, DefaultRootWindow(disp), True, GrabModeAsync, GrabModeAsync);

	//Alt + F2
	XGrabKey(disp, XKeysymToKeycode(disp, XK_F2), Mod1Mask, DefaultRootWindow(disp), True, GrabModeAsync, GrabModeAsync);

	//Alt + F4
	XGrabKey(disp, XKeysymToKeycode(disp, XK_F4), Mod1Mask, DefaultRootWindow(disp), True, GrabModeAsync, GrabModeAsync);
	
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

	//Events for reparenting 
	XSelectInput(disp, DefaultRootWindow(disp), SubstructureRedirectMask |SubstructureNotifyMask);
}

void handleButtRelease(XButtonReleasedEvent ev){
	//reset start
	if(ev.type == ButtonRelease)
    	start.subwindow = None;
}

void reparentWindow(Window window){
	//Attributes of origial window
	XGetWindowAttributes(disp, window, &attr);

	//Setting parent window charechtaristics, last 3: border width, colour and background
	//TODO: Create variables to simply change attributes
	//TODO: Create an interactive title bar(move/resize, etc)
	//TODO: (bug) Crashes when trying to move window (XReparentWindow: BadMatch)
	Window parent = XCreateSimpleWindow(disp, DefaultRootWindow(disp), attr.x, attr.y, attr.width, attr.height, 5, 0xFFF000, 0xFFFFFF);
	
	//Save set for if the window manager crashes, the reparented window survives
	XAddToSaveSet(disp, window);

	//Reparents child window
	XReparentWindow(disp, window, parent, 0, 0);
	//Displays parent window(frame)
	XMapWindow(disp, parent);
        openClients[window] = parent;
}

void handleMapRequest(XMapRequestEvent ev){

	reparentWindow(ev.window);

	//Remaps the child window
	XMapWindow(disp, ev.window);
}

//Fixes xterm being tiny when reparented and sets a minium window size
void handleConfigRequest(XConfigureRequestEvent ev){
	
	XWindowChanges ch;
	ch.x = ev.x;
	ch.y = ev.y;
	//Set minimum width and height of new windows
	ch.width = /*MAX(100, */ev.width/*)*/;
	ch.height = /*MAX(100,*/ ev.height/*)*/;
	ch.border_width = ev.border_width;
	ch.sibling = ev.above;
	ch.stack_mode = ev.detail;

	XConfigureWindow(disp, ev.window, ev.value_mask, &ch);
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
		//Minimum size of a window
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
	else if(ev.state == Mod1Mask && ev.subwindow != None && ev.keycode == XKeysymToKeycode(disp,XK_Tab)){
		system("urxvtv &");
	}

	//Alt + F4 closes window
	//BUG: Xclock does not close fully close, just the inside, may be just window instead of subwindow
	else if(ev.state == Mod1Mask && ev.subwindow != None && ev.keycode == XKeysymToKeycode(disp,XK_F4)){
		XDestroySubwindows(disp, ev.subwindow);
	}

	//Alt + Escape quits window manager
	else if(ev.state == Mod1Mask && ev.subwindow != None && ev.keycode == XKeysymToKeycode(disp,XK_Escape)){
		XCloseDisplay(disp);
	}

 }

void hadnleUnmapNotify(Window window) {

    const Window frame = openClients[window];
    XUnmapWindow(disp, frame);
    XReParentWindow(disp, window, DefaultRootWindow(disp),0, 0);
    XRemoveFromSaveSet(disp, window);
    XDestroyWindow(disp, frame);
    openClients.erase(window);

}

//Event loop for intercepting different types of events
void eventLoop()
{
	XNextEvent(disp, &ev);
	
	switch(ev.type){
		case KeyPress:			handleKey(ev.xkey);		break;
		case ButtonPress:		handleButton(ev.xbutton);	break;
		case ButtonRelease:		handleButtRelease(ev.xbutton);	break;
		case MotionNotify: 		handleMotion(ev.xmotion);	break;
		case MapRequest:		handleMapRequest(ev.xmaprequest); break;
		case ConfigureRequest:	handleConfigRequest(ev.xconfigurerequest); break;
                case UnmapNotify:       handleUnmapNotify(ev.xunmap); break;
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

