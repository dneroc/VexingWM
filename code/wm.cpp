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
XWindowAttributes titleAttr; //titlebar attributes
XButtonEvent start; 	//save pointers state at the beginning
XEvent ev;				//event variable
Window title;			//Titlebar variable
Window exitButton;		//Exit button at the top
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
	attr.y, 20, 20, 0, colour, exitColour);

	//Reparents client window
	XReparentWindow(disp, window, frame, 0, 20);

	//Displays parent window(frame)
	XMapWindow(disp, frame);
	
	//Displays title (1st child of frame)
	XMapWindow(disp, title);

	//Displays exit button (2nd child of frame)
	XMapWindow(disp, exitButton);
	
	//Mapping of window and frame for referencing later
	clients[window] = frame; 

	//Click on the title bar event (Button1)
	XGrabButton(disp, 1, AnyModifier, title, 
	True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
	GrabModeAsync, GrabModeAsync, None, None);

	//Grab for exit button
	XGrabButton(disp, 1, AnyModifier, exitButton, 
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
	
	//Get requests and change them, allows clients to resize
	XSelectInput(disp, frame,
	SubstructureRedirectMask | SubstructureNotifyMask);
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
	if(start.subwindow != None){
		
		//cout << "Frame resize start" << ev.window << endl;
		XResizeWindow(disp, ev.window,
        MAX(100, attr.width + xdiff),
        MAX(100, attr.height + ydiff));
		//cout << "Frame resize end" << endl;

		//cout << "Client resize start: " << ev.subwindow << endl;
		XResizeWindow(disp, client,
        MAX(100, attr.width + xdiff),
        MAX(100, attr.height + ydiff));
		//cout << "Client resize end" << endl;

		//cout << "Title resize start" << title << endl;
		XResizeWindow(disp, title,
        MAX(100, attr.width + xdiff), 20);
		//cout << "Title resize end" << endl;
    }
}

void handleButton(XButtonEvent ev) {

	cout << "Button press event" << endl;
	queryTree(ev.window);
	queryTree(parent);
	title = child[0];
	exitButton = child[1];
	client = child[2];

	
	if(ev.window == exitButton && ev.button != 3){
		cout << "ExitButton start" << endl;
		queryTree(ev.window);
        XKillClient(disp, client);
		XDestroyWindow(disp, parent);
		start = ev;
		cout << "ExitButton end" << endl;
	}

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
	
	//Left click to raise window
	else if(ev.button != 3 && (ev.window != title || ev.window != exitButton)){
		cout << "Raise window left click anywhere" << endl;
		XRaiseWindow(disp, ev.window);
		cout << "Raise window left click anywhere end" << endl;
	}

	//Button 3 sets the start of the pointer for moving it
	else{
		cout << "Button 3 + Alt press start" << endl;
		queryTree(ev.window);
		//Get attributes of the frame, Alt+3 window = frame
		client = child[2];
		title = child[0];
        XGetWindowAttributes(disp, ev.window, &attr);
		cout << "Get attr" << endl;
		XRaiseWindow(disp, ev.window);
		cout << "Raise window" << endl;
		start = ev;
		cout << "Button 3 + Alt press end" << endl;
    }
	
	
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
		queryTree(ev.window);
		cout << "Kill client" << endl;
        XKillClient(disp, ev.window);
		cout << "Destroy frame: " << parent << endl;
		XDestroyWindow(disp, parent);
		cout << "Kill window end" << endl;
    }

	//Alt+Tab switch windowsg
	else if(ev.keycode == XKeysymToKeycode(disp,XK_Tab)){
		queryTree(ev.window);
		XRaiseWindow(disp, child[0]);	
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

