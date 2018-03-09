# Progressing: Blog 1 Mar 2018

![] (https://gitlab.computing.dcu.ie/craigd2/2018-CA326-dcraig-XWindowManager/raw/master/Screenshots/WindowManagerWithExit.png)

The current window manager. Additions include the red exit button at the top left corner of each window frame and the begginings of a title bar seen at the bottom. The exit button is a child of frame and a sibling of titlebar and client window. With the help of tree query we can differentiate the siblings, especially needed for window resizing.

The biggest challange today was figurng out how to pick the correct variables and windows for closing and resizing. The code was a bit messy and made it confusing to see or figure out what each variable did. The first obstacle was figuring out the closing down of windows which was a case of selecting the right thing to close. I was able to get it to close for a single window per session as the variable was not reseting for each window. The soloution was to use XQueryTree() to find the parent of each client so I could destroy the frame. I was then able to create an exit button window and apply the same function to it to close the window.

Resizing at this point was still broken, crashing the system. The creation and correct implementation of the queryTree() function almost immedietly solved the issue. I was also able to easily implement window switching with alt+tab due to the queryTree() function.

Currently working on the titlebar menu as can be seen in the screenshot but will mainly be focusing on creating tests tommorrow since its pretty stable at the moment.