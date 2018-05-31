// asw.h : this is the main header file of this program. I will include this
// into every source file. A lof of the header files included here can probably
// be moved into stdafx.h
//

#pragma once

#define VERSION_STRING "1.0.3rc5"
//#define EXTRA_LINES

#include <stdlib.h> // used for printf and maybe other things
#include <iostream> // just used for std::system("pause");

#include <windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

// the following 3 files are taken from vJoy218SDK-291116.zip
// this can be downloaded here: http://vjoystick.sourceforge.net/site/index.php/81-news/104-vjoy-2-1-5-sdk-released
#include <public.h> // needed for vjoy
#include <vjoyinterface.h> // needed for vjoy
#pragma comment(lib, "lib\\vJoyInterface.lib")

// the following 2 files are taken from: Microsoft Direct X SDK (June 2010) (DXSDK_Jun10.exe)
// this can be downloaded here: https://www.microsoft.com/en-us/download/details.aspx?id=6812
#include <Xinput.h> // needed to access the 360 controller through xinput
#pragma comment(lib, "lib\\XInput.lib")

struct s_aliaslist
{
	UINT* iInterface;
	DWORD* controller_number;
	int* thumbstick_deadzone;
	int* virtual_wheel_max_angle;
	int* bind_mode;
	bool* any_key_to_quit;
	int* bind_mode_increment;
	int* bind_mode_movement_direction;
	bool* bind_mode_non_reversible;
	int* bind_mode_reset_wait;
	int* wheel_reset_buttons_flag;
	bool* disable_wheel_reset;
	int* tick_delay;
	bool* use_right_stick;
	bool* help_detected;
	UINT* wheel_axis;
	UINT* left_trigger_axis;
	UINT* right_trigger_axis;
	LONG* invert_wheel_axis;
	LONG* invert_left_trigger;
	LONG* invert_right_trigger;
};