#!/bin/bash
#Ensure there are minimal windows open when running this test script

#Count for test passes
count=0

#Remove up to five windows in the top left corner
xdotool mousemove 10 10
xdotool click 1
xdotool click 1
xdotool click 1
xdotool click 1
xdotool click 1

#Get root window number
output=$(xdotool getmouselocation)
rootWindow=$(echo $output | awk '{print $4}' | cut -d":" -f2)

#Open terminal
xdotool key alt+Return
sleep 0.2

#Get terminal number
xdotool mousemove 100 10
output=$(xdotool getmouselocation)
terminal=$(echo $output | awk '{print $4}' | cut -d":" -f2)

#Test1: Check if terminal is open
if [ "$rootWindow" != "$terminal" ]
then
	count=$((count + 1))
else
	echo "Test1: Terminal open test fail"
fi

#Test2: Check exit button
xdotool mousemove 10 10
xdotool click 1
xdotool mousemove 100 10
output=$(xdotool getmouselocation)
window=$(echo $output | awk '{print $4}' | cut -d":" -f2)
if [ "$rootWindow" == "$window" ]
then
	count=$((count + 1))
else
	echo "Test2: Exit button test fail"
fi

#Test3: Move window
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
	xdotool mousemove_relative -- -90 0
	xdotool click 1
else
	echo "Test3: Moving window test fail. Should be 100,190, is: $windowLocation"
fi


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
	xdotool mousemove 10 10
	xdotool click 1
	count=$((count + 1))
else
	echo "Test4: Window resizing failed. Should be 674x526, is: $windowSize"
fi



echo "$count /4 tests passed"

sleep 500

#Open, move and close terminal
xdotool key alt+Return
sleep 0.5
xdotool mousemove 100 10
sleep 0.5
xdotool mousedown 1
xdotool mousemove 300 300 
xdotool mouseup 1
xdotool mousemove_relative -- -90 0
sleep 0.5
xdotool click 1


#Open Dmenu and firefox
xdotool key alt+F2
sleep 1
xdotool type 'firefox'
sleep 0.5
xdotool key Return
sleep 3
xdotool mousemove 10 10
sleep 1
xdotool click 1

#Alt tab switch
xdotool key alt+Return
sleep 5
xdotool type 'Old terminal'
sleep 5
xdotool key alt+Return
sleep 5
xdotool key alt+Tab



#Open and resize terminal on client
xdotool key alt+Return
sleep 0.5
xdotool mousemove 100 100
sleep 0.5 
xdotool keydown alt
xdotool mousedown 3
xdotool mousemove 300 300
sleep 0.5
xdotool keyup alt
xdotool mouseup 3
xdotool mousemove 10 10
sleep 0.5
xdotool click 1

#Open and resize terminal on title
xdotool key alt+Return
sleep 0.5
xdotool mousemove 100 10
sleep 1 
xdotool keydown alt
xdotool mousedown 3
xdotool mousemove 300 300
sleep 0.5
xdotool keyup alt
xdotool mouseup 3
xdotool mousemove 10 10
sleep 0.5
xdotool click 1

#Open and maximise terminal
xdotool key alt+Return
sleep 0.5
xdotool mousemove 30 10
xdotool click 1
sleep 0.5
xdotool mousemove 10 10
sleep 0.5
xdotool click 1


