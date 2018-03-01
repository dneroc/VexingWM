#include <X11/Xlib.h>
#include <X11/keysym.h>
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
Window exitButton;		//Exit button at the top
Window resize;			//Variable for resizing
Window frame; 			//Reparent variable
Window client;			//For resizing the client
Window parent, *child;	//Parent and children of query tree
unsigned int nchild;	//No. of children in query tree
Window test;			//Test window for closing


//Gets the children and parent of a window
void queryTree(Window window){
	cout << "Get frame start" << endl;
	Window root;
	child = NULL;
	XQueryTree(disp, window, &root, &parent, &child, &nchild);
}

//EventMasks, only sends events of this type on the root window
void setMasks(){
	
	//Alt + F1, nothing for now
    //XGrabKey(disp, XKeysymToKeycode(disp, XK_F1), 
	//Mod1Mask, DefaultRootWindow(disp), 
	//True, GrabModeAsync, GrabModeAsync);

	//Alt + F2, nothing for now
	//XGrabKey(disp, XKeysymToKeycode(disp, XK_F2), 
	//Mod1Mask, DefaultRootWindow(disp), 
	//True, GrabModeAsync, GrabModeAsync);
	
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

	int borderWidth = 2;
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
	//TODO:(bug) resizing fails when mouse moved outside of the window
	//TODO:(bug) bug when moved to title bar
	if(start.subwindow != None){
		
		//cout << "Frame resize start" << ev.window << endl;
		//Ev.window = frame
		XResizeWindow(disp, ev.window,
        MAX(100, attr.width + xdiff),
        MAX(100, attr.height + ydiff));
		//cout << "Frame resize end" << endl;

		//cout << "Client resize start: " << ev.subwindow << endl;
		//Client = client
		XResizeWindow(disp, client,
        MAX(100, attr.width + xdiff),
        MAX(100, attr.height + ydiff));
		//cout << "Client resize end" << endl;

		//cout << "Title resize start" << title << endl;
		//Title = title
		XResizeWindow(disp, title,
        MAX(100, attr.width + xdiff), 20);
		//cout << "Title resize end" << endl;
    }
}

void handleButton(XButtonEvent ev) {

	cout << "Button press event" << endl;
	queryTree(ev.window);
	queryTree(parent);
	title = child[1];
	exitButton = child[2];
	client = child[0];
	resize = ev.window;
	cout << "Alt + 3: " << ev.window << endl;
	cout << "Frame: " << frame << endl;
	//cout << "ALt + 3 subwindow: " << ev.subwindow << endl;
	cout << "Client: " << client << endl;
	//cout << "Title: " << title << endl;
	cout << "Button press event end" << endl;

	//Left click + exit button, kills whole window
	if(ev.window == exitButton && ev.button != 3){
		cout << "ExitButton start" << endl;
		queryTree(ev.window);
		XChangeSaveSet(disp, client, SetModeDelete);
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
	
	//TODO:(bug) Resizing two frames, check the right variables in motion event
	//Button 3 sets the start of the pointer for moving it
	else{
		cout << "Button 3 + Alt press start" << endl;
		queryTree(ev.window);
		//Get attributes of the frame, Alt+3 window = frame
		client = child[0];
		title = child[1];
        XGetWindowAttributes(disp, ev.window, &attr);
		cout << "Get attr" << endl;
		XRaiseWindow(disp, ev.window);
		cout << "Raise window" << endl;
		start = ev;
		cout << "Button 3 + Alt press end" << endl;
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
		XChangeSaveSet(disp, ev.window, SetModeDelete);
		cout << "Kill client" << endl;
        XKillClient(disp, ev.window);
		cout << "Destroy frame: " << frame << endl;
		XDestroyWindow(disp, parent);
		cout << "Kill window end" << endl;
    }

	//Alt+Tab switch windows
	else if(ev.keycode == XKeysymToKeycode(disp,XK_Tab)){
		queryTree(ev.window);
		XRaiseWindow(disp, child[0]);	
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

