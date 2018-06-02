asw (Analog Stick Wheel)

Asw turns the thumbstick of an Xbox controller (or any other XInput compatible controller) into a wheel-like control. This can allow for more precise control in racing games.

=========
Overview:
=========
Asw is a vJoy feeder and needs vJoy to work. vJoy is a device driver that adds a virtual game controller to the system.
Asw does 2 things.

1. Asw reads the rotational movement of the left thumbstick on the Xbox controller (or any other XInput compatible controller) and converts it to a linear value that it sends (feeds) to a vJoy axis. Or you can say that it creates an axis (on the vJoy device) that responds to the rotation of the controller analog stick.
Basically, it makes the thumbstick work kind of like a wheel.

2. Asw also reads the left and right triggers and feeds the values to vJoy.
This is useful because when the Xbox controller is used in DirectInput mode, the left and right triggers share an axis. This function allows you to have independent inputs from the triggers present on the vJoy device (each trigger will have it's own axis).

Asw was made to improve the level of precision offered by the Xbox controller in racing games.

====================
System requirements:
====================
Windows 7 or later (only tested in Win 7 and 10)
Xbox controller or any other XInput compatible controller
vJoy (tested with version v2.1.8)

=======================
How to install and use:
=======================
First, download asw from asw's github page. There are two ways you can achieve this.
Method 1) Download the entire source code package using the green download button on project’s main page (https://github.com/aswdevz/asw), all that you need to run asw will be inside the "Release" directory. 
Method 2) Download the package from the releases section (https://github.com/aswdevz/asw/releases), this will contain just the program and documentation.

Then install vJoy (vJoy is required to run asw), you can get it from here:
http://vJoystick.sourceforge.net/site/index.php/download-a-install/download
Check that vJoy has at least one virtual device using the "Configure vJoy" or "Monitor vJoy" tools (these are included with vJoy). This device should have at least these 3 controls: x axis, slider 0, slider 1. It can have more, but if one of the previously mentioned controls is missing, asw will lose the corresponding functionality. The device should be created automatically when you install vJoy. But if it's not present for some reason, create it. 
vJoy devices have numbers from 1-16. Asw will try to use the device numbered 1. You can change this behavior using the -interface command line argument.
Note: the 3 controls this feeder uses can be changed with command line arguments that are explained later in this file.

Next, run asw (asw.exe).
Asw will start working (feed inputs to vJoy) right after it's launched. You can confirm that asw is working by checking the last message in the command window, the message should be: "asw version x.x successfully started, press esc to quit".
In case asw does not start properly, try launching it from the command line so you can see the program output. The output may explain what the problem is.
You should now be able to assign asw's controls in your racing game and use them.
A guide showing how to make asw work with Assetto Corsa and Project Cars is available in the doc directory. Screenshots showing the final control configuration for these two games are also present.
Asw should work with other games as well, but only these two were tested.
The recommended way to use the controller with asw is: push the left thumbstick to the edge with your thumb, then move it along the edge.
Pressing the back button and Y button together will reset the wheel to center.
Pressing the Escape key exits asw.
You can adjust how asw works using command line arguments, explained below.

====================================
Command line arguments for asw.exe:
====================================
-interface or -i
sets the vJoy interface to use
valid vJoy interface numbers are 1-16
default is 1
example: -i 3

-controller or -c
sets the controller to use
XInput controllers are numbered 1-4 (they are actually numbered 0-3 in the code)
default is 1 (or 0 in the code)
example: -c 2

-deadzone or -d
sets the deadzone size to use for the thumbstick input in percent
valid deadzone setting is 0-100
default is 15
example: -d 5

-rotation or -r
sets the maximum number of degrees the virtual wheel can be rotated to (or wheel lock)
default is 900
example: -r 720

-bindmode or -b
Bindmode is used to assign (bind) an axis in game when it can't be done by usual means. You see, when you use this program to feed vJoy and use the controller, the system will register inputs from the vJoy virtual controller you created and the XInput controller at the same time. This means that the game will probably see inputs from both of these controllers which can mean you won't be able to assign the right one when the game prompts you for controller input while assign the control. To get past this, use this argument followed by a 1,2 or 3.
1 to make asw generate movement in the virtual wheel axis
2 to make asw generate movement in the left trigger
3 to make asw generate movement in the right trigger
That way you should be able to bind the control if it doesn't work normally.
example:
-b 2

-anykey or -a
when this is present, any key press will exit the program
if this is not present, only pressing the Escape key will exit the program
example: -a

================================
ADVANCED command line arguments:
================================
-bindmodeincrement or -bmi
Adjust the speed of movement generated by bindmode.
You probably won't need to touch this.
defaults is 128
Higher values mean faster movement
Smaller values mean slower movement
A value of 128 will make the control move from end position to end position in about 2 seconds

-bindmodemovementdirection or -bmmd
Adjusts the direction of movement generated by bindmode.
Accepts -1 and 1 only. 1 means forward direction, -1 means reverse direction.
example: -bmnd -1

-bindmodenonreversible or -bmnr
When this is used, bindmode will move the axis in one direction only. The normal behavior is reversing the direction when end of travel is reached.
example: -bmnr

-bindmoderesetwait or -bmrw
When bindmode moves the axis to the end of it's travel, bindmode will stop and wait for a bit before it starts moving the axis again. This option can set this delay. The delay is set in cycles. The default value is 125 cycles and the default length of a cycle is 8ms so the default delay is roughly 125*8ms = 1000ms = 1 second.
example: -bmrw 250
note: the above command will double the usual delay

-wheelresetbuttonsflag or -wrbf
Change the button or button combination that has to be pressed to reset the wheel to center.
This argument accepts a hexadecimal value only.
Default is: 0x8020 (back and Y buttons)
Use one of the values from the following table:
0x0001  XInput_GAMEPAD_DPAD_UP
0x0002  XInput_GAMEPAD_DPAD_DOWN
0x0004  XInput_GAMEPAD_DPAD_LEFT
0x0008  XInput_GAMEPAD_DPAD_RIGHT
0x0010  XInput_GAMEPAD_START
0x0020  XInput_GAMEPAD_BACK
0x0040  XInput_GAMEPAD_LEFT_THUMB
0x0080  XInput_GAMEPAD_RIGHT_THUMB
0x0100  XInput_GAMEPAD_LEFT_SHOULDER
0x0200  XInput_GAMEPAD_RIGHT_SHOULDER
0x1000  XInput_GAMEPAD_A
0x2000  XInput_GAMEPAD_B
0x4000  XInput_GAMEPAD_X
0x8000  XInput_GAMEPAD_Y
Or, if you want the reset to be triggered by pressing a combination of buttons, just add
the values together (using hexadecimal addition) and pass the result to this option.
example:
-wrbf C000
note: the above will make the reset happen when X and Y buttons are pressed at the same time
this is because 0x4000 + 0x8000 = 0xC000
note: the value can be passed in with the "0x" in front of the value or without it

-disablewheelreset or -dwr
The wheel will no longer reset to center if the assigned button combination is pressed.

-tickdelay or -t
Sets how long to wait (in milliseconds) between consecutive vJoy device updates.
This basically sets the delay in milliseconds between each tick/cycle (each time the main loop is run).
Setting this to 8 means that the vJoy device will get updated 1000/8 = 125 times a second.
Setting this to 25 will mean that the vJoy device will get updated 1000/25 = 40 times a second.
The default value is 8. The minimum allowed value is 1 and the maximum allowed value is 1000.
You should probably keep this at the default value.
example:
-t 17

-rightstick or -rs
Use the right thumb stick on your controller for the virtual wheel input instead of the left thumb stick.
example:
-rs

-wheelaxis or -wa
Sets the vJoy device axis that should be used as output for the virtual wheel.
Enter any value that vJoy recognizes as an axis identifier. The value should be a hexadecimal integer.
The recognized axis identifiers are at the end of this file.
Default is: 0x30
exaple 1:
-wa 0x31
exaple 2:
-wa 31
note: both examples will result in the same behavior

-lefttriggeraxis or -lta
Sets the vJoy device axis that should be used as output for the left trigger.
Enter any value that vJoy recognizes as an axis identifier. The value should be a hexadecimal integer.
The recognized axis identifiers are at the end of this file.
Default is: 0x36
exaple 1:
-wa 0x32
exaple 2:
-wa 32
note: both examples will result in the same behavior

-righttriggeraxis or -rta
Sets the vJoy device axis that should be used as output for the right trigger.
Enter any value that vJoy recognizes as an axis identifier. The value should be a hexadecimal integer.
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

-invertrighttrigger or -irt
invert right trigger output

-help or -h
display help text

-version or -v
display program version

========================================
The axis identifiers in vJoy v2.1.8 are:
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

=========================
Repository url + contact:
=========================
https://github.com/aswdevz/asw
aswdevz@gmail.com
