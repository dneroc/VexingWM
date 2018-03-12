#!/bin/bash
#Ensure there are minimal windows open when running this test script

#Count for test passes
count=0

#Move the terminal incase it's in the top left corner
xdotool mousemove 100 10
xdotool mousedown 1
xdotool mousemove 300 300
xdotool mouseup 1

#Remove up to five windows in the top left corner
xdotool mousemove 10 10
xdotool click --repeat 5 1

#Get root window number
output=$(xdotool getmouselocation)
rootWindow=$(echo $output | awk '{print $4}' | cut -d":" -f2)

#Test1: Open terminal(alt + Return)
xdotool key alt+Return
sleep 0.2
xdotool mousemove 100 10
output=$(xdotool getmouselocation)
terminal=$(echo $output | awk '{print $4}' | cut -d":" -f2)
if [ "$rootWindow" != "$terminal" ]
then
	count=$((countx + 1))
else
	echo "Test1: Terminal open test fail"
fi

xdotool mousemove 10 10
xdotool click 1

#Test2: Exit button(left click)
xdotool key alt+Return
sleep 0.2
xdotool mousemove 10 10
xdotool click 1
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
if [ "$rootWindow" == "$window" ]
then
	count=$((count + 1))
else
	echo "Test2: Exit button test fail"
fi

#Test3: Move window(left click title)
xdotool key alt+Return
sleep 0.1
xdotool mousemove 100 10
xdotool mousedown 1
xdotool mousemove 200 200
xdotool mouseup 1
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowLocation=$(echo $output | awk '{print $4}' | cut -d":" -f2)

if [ "$windowLocation" == "100,190" ]
then
	count=$((count + 1))
else
	echo "Test3: Moving window test fail. Should be 100,190, is: $windowLocation"
fi

xdotool mousemove_relative -- -90 0
xdotool click 1

#Test4: Resize window (from exit button)
xdotool key alt+Return
sleep 0.1
xdotool mousemove 10 10
xdotool keydown alt
xdotool mousedown 3
xdotool mousemove 200 200
xdotool keyup alt
xdotool mouseup 3
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowSize=$(echo $output | awk '{print $8}' | cut -d":" -f2)

if [ "$windowSize" == "674x526" ]
then
	count=$((count + 1))
else
	echo "Test4: Window resizing failed. Should be 674x526, is: $windowSize"
fi

xdotool mousemove 10 10
xdotool click 1

#Test5: Resize window (from maximise button)
xdotool key alt+Return
sleep 0.1
xdotool mousemove 30 10
xdotool keydown alt
xdotool mousedown 3
xdotool mousemove 200 200
xdotool keyup alt
xdotool mouseup 3
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowSize=$(echo $output | awk '{print $8}' | cut -d":" -f2)

if [ "$windowSize" == "654x526" ]
then
	count=$((count + 1))
else
	echo "Test5: Window resizing failed. Should be 654x526, is: $windowSize"
fi

xdotool mousemove 10 10
xdotool click 1

#Test6: Resize window (from title bar)
xdotool key alt+Return
sleep 0.1
xdotool mousemove 100 10
xdotool keydown alt
xdotool mousedown 3
xdotool mousemove 200 200
xdotool keyup alt
xdotool mouseup 3
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowSize=$(echo $output | awk '{print $8}' | cut -d":" -f2)

if [ "$windowSize" == "584x526" ]
then
	count=$((count + 1))
else
	echo "Test6: Window resizing failed. Should be 654x526, is: $windowSize"
fi

xdotool mousemove 10 10
xdotool click 1

#Test7: Resize window (from client window)
xdotool key alt+Return
sleep 0.1
xdotool mousemove 100 100
xdotool keydown alt
xdotool mousedown 3
xdotool mousemove 200 200
xdotool keyup alt
xdotool mouseup 3
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowSize=$(echo $output | awk '{print $8}' | cut -d":" -f2)

if [ "$windowSize" == "584x436" ]
then
	count=$((count + 1))
else
	echo "Test7: Window resizing failed. Should be 654x526, is: $windowSize"
fi

xdotool mousemove 10 10
xdotool click 1

#Test8: Maximise button (left click)
xdotool key alt+Return
sleep 0.1
xdotool mousemove 30 10
xdotool click 1
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowSizeX=$(echo $output | awk '{print $8}' | cut -d"x" -f1)
windowSizeY=$(echo $output | awk '{print $8}' | cut -d"x" -f2)
output=$(xdotool getwindowgeometry $rootWindow)
rootSizeX=$(echo $output | awk '{print $8}' | cut -d"x" -f1)
rootSizeY=$(echo $output | awk '{print $8}' | cut -d"x" -f2)
#Border is size 4 so window is 4 less than the root
windowSizeX=$((windowSizeX + 4))
windowSizeY=$((windowSizeY + 4))
if [ $windowSizeX == $rootSizeX ] && [ $windowSizeY == $rootSizeY ]
then
	count=$((count + 1))
else
	echo "Test8: Window resizing failed. Should be $rootSizeX x $rootSizeY, is: $windowSizeX x $windowSizeY"
fi

xdotool mousemove 10 10
xdotool click 1

#Test9: Open Dmenu
xdotool mousemove 5 5
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
xdotool key alt+F2
sleep 0.2
output=$(xdotool getmouselocation)
dmenu=$(echo $output | awk '{print $4}' | cut -d":" -f2)

if [ $window != $dmenu ]
then
	count=$((count + 1))
else
	echo "Test9: Open dmenu failed. Background window: $window should not equal dmenu window: $dmenu "
fi

xdotool key Return

#Test10: Open program from dmenu
xdotool key alt+F2
sleep 0.5
xdotool type firefox
xdotool key Return
sleep 2
output=$(xdotool search --class firefox)

if [ -z "$output" ]
then
	echo "Test10: Open program from dmenu failed. Firefox not open"
else
	count=$((count + 1))
fi

xdotool mousemove 10 10
xdotool click 1

#Test11: Maximise with Alt+Up
xdotool key alt+Return
sleep 0.1
xdotool mousemove 30 10
xdotool click 1
xdotool key alt+Up
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowSizeX=$(echo $output | awk '{print $8}' | cut -d"x" -f1)
windowSizeY=$(echo $output | awk '{print $8}' | cut -d"x" -f2)
output=$(xdotool getwindowgeometry $rootWindow)
rootSizeX=$(echo $output | awk '{print $8}' | cut -d"x" -f1)
rootSizeY=$(echo $output | awk '{print $8}' | cut -d"x" -f2)
windowSizeX=$((windowSizeX + 4))
windowSizeY=$((windowSizeY + 4))

if [ $windowSizeX == $rootSizeX ] && [ $windowSizeY == $rootSizeY ]
then
        count=$((count + 1))	
else
	echo "Test1: Window resizing failed. Should be $rootSizeX x $rootSizeY, is: $windowSizeX x $windowSizeY"
fi

xdotool mousemove 10 10
xdotool click 1

#Test12: Move to left side of screen ALt+Left
xdotool key alt+Return
sleep 0.1
xdotool mousemove 30 10
xdotool click 1
xdotool key alt+Left
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowSizeX=$(echo $output | awk '{print $8}' | cut -d"x" -f1)
windowSizeY=$(echo $output | awk '{print $8}' | cut -d"x" -f2)
output=$(xdotool getwindowgeometry $rootWindow)
rootSizeX=$(echo $output | awk '{print $8}' | cut -d"x" -f1)
rootSizeY=$(echo $output | awk '{print $8}' | cut -d"x" -f2)
windowSizeX=$((windowSizeX))
windowSizeY=$((windowSizeY + 4))
halfRootX=$((rootSizeX / 2))
halfRootY=$((rootSizeY / 2))

if [ $windowSizeX == $halfRootX ] && [ $windowSizeY == $rootSizeY ]
then
        count=$((count + 1))
else
    echo "Test2: Window resizing failed. Should be $halfRootX x $rootSizeY, is: $windowSizeX x $windowSizeY"
fi

xdotool mousemove 10 10
xdotool click 1

#Test13: Move to right side of screen ALt+Right
xdotool key alt+Return
sleep 0.1
xdotool mousemove 30 10
xdotool click 1
xdotool key alt+Right
xdotool mousemove 750 30
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowSizeX=$(echo $output | awk '{print $8}' | cut -d"x" -f1)
windowSizeY=$(echo $output | awk '{print $8}' | cut -d"x" -f2)
output=$(xdotool getwindowgeometry $rootWindow)
rootSizeX=$(echo $output | awk '{print $8}' | cut -d"x" -f1)
rootSizeY=$(echo $output | awk '{print $8}' | cut -d"x" -f2)
windowSizeX=$((windowSizeX+4))
windowSizeY=$((windowSizeY + 4))
halfRootX=$((rootSizeX / 2))
halfRootY=$((rootSizeY / 2))
if [ $windowSizeX == $halfRootX ] && [ $windowSizeY == $rootSizeY ]
then
        count=$((count + 1))
else
    echo "Test3: Window resizing failed. Should be $halfRootX x $rootSizeY, is: $windowSizeX x $windowSizeY"
fi

xdotool mousemove $halfRootX 10
xdotool mousemove_relative 10 0
xdotool click 1

#Test14: Close window with Alt+F4
xdotool key alt+Return
xdotool mousemove 100 100
xdotool keydown alt
xdotool click 1
xdotool keyup alt
output=$(xdotool getmouselocation)
window1=$(echo $output | awk '{print $4}' | cut -d":" -f2)
sleep 0.2
xdotool key alt+F4
output=$(xdotool getmouselocation)
window2=$(echo $output | awk '{print $4}' | cut -d":" -f2)

if [ $window1 != $window2 ]
then
        count=$((count + 1))
else
    echo "Test14: Alt+F4 failed. Should be window1: $window1 should not equal window2: $window2"
fi

#Test15: Open 100 terminals
xdotool key --repeat 100 alt+Return
sleep 0.5
count=$((count + 1))

#Test16: Close 100 terminals
xdotool mousemove 10 10
xdotool click --repeat 100 1
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)

if [ $window == $rootWindow ]
then
        count=$((count + 1))
else
    echo "Test16: Close 100 terminals failed. Should be window1: $window equals window2: $rootWindow"
fi

#Test17: Raise window (left click title)
xdotool key alt+Return
sleep 0.5
xdotool mousemove 100 10
xdotool mousedown 1
xdotool mousemove_relative 100 0
xdotool mouseup 1
xdotool key alt+Return
output=$(xdotool getmouselocation)
window1=$(echo $output | awk '{print $4}' | cut -d":" -f2)
xdotool mousemove_relative 300 0
xdotool click 1
xdotool mousemove_relative -- -300 0
output=$(xdotool getmouselocation)
window2=$(echo $output | awk '{print $4}' | cut -d":" -f2)

if [ $window1 != $window2 ]
then
        count=$((count + 1))
else
    echo "Test17: Left click raise failed. Window1: $window1 should != window2: $window2"
fi

xdotool mousemove 10 10
sleep 0.1
xdotool click 1
xdotool mousemove 110 10
sleep 0.1
xdotool click 1

#Test18: Raise window (alt+left click client)
xdotool key alt+Return
sleep 0.5
xdotool mousemove 100 10
xdotool mousedown 1
xdotool mousemove_relative 100 0
xdotool mouseup 1
xdotool key alt+Return
xdotool mousemove_relative 0 100
xdotool keydown alt
xdotool click 1
xdotool keyup alt
output=$(xdotool getmouselocation)
window1=$(echo $output | awk '{print $4}' | cut -d":" -f2)
xdotool mousemove_relative 300 0
xdotool keydown alt
xdotool click 1
xdotool keyup alt
xdotool mousemove_relative -- -300 0
output=$(xdotool getmouselocation)
window2=$(echo $output | awk '{print $4}' | cut -d":" -f2)


if [ $window1 != $window2 ]
then
        count=$((count + 1))
else
    echo "Test18: alt + Left click raise failed. Window1: $window1 should != window2: $window2"
fi

xdotool mousemove 10 10
sleep 0.1
xdotool click 1
xdotool mousemove 110 10
sleep 0.1
xdotool click 1

#Test19: Raise window (alt+right click client)
xdotool key alt+Return
sleep 0.5
xdotool mousemove 100 10
xdotool mousedown 1
xdotool mousemove_relative 100 0
xdotool mouseup 1
xdotool key alt+Return
sleep 0.2
xdotool mousemove_relative 0 100
output=$(xdotool getmouselocation)
window1=$(echo $output | awk '{print $4}' | cut -d":" -f2)
xdotool mousemove_relative 300 0
xdotool keydown alt
xdotool click 3
xdotool keyup alt
xdotool mousemove_relative -- -300 0
output=$(xdotool getmouselocation)
window2=$(echo $output | awk '{print $4}' | cut -d":" -f2)

if [ $window1 != $window2 ]
then
        count=$((count + 1))
else
    echo "Test19: alt + Right click raise failed. Window1: $window1 should != window2: $window2"
fi

xdotool mousemove 10 10
sleep 0.1
xdotool click 1
xdotool mousemove 110 10
sleep 0.1
xdotool click 1

#Test20: Move window(left click client)
xdotool key alt+Return
sleep 0.2
xdotool mousemove 100 100
xdotool mousedown 1
xdotool mousemove 200 200
xdotool mouseup 1
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowLocation=$(echo $output | awk '{print $4}' | cut -d":" -f2)

if [ "$windowLocation" == "0,0" ]
then
	count=$((count + 1))
else
	echo "Test20: Moving window test fail. Should be 0,0, is: $windowLocation"
fi

xdotool mousemove 10 10
xdotool click 1

#Test21: Exit button(right click)
xdotool key alt+Return
sleep 0.2
xdotool mousemove 10 10
output=$(xdotool getmouselocation)
window1=$(echo $output | awk '{print $4}' | cut -d":" -f2)
xdotool click 3
output=$(xdotool getmouselocation)
window2=$(echo $output | awk '{print $4}' | cut -d":" -f2)
if [ "$window1" == "$window2" ]
then
	count=$((count + 1))
else
	echo "Test21: Exit button test fail"
fi

xdotool click 1

#Test22: Exit button(alt + left click)
xdotool key alt+Return
sleep 0.2
xdotool mousemove 10 10
xdotool keydown alt
xdotool click 1
xdotool keyup alt
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
if [ "$rootWindow" == "$window" ]
then
	count=$((count + 1))
else
	echo "Test22: Exit button test fail"
fi

#Test23: Exit button(alt + right click)
xdotool key alt+Return
sleep 0.2
xdotool mousemove 10 10
xdotool keydown alt
xdotool click 3
xdotool keyup alt
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
if [ "$rootWindow" != "$window" ]
then
	count=$((count + 1))
else
	echo "Test23: Exit button test fail"
fi

xdotool mousemove 10 10
xdotool click 1

#Test24: Open terminal(dmenu)
xdotool key alt+F2
sleep 0.2
xdotool type xterm
sleep 0.2
xdotool key Return
sleep 0.2
xdotool mousemove 100 10
output=$(xdotool getmouselocation)
terminal=$(echo $output | awk '{print $4}' | cut -d":" -f2)
if [ "$rootWindow" != "$terminal" ]
then
	count=$((count + 1))
else
	echo "Test24: Terminal open test fail"
fi

xdotool mousemove 10 10
xdotool click 1

#Test25: Open terminal(terminal)
xdotool key alt+Return
xdotool mousemove 100 100
xdotool keydown alt
xdotool click 1
xdotool keyup alt
output=$(xdotool getmouselocation)
terminal1=$(echo $output | awk '{print $4}' | cut -d":" -f2)
xdotool type xterm
xdotool key Return
sleep 0.2
output=$(xdotool getmouselocation)
terminal2=$(echo $output | awk '{print $4}' | cut -d":" -f2)
if [ "$terminal" != "$terminal2" ]
then
	count=$((count + 1))
else
	echo "Test25: Terminal open test fail"
fi

xdotool mousemove 10 10
xdotool click --repeat 2 1

#Test26: Maximise button (alt-left click)
xdotool key alt+Return
sleep 0.1
xdotool mousemove 30 10
xdotool keydown alt
xdotool click 1
xdotool keyup alt
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowSizeX=$(echo $output | awk '{print $8}' | cut -d"x" -f1)
windowSizeY=$(echo $output | awk '{print $8}' | cut -d"x" -f2)
output=$(xdotool getwindowgeometry $rootWindow)
rootSizeX=$(echo $output | awk '{print $8}' | cut -d"x" -f1)
rootSizeY=$(echo $output | awk '{print $8}' | cut -d"x" -f2)
#Border is size 4 so window is 4 less than the root
windowSizeX=$((windowSizeX + 4))
windowSizeY=$((windowSizeY + 4))
if [ $windowSizeX == $rootSizeX ] && [ $windowSizeY == $rootSizeY ]
then
	count=$((count + 1))
else
	echo "Test26: Window resizing failed. Should be $rootSizeX x $rootSizeY, is: $windowSizeX x $windowSizeY"
fi

xdotool mousemove 10 10
xdotool click 1

#Test27: Maximise button (right click)
xdotool key alt+Return
sleep 0.1
xdotool mousemove 30 10
xdotool click 3
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowSizeX=$(echo $output | awk '{print $8}' | cut -d"x" -f1)
windowSizeY=$(echo $output | awk '{print $8}' | cut -d"x" -f2)
output=$(xdotool getwindowgeometry $rootWindow)
rootSizeX=$(echo $output | awk '{print $8}' | cut -d"x" -f1)
rootSizeY=$(echo $output | awk '{print $8}' | cut -d"x" -f2)
#Border is size 4 so window is 4 less than the root
windowSizeX=$((windowSizeX + 4))
windowSizeY=$((windowSizeY + 4))
if [ $windowSizeX != $rootSizeX ] && [ $windowSizeY != $rootSizeY ]
then
	count=$((count + 1))
else
	echo "Test27: Window resizing failed. Should not be $rootSizeX x $rootSizeY, is: $windowSizeX x $windowSizeY"
fi

xdotool mousemove 10 10
xdotool click 1

#Test28: Maximise button (alt-right click)
xdotool key alt+Return
sleep 0.1
xdotool mousemove 30 10
xdotool keydown alt
xdotool click 3
xdotool keyup alt
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowSizeX=$(echo $output | awk '{print $8}' | cut -d"x" -f1)
windowSizeY=$(echo $output | awk '{print $8}' | cut -d"x" -f2)
output=$(xdotool getwindowgeometry $rootWindow)
rootSizeX=$(echo $output | awk '{print $8}' | cut -d"x" -f1)
rootSizeY=$(echo $output | awk '{print $8}' | cut -d"x" -f2)
#Border is size 4 so window is 4 less than the root
windowSizeX=$((windowSizeX + 4))
windowSizeY=$((windowSizeY + 4))
if [ $windowSizeX != $rootSizeX ] && [ $windowSizeY != $rootSizeY ]
then
	count=$((count + 1))
else
	echo "Test28: Window resizing failed. Should not be $rootSizeX x $rootSizeY, is: $windowSizeX x $windowSizeY"
fi

xdotool mousemove 10 10
xdotool click 1

#Test29: Raise window (left click client)
xdotool key alt+Return
sleep 0.5
xdotool mousemove 100 10
xdotool mousedown 1
xdotool mousemove_relative 100 0
xdotool mouseup 1
xdotool key alt+Return
xdotool mousemove_relative 0 100
xdotool click 1
output=$(xdotool getmouselocation)
window1=$(echo $output | awk '{print $4}' | cut -d":" -f2)
xdotool mousemove_relative 300 0
xdotool click 1
xdotool mousemove_relative -- -300 0
output=$(xdotool getmouselocation)
window2=$(echo $output | awk '{print $4}' | cut -d":" -f2)


if [ $window1 == $window2 ]
then
        count=$((count + 1))
else
    echo "Test29: Left click raise test failed. Window1: $window1 should == window2: $window2"
fi

xdotool mousemove 10 10
sleep 0.1
xdotool click 1
xdotool mousemove 110 10
sleep 0.1
xdotool click 1

#Test30: Raise window (right click client)
xdotool key alt+Return
sleep 0.5
xdotool mousemove 100 10
xdotool mousedown 1
xdotool mousemove_relative 100 0
xdotool mouseup 1
xdotool key alt+Return
xdotool mousemove_relative 0 100
xdotool click 3
output=$(xdotool getmouselocation)
window1=$(echo $output | awk '{print $4}' | cut -d":" -f2)
xdotool mousemove_relative 300 0
xdotool click 3
xdotool mousemove_relative -- -300 0
output=$(xdotool getmouselocation)
window2=$(echo $output | awk '{print $4}' | cut -d":" -f2)


if [ $window1 == $window2 ]
then
        count=$((count + 1))
else
    echo "Test30: Right click raise test failed. Window1: $window1 should == window2: $window2"
fi

xdotool mousemove 10 10
sleep 0.1
xdotool click 1
xdotool mousemove 110 10
sleep 0.1
xdotool click 1

#Test31: Raise window (right click title)
xdotool key alt+Return
sleep 0.5
xdotool mousemove 100 10
xdotool mousedown 1
xdotool mousemove_relative 100 0
xdotool mouseup 1
xdotool key alt+Return
sleep 0.5
output=$(xdotool getmouselocation)
window1=$(echo $output | awk '{print $4}' | cut -d":" -f2)
xdotool mousemove_relative 300 0
xdotool click 3
xdotool mousemove_relative -- -300 0
output=$(xdotool getmouselocation)
window2=$(echo $output | awk '{print $4}' | cut -d":" -f2)

if [ $window1 == $window2 ]
then
        count=$((count + 1))
else
    echo "Test31: Right click raise failed. Window1: $window1 should == window2: $window2"
fi

xdotool mousemove 10 10
sleep 0.1
xdotool click 1
xdotool mousemove 110 10
sleep 0.1
xdotool click 1

#Test32: Raise window (alt+right click title)
xdotool key alt+Return
sleep 0.5
xdotool mousemove 100 10
xdotool mousedown 1
xdotool mousemove_relative 100 0
xdotool mouseup 1
xdotool key alt+Return
output=$(xdotool getmouselocation)
window1=$(echo $output | awk '{print $4}' | cut -d":" -f2)
xdotool mousemove_relative 300 0
xdotool keydown alt
xdotool click 3
xdotool keyup alt
xdotool mousemove_relative -- -300 0
output=$(xdotool getmouselocation)
window2=$(echo $output | awk '{print $4}' | cut -d":" -f2)

if [ $window1 != $window2 ]
then
        count=$((count + 1))
else
    echo "Test32: Alt-right click raise failed. Window1: $window1 should != window2: $window2"
fi

xdotool mousemove 10 10
sleep 0.1
xdotool click 1
xdotool mousemove 110 10
sleep 0.1
xdotool click 1

#Test33: Raise window (alt+left click title)
xdotool key alt+Return
sleep 0.5
xdotool mousemove 100 10
xdotool mousedown 1
xdotool mousemove_relative 100 0
xdotool mouseup 1
xdotool key alt+Return
sleep 0.5
output=$(xdotool getmouselocation)
window1=$(echo $output | awk '{print $4}' | cut -d":" -f2)
xdotool mousemove_relative 300 0
xdotool keydown alt
xdotool click 1
xdotool keyup alt
xdotool mousemove_relative -- -300 0
output=$(xdotool getmouselocation)
window2=$(echo $output | awk '{print $4}' | cut -d":" -f2)

if [ $window1 != $window2 ]
then
        count=$((count + 1))
else
    echo "Test33: Alt-right click raise failed. Window1: $window1 should != window2: $window2"
fi

xdotool mousemove 10 10
sleep 0.1
xdotool click 1
xdotool mousemove 110 10
sleep 0.1
xdotool click 1

#Test34: Try Resize window (from exit button, no alt)
xdotool key alt+Return
sleep 0.1
xdotool mousemove 10 10
xdotool mousedown 3
xdotool mousemove 200 200
xdotool mouseup 3
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowSize=$(echo $output | awk '{print $8}' | cut -d":" -f2)

if [ "$windowSize" != "674x526" ]
then
	count=$((count + 1))
else
	echo "Test34: Window resizing failed. Should not be 674x526, is: $windowSize"
fi

xdotool mousemove 10 10
xdotool click 1

#Test35: Try Resize window (from maximise button, no alt)
xdotool key alt+Return
sleep 0.1
xdotool mousemove 30 10
xdotool mousedown 3
xdotool mousemove 200 200
xdotool mouseup 3
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowSize=$(echo $output | awk '{print $8}' | cut -d":" -f2)

if [ "$windowSize" != "654x526" ]
then
	count=$((count + 1))
else
	echo "Test35: Window resizing failed. Should not be 654x526, is: $windowSize"
fi

xdotool mousemove 10 10
xdotool click 1

#Test36: Try Resize window (from title bar, no alt)
xdotool key alt+Return
sleep 0.1
xdotool mousemove 100 10
xdotool mousedown 3
xdotool mousemove 200 200
xdotool mouseup 3
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowSize=$(echo $output | awk '{print $8}' | cut -d":" -f2)

if [ "$windowSize" != "584x526" ]
then
	count=$((count + 1))
else
	echo "Test36: Window resizing failed. Should not be 654x526, is: $windowSize"
fi

xdotool mousemove 10 10
xdotool click 1

#Test37: Try Resize window (from client window, no alt)
xdotool key alt+Return
sleep 0.1
xdotool mousemove 100 100
xdotool mousedown 3
xdotool mousemove 200 200
xdotool mouseup 3
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
output=$(xdotool getwindowgeometry $window)
windowSize=$(echo $output | awk '{print $8}' | cut -d":" -f2)

if [ "$windowSize" != "584x436" ]
then
	count=$((count + 1))
else
	echo "Test37: Window resizing failed. Should not be 584x436, is: $windowSize"
fi

xdotool mousemove 10 10
xdotool click 1




echo "$count /37 tests passed"
