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
Window client;			//For resizing the client
Window parent;			//Parent for titlebar move
Window test;			//Test window for closing
Window exitButton;		//Exit button at the top

::std::unordered_map<Window, Window> frames;

//Gets the parent of a window, helpful for dealing with window frames
void getParent(Window window){
	cout << "Get frame start" << endl;
	Window root, *child = NULL;
	unsigned int nchild;
	XQueryTree(disp, window, &root, &parent, &child, &nchild);
}

//EventMasks, only sends events of this type on the root window(anywhere)
void setMasks(){
	
	//Alt + F1, nothing for now
    //XGrabKey(disp, XKeysymToKeycode(disp, XK_F1), 
	//Mod1Mask, DefaultRootWindow(disp), 
	//True, GrabModeAsync, GrabModeAsync);

	//Alt + F2, nothing for now
	//XGrabKey(disp, XKeysymToKeycode(disp, XK_F2), 
	//Mod1Mask, DefaultRootWindow(disp), 
	//True, GrabModeAsync, GrabModeAsync);
	
	//Alt + Tab(for creating new windows)
	//TODO: Set for window switching
	XGrabKey(disp, XKeysymToKeycode(disp, XK_Tab), 
	Mod1Mask, DefaultRootWindow(disp), 
	True, GrabModeAsync, GrabModeAsync);
	
	//Alt + Esc(for escaping the client)
	XGrabKey(disp, XKeysymToKeycode(disp, XK_Escape), 
	Mod1Mask, DefaultRootWindow(disp), 
	True, GrabModeAsync, GrabModeAsync);

	//Events for reparenting 
	XSelectInput(disp, DefaultRootWindow(disp),
	SubstructureRedirectMask | SubstructureNotifyMask);
}
//reset start for resizing, moving
void handleButtRelease(XButtonReleasedEvent ev){
	cout << "Button release" << endl;
	if(ev.type == ButtonRelease)
    	start.subwindow = None;
}

//Adds frame to each client
void reparentWindow(Window window){
	
	//Attributes of origial window
	XGetWindowAttributes(disp, window, &attr);
	
	//TODO: add buttons to title bar, top left easiest
	int borderWidth = 3;
	int colour = 0xFFF000;
	int background = 0xFFFFFF;
	int titleColour = 0x000FFF;
	int exitColour = 0xFF0000;

	//Setting frame window charechtaristics
	frame = XCreateSimpleWindow(disp, DefaultRootWindow(disp), attr.x, attr.y, attr.width, attr.height + 20, borderWidth, colour, background);
	
	//Save set for if the window manager crashes
	XAddToSaveSet(disp, window);

	//Reparents child window
	XReparentWindow(disp, window, frame, 0, 20);

	//Displays parent window(frame)
	XMapWindow(disp, frame);
	
	//Create title bar
	title = XCreateSimpleWindow(disp, frame, attr.x, attr.y,
	attr.width, 20, 0, colour, titleColour);
	XMapWindow(disp, title);

	//Create exit button
	exitButton = XCreateSimpleWindow(disp, frame, attr.x, attr.y,
	20, 20, 0, colour, exitColour);
	XMapWindow(disp, exitButton);

	client = window;

	//Grab for exit button
	XGrabButton(disp, 1, AnyModifier, exitButton, 
	True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
	GrabModeAsync, GrabModeAsync, None, None);

	//Click on the title bar event (Button1)
	XGrabButton(disp, 1, AnyModifier, title, 
	True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
	GrabModeAsync, GrabModeAsync, None, None);
	
	//Frame event (Alt-Button3)
	XGrabButton(disp, 3, Mod1Mask, frame, 
	True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
 	GrabModeAsync, GrabModeAsync, None, None);

	//Alt + F4 event (for closing windows)
	XGrabKey(disp, XKeysymToKeycode(disp, XK_F4), 
	Mod1Mask, window, 
	True, GrabModeAsync, GrabModeAsync);
}

void handleMotion(XMotionEvent ev) {

	cout << "Motion Event" << endl;

	int xdiff = ev.x_root - start.x_root;
    int ydiff = ev.y_root - start.y_root;
	cout << "Xdiff: " << xdiff << endl;
	cout << "Ydiff: " << ydiff << endl;

	//Move window by dragging title bar with left mouse
	if(start.window == title && start.button != 3){
		cout << "Start window move" << endl;
        XMoveWindow(disp, parent,
        attr.x + xdiff,
        attr.y + ydiff);
		cout << "End window move" << xdiff << ydiff << endl;
	}
	
	//Resize windows with alt + right mouse button
	//TODO: Set to resizing by grabbing the border, may be difficult as another window will need to be created to be used as the pull
	//TODO:(bug) resizing fails when mouse moved outside of the window
	//TODO:(bug) bug when moved to title bar
	if(start.subwindow != None){
		
		cout << "Frame resize start" << ev.window << endl;
		XResizeWindow(disp, ev.window,
        MAX(100, attr.width + xdiff),
        MAX(100, attr.height + ydiff));
		cout << "Frame resize end" << endl;

		cout << "Client resize start: " << ev.subwindow << endl;
		XResizeWindow(disp, ev.subwindow,
        MAX(100, attr.width + xdiff),
        MAX(100, attr.height + ydiff));
		cout << "Client resize end" << endl;

		cout << "Title resize start" << title << endl;
		XResizeWindow(disp, title,
        MAX(100, attr.width + xdiff), 20);
		cout << "Title resize end" << endl;
    }
}

void handleButton(XButtonEvent ev) {

	cout << "Button press event" << endl;
	//Fixes titles not being reset, allows selection and movement
	if(ev.button != 3 && ev.window != exitButton){
		title = ev.window;
	}

	//Exit button 
	//TODO: Need to reset client
	else if(ev.window == exitButton){
		cout << "ExitButton start" << endl;
		getParent(ev.window);
		XChangeSaveSet(disp, client, SetModeDelete);
        XKillClient(disp, client);
		XDestroyWindow(disp, parent);
		cout << "ExitButton end" << endl;;
	}

	
	//TODO:(bug) Resizing with title slight bug
	//TODO:(bug) Resizing title only happens when clicked on with 1
	//Button 3 
	//Sets the start of the pointer for moving it
	if(ev.subwindow != None && ev.button != 1){
		cout << "Button 3 + Alt press start" << endl;
        XGetWindowAttributes(disp, ev.subwindow, &attr);
		XRaiseWindow(disp, ev.window);
		start = ev;
		cout << "Button 3 + Alt press end" << endl;
    }
	
	//Button 1
	//For pressing on the title bar
	//Without ev.button != 3 it crashes as it assumes that button 1 is pressed
	else if(ev.window == title && ev.button != 3 && ev.window != exitButton){
		cout << "Button 1 title press start" << endl;
		getParent(ev.window);
		XGetWindowAttributes(disp, parent, &attr);
		XRaiseWindow(disp, parent);
		start = ev;
		cout << "Button 1 title press end" << endl;

	}
}

//Will need if the program exits out itself
/*void unframe(Window window){
	cout << "Unframe start" << endl;
	XUnmapWindow(disp, frame);
	XReparentWindow(disp, window, DefaultRootWindow(disp), 0, 0);
	XChangeSaveSet(disp, window, SetModeDelete);
	XDestroyWindow(disp, frame);
}*/


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
	else if(ev.keycode == XKeysymToKeycode(disp,XK_F4)){
			cout << "Kill window start" << endl;
			getParent(ev.window);
			XChangeSaveSet(disp, ev.window, SetModeDelete);
			cout << "Kill client" << endl;
            XKillClient(disp, ev.window);
			cout << "Destroy frame: " << frame << endl;
			XDestroyWindow(disp, parent);
			cout << "Kill window end" << endl;
    }
}

//Used for unmapping the frame if client minimised(unmapped), return to root and destroy coressponding frame
void handleUnmapNotify(XUnmapEvent ev) {
	cout << "Unmap start" << endl;

}

void handleMapRequest(XMapRequestEvent ev){
	
	//Reparents window that sent map request
	reparentWindow(ev.window);
	cout << "Window reparented" << endl;

	//Remaps the child window
	XMapWindow(disp, ev.window);
	cout << "Frame mapped" << endl;
}

//Fixes xterm being tiny when reparented, allows client to set attribs
void handleConfigRequest(XConfigureRequestEvent ev){
	cout << "Window configure start" << endl;
	XWindowChanges ch;
	ch.x = ev.x;
	ch.y = ev.y;
	ch.width = ev.width;
	ch.height = ev.height;
	ch.border_width = ev.border_width;
	ch.sibling = ev.above;
	ch.stack_mode = ev.detail;

	XConfigureWindow(disp, ev.window, ev.value_mask, &ch);
	cout << "Window configured. ID:" << ev.window << endl;
}

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
			handleUnmapNotify(ev.xunmap); break;

	}
}

int main(void) {

	//fail if can't connect, another window manager running
    if(!(disp = XOpenDisplay(0x0))) return 1;
	setMasks();
    start.subwindow = None;
    while(True){
		//basic X event loop		
		eventLoop();
    }
} 

