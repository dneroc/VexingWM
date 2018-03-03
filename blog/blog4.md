# Testing: Blog 3 Mar 2018

Yesterday worked on creating and writing some tests for the program. Looked at creating unit tests and learned a bit about asserts but not much besides that. Below are the test resuts I got after doing the tests this morning.

![] (https://gitlab.computing.dcu.ie/craigd2/2018-CA326-dcraig-XWindowManager/raw/master/Screenshots/030318Test.png)

The biggest issue right now is considering the reparenting of windows. I believe launching Chromium or LibreOffice causes an issue because in the case of LibreOffice it creates a loading screen which gets framed and when that is gone the frame foes not dissapear. A similar issue is happening with Chromium as you can see a smaller empty frame when you close it which leads me to believe that Chromium is creating a smaller window. The issue I think is that frames are not being destroyed when an application closes a window itself. I think fixing this issue might fix the issue of the programs not being parented correctly. I need to add this in either way since a program might close itself, and not through the exit button. Below is an image of the incorrect framing.

![] (https://gitlab.computing.dcu.ie/craigd2/2018-CA326-dcraig-XWindowManager/raw/master/Screenshots/ChromiumReparentingFail.png)

The title bar is also causing some issues but I want to focus on the reparenting aspect and adding some more features/tests.

Today I'm going to work on fixing the reparenting issue and hopefully adding more features.