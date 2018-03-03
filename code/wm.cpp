#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <iostream>
#include <unordered_map>

using namespace std;

//Max function for setting minimum window size
#define MAX(a, b) ((a) > (b) ? (a) : (b))

unordered_map<Window, Window> clients;

Display * disp;			//main display
XWindowAttributes attr;	//attributes of a window
XButtonEvent start; 	//save pointers state at the beginning
XEvent ev;				//event variable
Window title;			//Titlebar variable
Window exitButton;		//Exit button on the titlebar
Window maxButton;		//Max button on the titlebar
Window client;			//For resizing the client
Window parent, *child;	//Parent and children of query tree
unsigned int nchild;	//No. of children in query tree

/*void createTitleMenu(){
	int titleBorder = 0x7e7e7e;
	int titleColour = 0xd3d3d3;
	int buttonColour = 0x6666ff;
	int buttonBorder = 0x242424;

	//Title menu
	XGetWindowAttributes(disp, DefaultRootWindow(disp), &attr);
	Window titleMenu = XCreateSimpleWindow(disp, 
	DefaultRootWindow(disp), (attr.x - 3), (attr.height - 39), 
	attr.width, 33, 3, titleBorder, titleColour);
	XMapWindow(disp, titleMenu);

	//TODO: Add text to window and make it launchable, check alt tab bug first
	//Xterm Launcher title menu button
	//XGetWindowAttributes(disp, titleMenu, &attr);
	Window xterm = XCreateSimpleWindow(disp, 
	DefaultRootWindow(disp), attr.x, (attr.height - 36), 
	(attr.width / 4), 27, 3, buttonBorder, buttonColour);
	XMapWindow(disp, xterm);
}*/

//Gets the children and parent of a window
void queryTree(Window window){

	cout << "Query Tree" << endl;
	Window root;
	child = NULL;
	XQueryTree(disp, window, &root, &parent, &child, &nchild);
}

//Sets title, exitButton, and client if parent is frame
void setChildren(Window parent){
	queryTree(parent);
	title = child[0];
	exitButton = child[1];
	maxButton = child[2];
	client = child[3];
}

//TODO:(bug) Resizes the wrong window once
void resize(Window window, int width, int height){
	
	setChildren(window);

	cout << "Frame resize start" << window << endl;
	XResizeWindow(disp, window,
    MAX(100, width), MAX(100, height));
	cout << "Frame resize end" << endl;

	cout << "Client resize start: " << client << endl;
	XResizeWindow(disp, client,
    MAX(100, width), MAX(100, height));
	cout << "Client resize end" << endl;

	cout << "Title resize start" << title << endl;
	XResizeWindow(disp, title, MAX(100, width), 20);
	cout << "Title resize end" << endl;
}

void setFrameMasks(Window window, Window frame, Window title, Window exitButton, Window maxButton){

	//Click on the title bar event (Button1)
	XGrabButton(disp, 1, AnyModifier, title, 
	True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
	GrabModeAsync, GrabModeAsync, None, None);

	//Grab for exit button
	XGrabButton(disp, 1, AnyModifier, exitButton, 
	True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
	GrabModeAsync, GrabModeAsync, None, None);

	//Event for maximising button
	XGrabButton(disp, 1, AnyModifier, maxButton, 
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

	//Alt + F4 event (for closing windows)
	XGrabKey(disp, XKeysymToKeycode(disp, XK_F4), 
	Mod1Mask, window, 
	True, GrabModeAsync, GrabModeAsync);

	//Alt + left arrow (for move window to the left)
	XGrabKey(disp, XKeysymToKeycode(disp, XK_Left), 
	Mod1Mask, frame, 
	True, GrabModeAsync, GrabModeAsync);
	
	//Alt + right arrow	(for move window to the right)
	XGrabKey(disp, XKeysymToKeycode(disp, XK_Right), 
	Mod1Mask, frame, 
	True, GrabModeAsync, GrabModeAsync);

	//Alt + up arrow (for window fill screen)
	XGrabKey(disp, XKeysymToKeycode(disp, XK_Up), 
	Mod1Mask, frame, 
	True, GrabModeAsync, GrabModeAsync);
	
	//Get requests and change them, allows clients to resize
	XSelectInput(disp, frame,
	SubstructureRedirectMask | SubstructureNotifyMask);
}

//EventMasks, only sends events of this type on the root window
void setMasks(){
	
	//Alt + Tab for window switching
	XGrabKey(disp, XKeysymToKeycode(disp, XK_Tab), 
	Mod1Mask, DefaultRootWindow(disp), 
	True, GrabModeAsync, GrabModeAsync);

	//Alt + Enter(for creating new windows)
	XGrabKey(disp, XKeysymToKeycode(disp, XK_Return), 
	Mod1Mask, DefaultRootWindow(disp), 
	True, GrabModeAsync, GrabModeAsync);
	
	//Alt + Esc(for escaping the client)
	XGrabKey(disp, XKeysymToKeycode(disp, XK_Escape), 
	Mod1Mask, DefaultRootWindow(disp), 
	True, GrabModeAsync, GrabModeAsync);

	//TODO:(later) For creating a submenu on the root display
	/*//Button 3 on root window for menu
	XGrabButton(disp, 3, AnyModifier, DefaultRootWindow(disp), 
	True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
 	GrabModeAsync, GrabModeAsync, None, None);*/
	
	//Get requests from client, allows clients to resize
	XSelectInput(disp, DefaultRootWindow(disp),
	SubstructureRedirectMask | SubstructureNotifyMask);
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

//Adds frame to each client
void reparentWindow(Window window){
	
	int borderWidth = 2;
	int colour = 0xFFF000;
	int background = 0xFFFFFF;
	int titleColour = 0x000FFF;
	int exitColour = 0xFF0000;
	int maxColour = 0x00FF00;

	//Attributes of origial window
	XGetWindowAttributes(disp, window, &attr);

	//Setting frame window charechtaristics
	Window frame = XCreateSimpleWindow(disp, 
	DefaultRootWindow(disp), attr.x, attr.y, 
	attr.width, attr.height + 20, borderWidth, colour, background);

	//Create title bar
	Window title = XCreateSimpleWindow(disp, frame, attr.x, attr.y,
	attr.width, 20, 0, colour, titleColour);

	//Create exit button
	Window exitButton = XCreateSimpleWindow(disp, frame, attr.x,
	attr.y, 20, 20, 0, exitColour, exitColour);

	//Create maximise button
	Window maxButton = XCreateSimpleWindow(disp, frame, attr.x + 21,
	attr.y, 20, 20, 0, maxColour, maxColour);

	//Reparents client window
	XReparentWindow(disp, window, frame, 0, 20);

	//Displays parent window(frame)
	XMapWindow(disp, frame);
	
	//Displays title (1st child of frame)
	XMapWindow(disp, title);

	//Displays exit button (2nd child of frame)
	XMapWindow(disp, exitButton);
	
	//Displays max button (3rd child of frame)
	XMapWindow(disp, maxButton);
	
	//Mapping of window and frame for referencing later
	clients[window] = frame; 

	setFrameMasks(window, frame, title, exitButton, maxButton);
}

void handleMapRequest(XMapRequestEvent ev){
	
	//TODO: Add client window to list/array	

	//Reparents window that sent map request
	reparentWindow(ev.window);
	cout << "Window reparented" << endl;

	//Remaps the child window (3rd child of frame)
	XMapWindow(disp, ev.window);
	cout << "Client mapped" << endl;
}

//Used for unmapping the frame if client minimised(unmapped), return to root and destroy coressponding frame
void handleUnmapNotify(XUnmapEvent ev) {
	cout << "Unmap start" << endl;
	
	//Check to see if it is a client window, else we don't unmap frame
	if(!clients.count(ev.window)){
		cout << "Not a client window" << endl;
		return;
	}
	
	//Find the frame for this client window
	Window unframe = clients[ev.window];

	//Unmap it from the display
	XUnmapWindow(disp, unframe);

	//Destroy the frame
	XDestroyWindow(disp, unframe);

	//Remove the mapping of window
	clients.erase(ev.window);

	cout << "Unmap window" << endl;
}

//reset start for resizing, moving
void handleButtRelease(XButtonReleasedEvent ev){
	cout << "Button release" << endl;
	if(ev.type == ButtonRelease)
    	start.subwindow = None;
}

void handleMotion(XMotionEvent ev) {

	cout << "Motion Event" << endl;

	int xdiff = ev.x_root - start.x_root;
    int ydiff = ev.y_root - start.y_root;
	//cout << "Xdiff: " << xdiff << endl;
	//cout << "Ydiff: " << ydiff << endl;

	//Move window by dragging title bar with left mouse
	if(start.window == title && start.button != 3){
		//cout << "Start window move" << endl;
        XMoveWindow(disp, parent,
        attr.x + xdiff,
        attr.y + ydiff);
		//cout << "End window move" << xdiff << ydiff << endl;
	}
	
	//Resize windows with alt + right mouse button
	//TODO: Set to resizing by grabbing the border, may be difficult as another window will need to be created to be used as the pull
	//TODO:(bug) When resizing the client window manages to appear a little below the frame
	else if(start.subwindow != None && start.subwindow != DefaultRootWindow(disp)){
		
		resize(ev.window, (attr.width + xdiff), (attr.height + ydiff));
		
    }
}

void handleButton(XButtonEvent ev) {

	cout << "Button press event" << endl;
	if(ev.window != DefaultRootWindow(disp)){
		queryTree(ev.window);
		setChildren(parent);
	}
	
	//Left click + red button closes window
	if(ev.window == exitButton && ev.button != 3){
		cout << "ExitButton start" << endl;
		queryTree(ev.window);
        XKillClient(disp, client);
		cout << "ExitButton end" << endl;
	}
/*
	//Alt+Up fill screen
	else if(ev.keycode == XKeysymToKeycode(disp, XK_Up)){
		cout << "Alt + up arrow" << endl;
		XGetWindowAttributes(disp, parent, &attr);
		XMoveWindow(disp, ev.window, attr.x, attr.y);
		resize(ev.window, attr.width - 4, attr.height - 4);
	}*/

	/*else if(ev.window == maxButton && ev.button != 3){
		queryTree(ev.window);
		XGetWindowAttributes(disp, parent, &attr);
		XMoveWindow(disp, ev.window, attr.x, attr.y);
		resize(ev.window, attr.width - 4, attr.height - 4);
	}*/

	//Left click + title bar, raises window, move window
	else if(ev.window == title && ev.button != 3){
		cout << "Button 1 title press start" << endl;
		queryTree(ev.window);
		//Gets the attributes of the frame for moving
		XGetWindowAttributes(disp, parent, &attr);
		XRaiseWindow(disp, parent);
		start = ev;
		cout << "Button 1 title press end" << endl;
	}
	
	/*//Left click to raise window
	//TODO: Fix, doesn't work yet
	else if(ev.button != 3 && (ev.window != title || ev.window != exitButton)){
		cout << "Raise window left click anywhere" << endl;
		XRaiseWindow(disp, ev.window);
		cout << "Raise window left click anywhere end" << endl;
	}*/

	//Button 3 sets the start of the pointer for moving it
	else if (ev.button == 3){
		cout << "Button 3 + Alt press start" << endl;
		//setChildren(ev.window);
        XGetWindowAttributes(disp, ev.window, &attr);
		XRaiseWindow(disp, ev.window);
		start = ev;
		cout << "Button 3 + Alt press end" << endl;
    }

	/*else if(ev.subwindow == None && ev.button == 3){
		cout << "Start submenu create" << endl;
		XGetWindowAttributes(disp, DefaultRootWindow(disp), &attr);
		Window menu = XCreateSimpleWindow(disp, 
		DefaultRootWindow(disp), ev.x, ev.y, 
		40, 80, 0, 0xFFF000, 0xFFF000);
		XMapWindow(disp, menu);
	}*/
	
	
}

void handleKey(XKeyEvent ev) {
	//Alt + Enter calls a system call(currently xterm)
	if(ev.keycode == XKeysymToKeycode(disp, XK_Return)){
		system("xterm &");
	}

	//Alt + Escape closes window Manager
	else if(ev.keycode == XKeysymToKeycode(disp, XK_Escape)){
	    XCloseDisplay(disp);	
	}
	
    //Alt+F4 closes window
	else if(ev.keycode == XKeysymToKeycode(disp,XK_F4)){
		cout << "Kill window start" << endl;
        XKillClient(disp, ev.window);
		cout << "Kill window end" << endl;
    }

	//Alt+Tab switch windows
	else if(ev.keycode == XKeysymToKeycode(disp,XK_Tab)){
		cout << "Window switch start" << endl;
		queryTree(ev.window);
		XRaiseWindow(disp, child[0]);	
    }

	//Alt+Left move window to screen left
	else if(ev.keycode == XKeysymToKeycode(disp,XK_Left)){
		cout << "Alt + left arrow" << endl;
		XGetWindowAttributes(disp, parent, &attr);
		XMoveWindow(disp, ev.window, attr.x, attr.y);
		resize(ev.window,(attr.width / 2), attr.height - 4);
		XRaiseWindow(disp, ev.window);	
	}

	//Alt+Right move window to screen right
	else if(ev.keycode == XKeysymToKeycode(disp,XK_Right)){
		cout << "Alt + left arrow" << endl;
		XGetWindowAttributes(disp, parent, &attr);
		XMoveWindow(disp, ev.window, (attr.x + (attr.width / 2)), attr.y);
		resize(ev.window,(attr.width / 2) - 4, attr.height - 4);
		XRaiseWindow(disp, ev.window);
	}

	//Alt+Up fill screen
	else if(ev.keycode == XKeysymToKeycode(disp, XK_Up)){
		cout << "Alt + up arrow" << endl;
		queryTree(ev.window);
		XGetWindowAttributes(disp, parent, &attr);
		XMoveWindow(disp, ev.window, attr.x, attr.y);
		resize(ev.window, attr.width - 4, attr.height - 4);
		XRaiseWindow(disp, ev.window);	
	}
}

//Event loop for intercepting different types of events
void eventLoop()
{
	XNextEvent(disp, &ev);
	
	switch(ev.type){

		case ConfigureRequest:	
			handleConfigRequest(ev.xconfigurerequest); break;

		case MapRequest:		
			handleMapRequest(ev.xmaprequest); break;

		case UnmapNotify:             
			handleUnmapNotify(ev.xunmap); break;

		case KeyPress:
			handleKey(ev.xkey);	break;

		case ButtonPress:
			handleButton(ev.xbutton); break;
			
		case ButtonRelease:		
			handleButtRelease(ev.xbutton); break;

		case MotionNotify: 		
			handleMotion(ev.xmotion); break;
	}
}

int main(void) {

	//fail if can't connect, another window manager running
    if(!(disp = XOpenDisplay(0x0))) return 1;
	//createTitleMenu();
	setMasks();
    start.subwindow = None;
    while(True){
		//basic X event loop		
		eventLoop();
    }
} 

