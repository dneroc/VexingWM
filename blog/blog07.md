# Blog 28 Feb 2018

![] (https://gitlab.computing.dcu.ie/craigd2/2018-CA326-dcraig-XWindowManager/raw/master/blog/Screenshot_20180228_130042.png)

Above is an image of the way our windows are laid out. The top level window is the root window seen in our code as "DefaultRootWindow(disp)". Then further on we have the frame window which is the window that gets created when a client is reparented. The frames children are the title bar and the client. Since the title and client are siblings specifiying which window is selected has caused some challange especially when selecting specific windows for resizing and moving. 


![] (https://gitlab.computing.dcu.ie/craigd2/2018-CA326-dcraig-XWindowManager/raw/master/blog/Screenshot_20180228_211220.png)

Above is our current build.

Today we managed to fix the window resizing crashes. After getting the window title click and move working correctly there was the issue of resizing causing a crash. This was cause by the program registering the wrong click. Then I had the issue of solving the resizing of the inner window and title bar. Only the frame was moving. Throughout the day I managed to fix and now both the title bar and client window resize correctly save for a few slight bugs such as the manager crashing when the mouse gets moved outside the window during resizing and the title bar not getting resized if it was not left clicked on.

Tried to work on window destrction and at this point the inside client and title are dissapearing but not the frame. Need to either unmap or destroy the frame. I also feel like there is structural improvement that could be made to the code, starting to look a bit spaghetti.

Still need to do create automated tests, fix window closing, and fix resizing bugs. Once window closing is fixed, then I can move on with creating title bar buttons to exit each program.
