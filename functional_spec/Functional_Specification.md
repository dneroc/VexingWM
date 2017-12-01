**0. Table of contents**

1.1. Introduction

1.1  Purpose

1.2  Overview

1.3  Glossary

**2. General Description**

2.1  Product/System Functions

2.2  User Characteristics and Objectives

2.3  Operational Scenarios

2.4  Constraints

**3.  Functional Requirements**

3.1  Moving windows

3.2  Connecting/Detaching Windows

3.3  Resizing Windows

3.4  Editing Configurations/Settings

3.5  Workspaces

3.6  Start Up Menu

3.7  Task Bar

3.8  Pin Windows

3.9   Statistics

3.10  Cheat Sheet

**4.  System Architecture**

**5.  High-Level Design**

5.1 LDS for Window Manager System

**6. Preliminary Schedule**

**1. Introduction**

**1.1 Purpose**

The purpose of this functional specification is to help design and describe the proposed X Window Manager for Linux. As well as showing the initial plan and specifications for the program, it will also serve as a guide for the production and completion of the project. This is intended to be read by the project programmers, project supervisor, project coordinator and the CA326 demonstration panel.

The reason for choosing this as our project is equal parts creating a useful window manager for linux and because it&#39;s something we are heavily interested in doing for ourselves.

**1.2 Overview**

The project being created is an X Window Manager for Linux. The program will be a hybrid of both tiling and floating window managers, based mostly on floating windows with tiling functionality within each window block. The design goal is to have a focus on user friendliness and mouse driven operation, with extended keyboard functionality. The window manager will have seamless floating and tiling combination that allows users to snap windows together, resize windows in different ways depending on each window context, or any other number of possible configurations.

Currently most window managers in Linux are written for the X Window System (X11/X)_._ X is a low level GUI framework that allows the movement of windows on the screen and handles user input. The X Window System does not mandate a particular type of window management which leaves a high degree of freedom for programmers. Currently two protocol client libraries exist to interact with X, Xlib and XCB, which are the lowest level of programming interfaces to X and allow users to interact with the structures that exist inside X.

While there are some window managers that are capable of creating floating windows while mainly being a tiling window manager, there are currently no X Window Managers on the market that do this efficiently and in a user friendly way. Most tiling window managers contain a lengthy manual that takes time to read and learn to use the program effectively, making it very difficult for new users to adopt such programs. And while many floating window managers do have some tilling capabilities, these are rarely used by the user as using the mouse is more convenient. The goal is to provide the best of both worlds, and to create a program that is approachable from both sides of the fence.

**1.3 Glossary**

_Mod_

Most modern keyboards boast a &quot;Windows Key&quot;. This key is redundant on machines that do not run Windows. Many Linux software makes use of the Windows key as a &quot;Modifier&quot; to user input, similar to how the shift key &quot;modifies&quot; the other keypresses made by a user by capitalising them. In cases in which the user does/can not use the Windows Key the Modifier can be assigned to the Alt key.

_X Window System(X11/X)_

Is a window system for bitmap displays written for Unix-like systems, providing the framework for drawing and manipulating windows in bitmap displays and receiving input.

_Workspaces_

Virtual desktops that can be switched between and allow the opening of different windows on each workspace. Usually used to separate workflows such as work and entertainment.

_Window Block_

A group of individual windows that have been connected together.

_Tiling Window Manager_

A Window Manager in which all windows are organized frames that do not usually overlap. Mostly keyboard driven.

_Floating Window Manager_

A Window Manager in which windows can be dragged and resized freely as well as be stacked over one another. Usually mouse driven.

_Extended Window Manager Hints(NetWM)_

It is the currently the most widely accepted standard of the X Window System improving on the previously used Inter-Client Communication Conventions Manual.

_Inter-Client Communication Conventions Manual(ICCCM)_

Previously most accepted standard regulating the X Window System and functionality.

_Title Bar_

The horizontal bar that runs across the top of a window and contains buttons such as close, maximise and minimise.

**2. General Description**

**2.1 Product/System Functions**

The main functionality of this program is being able to manipulate and create windows, work in different workspaces, be usable with both mouse and keyboard, and have features that most users would expect such as taskbar, start up menu,terminal accessand others.

There are a number of main functions that are planned to be implemented listed below:

- Seamless Floating and Pseudo-Tiling Window Operations
- Mouse Driven
- Extended Keyboard Functionality
- GUI and Javascript Configuration
- Multiple Workspaces
- Task Bar and Startup Menu
- Convenient Terminal Access
- Statistics
- Cheat Sheet

**2.2 User Characteristics and Objectives**

The user community is going to be two main types of users. Beginners to tiling window managers that are familiar with floating window managers but are looking for an accessible tilling X Window Manager and power users that have a great understanding of keyboard driven tiling window managers but are looking for something that resolves issues that they may be having with their current window manager or who are interested in experimenting with their workflow.

We expect users to have basic computer skills as this is a Linux based window manager, which requires the users to install Linux on their own systems. Users will also have to install the window manager on their system which requires a basic understanding of Linux.

As many floating window managers are available, and default by most distros, on Linux, such as the Mutter Window Manager for Ubuntu, we expect many of our beginner users not having much or any experience with tiling window managers. The goal is to provide something that is easy to get started on and also easy to master.

As for the experienced users, we expect a good knowledge of the Linux system and the use of tiling window managers. The program created hopes to provide a familiar and intuitive experience, with all the functionality and customization that the experienced users are used to.

**2.3 Operational Scenarios**

_Moving a window_

Joey wants to move a window from the left side of the screen to the right side. He clicks and holds on the title bar of the window and drags it across with the mouse.

_Moving a window into the next workspace_

Joey wants to move a window from the current workspace that he is in, to the one on the right. He clicks and holds on the title bar of the window and drags it across with the mouse the the right side of his screen. The workspace changes to the next one and he places it on the screen.

_Connecting windows_

Joey has two floating windows open on his screen. He grabs the window on the left and brings the right border of the window to the left border of the right window. The system &quot;snaps&quot; the two windows, resizing the window that was moved to the height of the window that was not moved.

_Detaching a window_

Joey wants to detach a window from a window block. He holds down the &quot;mod&quot; button and all of the windows in the block become temporarily floating. He holds and clicks on the title bar of a window and drags it away to remove it from the block.

_Splitting all windows in a block_

Joey wants to separate all the windows in a window block. He clicks the split button at the top of the title bar and all windows in that block become detached from each other.

_Resizing windows within a window block_

Joey wants to change the width of a window within the window block but not change the size of the actual block. He selects one of the inner horizontal borders and drags it across to change the split of the windows attached.

_Accessing the startup menu_

Joey wants to access the startup menu so he can get to the GUI configuration. He right clicks on the background of the screen. This brings up the startup menu where he can access the GUI configuration.

_Using the GUI configuration_

Joey wants to change the way the windows behave. He doesn&#39;t like that dragged windows get resized when connected to another window. He opens the GUI configuration window from the startup menu and updates the relevant option.

_Using the javascript configuration file_

Joey wants to change the keybindings to his own preference. Tim opens the javascript configuration file from the startup menu. He finds the key binding option and edits the keybindings to his own preference.

_Opening a program using the taskbar_

Joey wants to open LibreOffice. He moves his mouse to the bottom of the screen where a task bar is placed. He then clicks on the LibreOffice icon and the program is loaded.

_Switching between workspaces using the mouse_

Joey wants to switch to workspace 2 to work on his project. He uses his mouse and clicks &quot;Workspace 2&quot; on the top of the screen which brings him to the workspace.

_Pinning windows_

Joey wants to keep a window in place throughout all of his workspaces. He clicks the &quot;pin&quot; button on the title bar of the window. He then switches workspaces where the window is &quot;pinned&quot; in the same spot as the previous workspace.

**2.4 Constraints**

_Time constraint_

The project must be completed and uploaded, along with any required documentation by 5pm, 09/0/2018. This includes the design, implementation, and testing of the program and documents like the Technical Specification, User Guide, blogs, and supervision forms. And lastly a video walkthrough. All documents must be completed in Markdown.

_Linux and X11_

All X Window Managers are now based on X11 and in turn everything the program does must go through this system. As X11 only runs on Linux we are constrained to using the Linux system and all of it&#39;s features.

_Testing Constraints_

Finding advanced users that are used to tiling window managers will be much more difficult to find than beginner users.

_Extended Window Manager Hints(NetWM)_

NetWM is an X Window System standard that many of today&#39;s window managers use which will be followed in this project. It is based on the Inter-Client Communications Conventions Manual (ICCCM) that is known for being ambiguous and hard to implement and NetWM tries to fix and improve on that. This project is going to follow the standards set out by NetWM.

_Memory_

The window manager must not be too big or use too much memory as there must be space left to run the OS and other system programs.

**3. Functional Requirements**

**3.1 Moving Windows**

_Description_

Users will be able to move windows by clicking and dragging on the title bar of the window or by pressing the corresponding keyboard buttons. Users will also be able to move windows to surrounding workspaces by dragging the window to the side of the screen.

_Criticality_

Essential as without it users are unable to attach or detach windows together, move windows out of the way to reveal other windows or in any way affect their placement.

_Technical issues_

Ensuring windows cannot be moved completely off the screen causing the user to be unable to access the window. Also have to make sure moving windows on top of other windows does not cause them to interfere.

_Dependencies with other requirements_

In order to move windows between workspaces, workspace functionality first needs to be implemented. The ability to move Windows should not interfere with the ability to attach/detach windows.

**3.2 Connecting/Detaching Windows**

_Description_

One of the main functions of this window manager are to allow users to connect and detach windows using the mouse and/or keyboard. The users can bring one window next to another and have them connect together to form a &quot;block&quot; that will essentially work as a single window that the user can move around. The connected window will resize to fit the height/width of the other window depending on where it was connected. The user will also be able to detach windows in this block by using a &quot;mod&quot; key and dragging with the mouse or by selecting a button on the title bar. There will also be an option to detach all windows in a block from each other.

_Criticality_

Essential as this is one of the main goals of our program and without it the program wouldn&#39;t be able to be called a tiling/floating hybrid window manager.

_Technical issues_

Functionality for how Windows Should interact with each other is not available in the X system so a solution will have to be implemented in the Window Manager.

_Dependencies with other requirements_

This functionality cannot interfere with the ability to move or resize other windows.

**3.3 Resizing Windows**

_Description_

Users will be able to expand and minimize windows by pressing a button on the title bar or by using the keyboard. They will also be able to resize individual windows and window blocks using the mouse and dragging to make them bigger, smaller, wider, and higher. Users will also be able to resize individual windows within a window block by dragging the inside borders.

_Criticality_

Essential as it&#39;s typical of every window manager to allow its windows to be resized. Without it users may not be able to view content or fit all of their windows within the screen.

_Technical issues_

It must be ensured that resizing blocks causes all the windows in the same block of windows to expand if it is joined to a block.

_Dependencies with other requirements_

Windows should not join to other windows as a result of being resized.

**3.4 Editing Configurations/Settings**

_Description_

Users will be able to edit the settings and configurations of the window manager by going into the startup menu and selecting the GUI configuration menu or editing the javascript file. The GUI configuration will have a set of main options that the user will be able to change based on their preference while users who want to have a much more advanced configuration can go to the javascript file. The configuration will allow users to change things like window behaviour, theme and colours, keybindings, and a host of other options.

_Criticality_

Quite important as it allows the user to customize their experience and set up the manager the way they prefer it. There will be default options that we think best suits the majority of users but don&#39;t want to restrict them.

_Technical issues_

Ensuring that all configuration changes represent the correct changes within the system and that users can&#39;t break the functionality and appearance of the program through accidental configuration changes. The menu will be built on top of a high level X toolkit such as GNOME or QT.

_Dependencies with other requirements_

The layout of the GUI and the syntax of the configuration file is dependent on the features already implemented in the program.

**3.5 Workspaces**

_Description_

Workspaces allow multiple desktop spaces where windows can be opened. This allows for users to have certain related windows open together such as having entertainment related windows such as internet browser and video player open in &quot;Workspace 1&quot; and work related windows such as a word editor and excel sheet open on &quot;Workspace 2&quot;. Can be switched between using the mouse and clicking on individual workspaces at the top of the screen or by using keyboard shortcuts. Will also be able to move windows into workspaces by dragging the window to the side of the screen to put it in workspace next to it.

_Criticality_

Not essential but a feature of many current window managers implement and something that users enjoy in similar product.

_Technical issues_

Ensuring that windows only open in workspaces that they are opened in and not in the others and that dragging the windows into another workspace removes it from the previous workspace and stores it in the correct one

_Dependencies with other requirements_

None.

**3.6 Start Up Menu**

_Description_

The startup menu will be accessed by right clicking on the desktop background and will allow you to launch currently installed programs and access the configuration menu. It will also contain your most used programs for quick and easy access, through the use of statistics. There will also be an icon on the taskbar that opens a start menu.

_Criticality_

Quite essential as without a startup menu, users would have to launch programs through the terminal which would go against the user friendly focus on the program

_Technical issues_

Ensuring the startup menu does not open when right clicking in other open windows or any areas where it is not set to. The task bar will need to have it&#39;s own rules defined as it is a window which does not share the behaviour of normal windows.

_Dependencies with other requirements_

Need to implement a task bar in order for a start bar to open on it.

**3.7 Task Bar**

_Description_

The task bar will contain user selected programs that they can quickly access instead of having to go through the their whole list of programs from the startup menu, as well as displaying information useful to the user. It will be situated at the bottom of the screen by default, but will be able to be moved through the configuration. It will also contain a button for the startup menu. The task bar will also allow users to switch between workspaces and will hold background clients.

_Criticality_

Important as without it users will have a hard time accessing the startup menu if a screen is filled with windows and it allows users to customise their own shortcuts.

_Technical issues_

The task bar will need to have it&#39;s own rules defined as it is a window which does not share the behaviour of normal windows. It will also need to gather information from the system in order to display information such as the current time, resource usage, etc.

_Dependencies with other requirements_

The taskbar will reflect information about workspaces and the user will allow the user to switch between workspaces thus it requires workspace functionality to be implemented.

**3.8 Pin Windows**

_Description_

Pinning a window is when a user selects to keep a window &quot;stuck&quot; or in other words pinned to a part of the screen, and the window will stay in that position when switching between workspaces. They will be able to do it both by clicking on a &quot;pin&quot; button or by keyboard hotkey.

_Criticality_

This is not essential, but a very nice function to have as it allows users further enhance their workspaces and improve workflow.

_Technical issues_

Ensuring the window switches over to every workspace and does not interact with already open windows that it appears near.

_Dependencies with other requirements_

Will need workspaces to be implemented.

**3.9 Statistics**

_Description_

The window manager will store simple statistics about things such as how long the window manager has been in use, how many times a user opened a program, how long each program has been used for and other relevant statistics. They will also be used to figure out the users most used programs in the startup menu.

_Criticality_

Not essential or something that would be missed by users.

_Technical issues_

Have to make sure all statistics are accurate and reliable. Will need to persist through numerous runs of the program.

_Dependencies with other requirements_

None.

**3.10 Cheat Sheet**

_Description_

There will be  a cheat sheet included, accessible by a simple keyboard button combination that will temporarily pop up on the screen showing the most useful/important keyboard shortcuts.

_Criticality_

Not essential at all but will be a nice feature to have for beginner users incase they forget any common shortcuts but don&#39;t want to look in the manual to find them.

_Technical issues_

Ensuring that it is temporary and does not stay on screen.

_Dependencies with other requirements_

None.

**4. System Architecture**

[System Architecture](REAL System Architecture.png)

The system revolves around the X server, which is the System which actually controls client/applications on the user&#39;s display. The X server sends information about Windows currently on the screen and the Window manager in turn responds with requests on how Windows Should be handled. Applications Send information on how they would like to be handled to the X Server which sends the information to the Window Manager, though these requests do not have to be fulfilled. Components such as the task bar and menu are separate programs that will come packaged with the Window Manager, and also interact With the Window Manager through the X Server. The X Server is the also the System which writes information to the display. The X Server also receives input from the mouse and keyboard, which is relayed to the Window Manager, which is also responsible for deciding how to handle this input.

**5. High Level Design**

**5.1 LDS for Window Manager System**

[LDS For System](LDS.png) 

The _User_ inputs data(via keyboard or mouse) to the _Window Manager_, which waits to receive data from the user. Applications which run on the _Display_ send requests to the _Window Manager_ as to how they would like to be shown on the _Display_, but the _Window Manager_ is not required to honor these requests and can manage the _Application Window_ however it chooses. The _Window Manager_ writes the desired bitmap to the _Display_ which illustrates the changes to the user. _Window Managers_ must be prepared to interact with multiple displays in case of multi monitor setups or users using multiple terminals(e.g. via SSH). The graph is actually slightly false, as the display does in fact send information to the window manager; at start up the Display will send information about itself to the Window Manager(e.g. it&#39;s resolution) though this exchange only occurs once at startup so it isn&#39;t included.

**6. Preliminary Schedule**

[Smartsheet Project](Smartsheet Project (2).png)