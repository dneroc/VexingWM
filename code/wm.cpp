#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <algorithm>


using ::std::max;
using ::std::mutex;
using ::std::string;
using ::std::unique_ptr;


//Max function for setting minimum window size
#define MAX(a, b) ((a) > (b) ? (a) : (b))

Display * disp;			//main display
XWindowAttributes attr;	//attributes of a window
XWindowAttributes titleAttr; //titlebar attributes
XButtonEvent start; 	//save pointers state at the beginning
XEvent ev;				//event variable
Window title;			//Titlebar variable
Window parent; 			//Reparent variable


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

	//TODO: Create an interactive title bar(move/resize, etc)
	int borderWidth = 3;
	int colour = 0xFFF000;
	int background = 0xFFFFFF;

	//Setting parent window charechtaristics, last 3: border width, colour and background
	parent = XCreateSimpleWindow(disp, DefaultRootWindow(disp), attr.x, attr.y, attr.width, attr.height + 20, borderWidth, colour, background);
	
	//Save set for if the window manager crashes, the reparented window survives
	XAddToSaveSet(disp, window);

	//Reparents child window
	XReparentWindow(disp, window, parent, 0, 20);
	//Displays parent window(frame)
	XMapWindow(disp, parent);
	openClients[window] = parent;

	int titleColour = 0x000FFF;	

	//Title bar
	//TODO: change to root display so I can grab indipendantly, move along with the rest of the windows
	title = XCreateSimpleWindow(disp, parent, attr.x, attr.y, attr.width, 20, 0, 0xFFF000, titleColour);

	XMapWindow(disp, title);
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
	//Can set minimum width and height of new windows
	ch.width = ev.width;
	ch.height = ev.height;
	ch.border_width = ev.border_width;
	ch.sibling = ev.above;
	ch.stack_mode = ev.detail;

	XConfigureWindow(disp, ev.window, ev.value_mask, &ch);
}

void handleMotion(XMotionEvent ev) {
	//TODO: Handle resizing to include the inner window
	//If window moved, depending on button pressed, left click move = move, right click + move = resize using alt mod
	if(start.subwindow != None){
        int xdiff = ev.x_root - start.x_root;
        int ydiff = ev.y_root - start.y_root;

        XMoveWindow(disp, start.subwindow,
        attr.x + (start.button==1 ? xdiff : 0),
        attr.y + (start.button==1 ? ydiff : 0));

		XResizeWindow(disp, ev.subwindow,
        MAX(100, attr.width + (start.button==3 ? xdiff : 0)),
        MAX(100, attr.height + (start.button==3 ? ydiff : 0)));

		
    }
}

void handleButton(XButtonEvent ev) {
	//Sets the start of the pointer for moving it
	if(ev.subwindow != None){
            XGetWindowAttributes(disp, ev.subwindow, &attr);
			XGetWindowAttributes(disp, title, &titleAttr);
            start = ev;
			XRaiseWindow(disp, ev.subwindow);
        }
}


void handleKey(XKeyEvent ev) {
	//Event to raise focused window with Alt+F1
	//TODO:Future will have to run by clicking window, when in focus

	if(ev.state == Mod1Mask && ev.subwindow != None && ev.keycode == XKeysymToKeycode(disp,XK_F1)){
        XRaiseWindow(disp, ev.subwindow);
	}
	
	//Alt + Tab creates new xclock
	else if(ev.state == Mod1Mask && ev.subwindow != None && ev.keycode == XKeysymToKeycode(disp,XK_Tab)){
		system("xterm &");
	}

	//Alt + Escape closes window Manager
	else if(ev.state == Mod1Mask && ev.keycode == XKeysymToKeycode(disp,XK_Escape)){
	        XCloseDisplay(disp);	
	}


        //Alt+F4 closes window
	else if(ev.state == Mod1Mask && ev.keycode == XKeysymToKeycode(disp,XK_F4)){
            XKillClient(disp, ev.window);
            return;
        }
/*
                Atom* supported_protocols;
                int num_supported_protocols;
                b
                if(XGetWMProtocols(disp, ev.window,&supported_protocols,&num_supported_protocols) && (::std::find(supported_protocols,supported_protocols + num_supported_protocols,XInternAtom(disp, "WM_DELETE_WINDOW", false)) != supported_protocols + num_supported_protocols)) {
                    XEvent msg;
                    memset(&msg, 0, sizeof(msg));
                    msg.xclient.type = ClientMessage;
                    msg.xclient.message_type = XInternAtom(disp, "WM_PROTOCOLS", false);
                    msg.xclient.window = ev.window;
                    msg.xclient.format = 32;
                    msg.xclient.data.l[0] = XInternAtom(disp, "WM_DELETE_WINDOW", false);

                    XSendEvent(disp, ev.window, false, 0, &msg);
                }
                else {
                    XKillClient(disp, ev.window);
               }
	}
*/
 
 }


void handleUnmapNotify(Window window) {

    const Window frame = openClients[window];
    XUnmapWindow(disp, frame);
    XReparentWindow(disp, window, DefaultRootWindow(disp),0, 0);
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
                case UnmapNotify:       handleUnmapNotify(ev.xunmap.window); break;
	}
}


int main(void) {

    if(!(disp = XOpenDisplay(0x0))) return 1; //fail if can't connect
	setMasks();
    start.subwindow = None;
    while(True){
		//basic X event loop
       eventLoop();

    }
} 

