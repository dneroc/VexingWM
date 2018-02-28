#Blog Number 5


This week we switched from Python to C. Our reason for doing this is that the Xlib port for Python was very experimental and undocumented. It was very difficult to figure out how many of the mechanisms in X worked in python and sometimes the functionality in the Library had not yet been implimented in Python, making some features impossible. We experimented working in C and found we covered a lot of ground very fast which was due to the originial X libraries being written in C, and thus being easy to understand. 

We figured out how to filter key presses and made a well functioning event loop, which is the center of our program. It is now possible to open terminal windows using our window manager and shift focus to whichever window is under the mouse. Many of the basic functionalities are now implimented in our project. Over the next week we would like to have a usable product that's testable by users


Testing has been made a lot easier since we figured out that all the software we need is available on the lab computers(the Python libraries aren't on the lab computers but the C ones are).
