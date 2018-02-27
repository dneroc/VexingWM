#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <algorithm>
#include <iostream>

using namespace std;

//Max function for setting minimum window size
#define MAX(a, b) ((a) > (b) ? (a) : (b))

Display * disp;			//main display
XWindowAttributes attr;	//attributes of a window
XWindowAttributes titleAttr; //titlebar attributes
XButtonEvent start; 	//save pointers state at the beginning
XEvent ev;				//event variable
Window title;			//Titlebar variable
Window frame; 			//Reparent variable
Window parent;			//Parent for titlebar move
Window test;			//Test window for closing

::std::unordered_map<Window, Window> frames;


//EventMasks, only sends events of this type
void setMasks(){

	//Alt + F1
    XGrabKey(disp, XKeysymToKeycode(disp, XK_F1), 
	Mod1Mask, DefaultRootWindow(disp), 
	True, GrabModeAsync, GrabModeAsync);

	//Alt + F2
	XGrabKey(disp, XKeysymToKeycode(disp, XK_F2), 
	Mod1Mask, DefaultRootWindow(disp), 
	True, GrabModeAsync, GrabModeAsync);

	//Alt + F4
	XGrabKey(disp, XKeysymToKeycode(disp, XK_F4), 
	Mod1Mask, DefaultRootWindow(disp), 
	True, GrabModeAsync, GrabModeAsync);
	
	//Alt + Tab
	XGrabKey(disp, XKeysymToKeycode(disp, XK_Tab), 
	Mod1Mask, DefaultRootWindow(disp), 
	True, GrabModeAsync, GrabModeAsync);
	
	//Alt + Esc
	XGrabKey(disp, XKeysymToKeycode(disp, XK_Escape), 
	Mod1Mask, DefaultRootWindow(disp), 
	True, GrabModeAsync, GrabModeAsync);

	//Alt + Right mouse click
    XGrabButton(disp, 3, Mod1Mask, DefaultRootWindow(disp), 
	True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
 	GrabModeAsync, GrabModeAsync, None, None);

	//Events for reparenting 
	XSelectInput(disp, DefaultRootWindow(disp),
	SubstructureRedirectMask | SubstructureNotifyMask);
}

void handleButtRelease(XButtonReleasedEvent ev){
	//reset start
	cout << "Button release" << endl;
	if(ev.type == ButtonRelease)
    	start.subwindow = None;
}

void reparentWindow(Window window){
	//Attributes of origial window
	XGetWindowAttributes(disp, window, &attr);
	
	//TODO: Fix title bar only working for one window (maybe reset which title bar it is?)
	//TODO: add buttons to title bar
	int borderWidth = 3;
	int colour = 0xFFF000;
	int background = 0xFFFFFF;
	int titleColour = 0x000FFF;	

	//Setting parent window charechtaristics, last 3: border width, colour and background
	frame = XCreateSimpleWindow(disp, DefaultRootWindow(disp), attr.x, attr.y, attr.width, attr.height + 20, borderWidth, colour, background);
	
	//Save set for if the window manager crashes, the reparented window survives
	XAddToSaveSet(disp, window);

	//Reparents child window
	XReparentWindow(disp, window, frame, 0, 20);

	//Displays parent window(frame)
	XMapWindow(disp, frame);
	
	//Title bar
	title = XCreateSimpleWindow(disp, frame, attr.x, attr.y,
	attr.width, 20, 0, 0xFFF000, titleColour);
	XMapWindow(disp, title);

	//Click on the title bar event
	XGrabButton(disp, 1, AnyModifier, title, 
	False, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
	GrabModeAsync, GrabModeAsync, None, None);
}

void handleMapRequest(XMapRequestEvent ev){

	reparentWindow(ev.window);
	cout << "Window reparented" << endl;

	//Remaps the child window
	XMapWindow(disp, ev.window);
	cout << "Frame mapped" << endl;
}

//Fixes xterm being tiny when reparented
//TODO: Double check this comment: //Causes error when window is moved to fast
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
	cout << "Window configured. ID:" << ev.window << endl;
}

void handleMotion(XMotionEvent ev) {
	cout << "Motion Event" << endl;
	//TODO: Handle resizing to include the inner window
	//TODO: Fix only one title bar being selected
	int xdiff = ev.x_root - start.x_root;

	cout << "Xdiff: " << xdiff << endl;

    int ydiff = ev.y_root - start.y_root;

	cout << "Ydiff: " << ydiff << endl;

	//Move window by dragging title bar
	if(start.window == title){
		cout << "Start window move" << endl;
        XMoveWindow(disp, parent,
        attr.x + (start.button==1 ? xdiff : 0),
        attr.y + (start.button==1 ? ydiff : 0));
		cout << "End window move" << xdiff << ydiff << endl;
	}
	
	//Resize
	if(start.subwindow != None){
		XResizeWindow(disp, ev.subwindow,
        MAX(100, attr.width + (start.button==3 ? xdiff : 0)),
        MAX(100, attr.height + (start.button==3 ? ydiff : 0)));
		cout << "Window resized" << endl;
		
    }
}

void handleButton(XButtonEvent ev) {
	//TODO:(bug) Resizing can cause a XConfigureWindow error and crash
	cout << "Button press event" << endl;
	
	//Fixes titles not being reset, allows selection and movement
	title = ev.window;

	//For pressing on the title bar
	if(ev.window == title){
		
		Window root, *child = NULL;
		unsigned int nchild;
		XQueryTree(disp, ev.window, &root, &parent, &child, &nchild);
		XGetWindowAttributes(disp, parent, &attr);
		XRaiseWindow(disp, parent);
		start = ev;
		cout << "Button 1 title press" << endl;
		title = ev.window;
	}	

	//Sets the start of the pointer for moving it
	else if(ev.subwindow != None){
        XGetWindowAttributes(disp, ev.subwindow, &attr);
		XRaiseWindow(disp, ev.subwindow);
		start = ev;
		cout << "Button 3 + Alt press" << endl;
    }
}

void handleKey(XKeyEvent ev) {
	//Alt + Tab creates new xclock
	if(ev.keycode == XKeysymToKeycode(disp,XK_Tab)){
		system("xclock &");
	}

	//Alt + Escape closes window Manager
	else if(ev.keycode == XKeysymToKeycode(disp,XK_Escape)){
	        XCloseDisplay(disp);	
	}
	
    //Alt+F4 closes window
	//TODO:(bug) KillClient kills the whole display
	else if(ev.keycode == XKeysymToKeycode(disp,XK_F4)){
            XKillClient(disp, ev.window);
            //TODO: Check if this causes the crash: return;
    }
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

void handleUnmapNotify(Window window) {

    Window frame = openClients[window];
    XUnmapWindow(disp, frame);
    XReparentWindow(disp, window, DefaultRootWindow(disp), 0, 0);
    XRemoveFromSaveSet(disp, window);
    XDestroyWindow(disp, frame);
    frames.erase(window);

}

void handleCreateNotify(XCreateWindowEvent ev) {}

void handleDestroyNotify(XDestroyWindowEvent ev) {}

void handleReparentNotify(XReparentEvent ev) {}

void handleMapNotify(XMapEvent ev) {}

//Event loop for intercepting different types of events
void eventLoop()
{
	XNextEvent(disp, &ev);
	
	switch(ev.type){
		case KeyPress:
			handleKey(ev.xkey);	break;

		case ButtonPress:
			handleButton(ev.xbutton); break;
			
		case ButtonRelease:		
			handleButtRelease(ev.xbutton); break;

		case MotionNotify: 		
			handleMotion(ev.xmotion); break;

		case MapRequest:		
			handleMapRequest(ev.xmaprequest); break;

		case ConfigureRequest:	
			handleConfigRequest(ev.xconfigurerequest); break;

        case UnmapNotify:       
			handleUnmapNotify(ev.xunmap.window); break;

        case CreateNotify:      
			handleCreateNotify(ev.xcreatewindow); break;

        case DestroyNotify:             
			handleDestroyNotify(ev.xdestroywindow); break;

        case ReparentNotify:            
			handleReparentNotify(ev.xreparent); break;

        case MapNotify:                 
			handleMapNotify(ev.xmap); break;
	}
}


//Useless calls


int main(void) {

	//fail if can't connect
    if(!(disp = XOpenDisplay(0x0))) return 1;
	setMasks();
    start.subwindow = None;
    test = XCreateSimpleWindow(disp, DefaultRootWindow(disp), 20, 20, 200, 200, 10, 0xFFFFFF, 0x696969);
    XMapWindow(disp,test);	
    while(True){
		//basic X event loop		
		eventLoop();
    }
} 

