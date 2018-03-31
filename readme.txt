ASW (Analog Stick Wheel)
version 1.0.0
https://github.com/aswdevz/asw
contact: aswdevz@gmail.com

=========
Overview:
=========
Asw is a vjoy feeder and needs vjoy to work.
Asw does 2 things.

1. Asw reads the rotational movement of a thumbstick on the xbox controller (or any other xinput compatible controller) and converts it to a linear value that it sends (feeds) to a vjoy axis. Or you can say that it creates an axis (on the vjoy device) that responds to the rotation of the controller analog stick.
Basically it turns the thumbstick into a wheel-like control.
This function is useful because the controller input in racing games has little precision most of the time since it only responds to the left to right movement of the analog stick. Using asw, more precise control is possible.

2. Asw also reads the left and right triggers and feeds the values to vjoy.
This is useful because when the xbox controller is used in DirectInput mode, the left and right triggers share an axis and are not independent. This function allows you to have independent inputs from the triggers present on the vjoy device.

Feel free to try asw when you're dissatisfied with the level of precision offered by the xbox controller in racing games.

====================
system requirements:
====================
Windows 7 or later (only tested in Win 7 and 10)
xbox controller or any other xinput controller
vjoy (tested with version v2.1.8)

=======================
How to install and use:
=======================
First, download asw from asw's github page. There are two ways you can go achieve this.
Method 1) Download the entire source code package using the green download button on project’s main page (https://github.com/aswdevz/asw), all that you need to run asw will be inside the "Release" directory. 
Method 2) Download the package from the releases section (https://github.com/aswdevz/asw/releases), this will contain just the program and documentation. 

Then install vjoy (vjoy is required to run asw), you can get it from here:
http://vjoystick.sourceforge.net/site/index.php/download-a-install/download
Check that vjoy has at least one virtual device using the "configure vjoy" or "monitor vjoy" tools (these are included with vjoy). This device should have at least these 3 controls: x axis, slider 0, slider 1. It can have more, but if one of the previously mentioned controls is missing, this feeder will lose the corresponding functionality. The device should be created automatically when you install vjoy. But if it's not present for some reason, create it. 
Vjoy devices have numbers from 1-16. asw will try to use the device numbered 1. You can change this behavior using the -interface command line argument.
Note: the 3 controls this feeder uses can be changed with command line arguments that are explained later in this file.

Next, run asw (asw.exe).
Asw should start to function (feed inputs to vjoy) right after it's launched.
You should now be able to bind the controls in your racing game and use them.
Sample control setup for Assetto Corsa and Project Cars is available in the doc directory.
Pressing ESC quits asw.
In case something goes wrong and the window closes, try launching from the command line so you can see the program output. The output may explain what the problem is.
You can adjust how asw works using command line arguments, explained below.

====================================
Command line arguments for asw.exe:
====================================
-interface or -i
sets the vjoy interface to use
valid vjoy interface numbers are 1-16
default is 1
example: -i 3

-controller or -c
sets the controller to use
xinput controllers are numbered 1-4 (they are actually numbered 0-3 in the code)
default is 1 (or 0 in the code)
example: -c 2

-deadzone or -d
sets the deadzone size to use for the thumbstick input in percent
valid deadzone setting is 0-100
default is 10
example: -d 5

-rotation or -r
sets the maximum number of degrees the virtual wheel can be rotated to (or wheel lock)
default is 900
example: -r 720

-anykey or -a
when this is present, any key press will quit the program
if this is not present, onlt pressing escape will quit the program
example: -a

-bindmode or -b
Bindmode is used to bind an axis in game when it can't be done by usual means. You see, when you use this program to feed vjoy and use the controller, the system will register inputs from, the vjoy virtual controller you created and the xinput controller at the same time. This means that the game will probably see inputs from both of these controllers which can mean you won't be able to bind the right one when the game prompts you for controller input while binding the control. To get past this, use this argument followed by a 1,2 or 3.
1 to make asw generate movement in the virtual wheel axis
2 to make asw generate movement in the left trigger
3 to make asw generate movement in the right trigger
That way you should be able to bind the control if it doesn't work normally.
example:
-b 2

================================
ADVANCED command line arguments:
================================
-bindmodeincrement or -bmi
adjust the speed of the movement generated in bind mode functionality
You probably won't need to touch this.
defaults is 128
Higher values mean faster movement
Smaller values mean slower movement
A value of 128 will make the control move from end position to end position in about 2 seconds

-tickdelay or -t
Sets how long to wait (in milliseconds) between consecutive vjoy device updates.
This basically sets the delay in milliseconds between each tick (each time the main loop is run).
Setting this to 8 means that the vjoy device will get updated 1000/8 = 125 times a second.
Setting this to 20 will mean that the vjoy device will get updated 1000/25 = 40 times a second.
The default value is 8. The minimum allowed value is 1 and the maximum allowed value is 1000.\
You should probably keep this at the default value.
example:
-t 17

-rightstick or -rs
Use the right thumb stick on your controller for the virtual wheel input instead of the
left thumb stick.
example:
-rs

-wheelaxis or -wa
Sets the vjoy device axis that should be used as output for the virtual wheel.
Enter any value that vjoy recognizes as axis identifier a hexadecimal integer.
The recognized axis identifiers are at the end of this file.
Default is: 0x30
exaple 1:
-wa 0x31
exaple 2:
-wa 31
note: both examples will result in the same behavior

-lefttriggeraxis or -lta
Sets the vjoy device axis that should be used as output for the left trigger.
Enter any value that vjoy recognizes as axis identifier a hexadecimal integer.
The recognized axis identifiers are at the end of this file.
Default is: 0x36
exaple 1:
-wa 0x32
exaple 2:
-wa 32
note: both examples will result in the same behavior

-righttriggeraxis or -rta
Sets the vjoy device axis that should be used as output for the right trigger.
Enter any value that vjoy recognizes as axis identifier a hexadecimal integer.
The recognized axis identifiers are at the end of this file.
Default is: 0x37
exaple 1:
-wa 0x35
exaple 2:
-wa 35
note: both examples will result in the same behavior

-invertwheel or -iw
inverts virtual wheel axis output

-invertletfttrigger or -ilt
invert left trigger output
		{
-invertrighttrigger or -irt
invert right trigger output

-help or -h
display help text

-version or -v
display program version

========================================
The vjoy axis identifiers presently are:
========================================
Axis     Value
X        0x30
Y        0x31
Z        0x32
Rx       0x33
Ry       0x34
Rz       0x35
Slider0  0x36
Slider1  0x37
Wheel    0x38
POV      0x39
