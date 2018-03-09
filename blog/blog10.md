# To the MAX: 03 March 2018

![] (https://gitlab.computing.dcu.ie/craigd2/2018-CA326-dcraig-XWindowManager/raw/master/Screenshots/WindowMaximised.png)

Above is the current window manager, I have removed the title menu and added a maximise button. It is the green one.

The first issue was clients not resizing within the frame. Although they were resizing with the frame they weren't resizing themselves which turned out to be that I wasn't getting the XChangeAttribute event which turned out to be a simple fix.

The next issue of today was solving the correct unframing when a client exited out itself. I solved this by creating a mapping of the frames and clients and checking if a window being unmapped is a client window. Since only the client windows should be framed this ensures that only their frames are destroyed. I also did a lot of code re-organising and creating more functions to make the code more re-usable. Still having issues with chromium framing but everything else seems to be framing fine for the most part. 

Next came the alt + left/right/up for window movement which was easy enough to implement. Currently a bit buggy but should be an easy enough fix. Since alt up maximises the window I introduced a maximise button. It is a bit buggy at the moment as seen below

![] (https://gitlab.computing.dcu.ie/craigd2/2018-CA326-dcraig-XWindowManager/raw/master/Screenshots/MoveBug.png)

Going to keep working on cleaining up the bugs and hopefully add another feature tomorrow. Below are the test results of tonight's testing.

![] (https://gitlab.computing.dcu.ie/craigd2/2018-CA326-dcraig-XWindowManager/raw/master/Screenshots/Test030318.png)