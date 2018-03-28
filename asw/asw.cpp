// asw.cpp : Defines the entry point for the console application.
//

#include "stdafx.h" // for precompiled headers, must be present in every source file

#include "asw.h" // I'm including this in every source file
#include "xy_to_angle.h"
#include "cmdline.h"

// TODO:
// fix deadzone function //fixed-check - ok but 100 still registered movement at the edge
// add keyboard input (maybe later
// add functionality to disable virtual wheel and triggers (not needed, can be done by removing the axis from vjoy)
// tweak text output

int main(int argc, char* argv[])
{
	// variables needed for vjoy
	UINT iInterface = 1; // vjoy virtual joystick number
	
	// variables needed for xinput
	DWORD controller_number = 0; // xinput controller number [0-3]
	XINPUT_STATE controller_state; // this structure holds data indicating the state of all controls present on the controller
	ZeroMemory(&controller_state, (sizeof(controller_state)));
	int thumbstick_deadzone = 10;

	// variables needed for windows console input
	HANDLE console_input = GetStdHandle(STD_INPUT_HANDLE); // get a handle for the console input buffer
	INPUT_RECORD console_input_record; // create an INPUT_RECORD structure, this structure will hold one console event
	ZeroMemory(&console_input_record, (sizeof(console_input_record)));
	DWORD console_events_count = 0; // GetNumberOfConsoleInputEvents() stores the event count left in the console input buffer into this variable
	DWORD console_events_read = 0; // ReadConsoleInput() stores the number of console input events read into this variable, should always be 1 in my code

	// variables that hold information about the stick position and the position of the virtual wheel axis
	double raw_angle = 0.0; // angle calculated from thumbstick X and Y axis position, should be from -180.0 to 180.0
	int rotations = 0; // the number of times the stick rotated past the 180° position, positive = right, negative = left
	int steering_lock = 0; // 0 = no lock, -1 = locked at full left, 1 = locked at full right, this variable is not actually used in this version of the code
	double real_angle = 0.0; // virtual wheel angle caluclated by combining "raw_angle" and "rotations"
	double last_raw_angle = 0.0; // holds the angle from the previous tick
	int virtual_wheel_max_angle = 900; // sets a limit on how much can the virtual wheel be rotated to

	// variables needed for bind mode functionality
	int generated_movement = 1; // this stores the generated control position for the bind functionality
	int generated_movement_direction = 1; // this determines if the generated control position is increasing (1) or decreasing (-1) each tick
		
	// other variables needed for my code
	bool any_key_to_quit = false; // if this is set to true, pressing any key will quit the app, else only esc will quit the app
	int bind_mode = 0; // if bind_mode is set to 1, this program will generate and send positional changes to the vjoy virtual wheel axis
	                   // 2 is the same but for left trigger
					   // 3 is the same but for right right trigger
	                   // this is useful for binding the virtual controller in games
	int bind_mode_increment = 128; // this defines how much will the generated position change between every tick
	int tick_delay = 8; // sets the delay in miliseconds between each tick (main loop)
	bool use_right_stick = false; // if this is true, the right thumbstick will be used (if false, the left stick will be used)
	bool help_detected = false; // if this is true, the program will print the supported command line argument list and then quit

	// variables that define which vjoy device axis to use and for axis inversion
	UINT wheel_axis = HID_USAGE_X;
	UINT left_trigger_axis = HID_USAGE_SL0;
	UINT right_trigger_axis = HID_USAGE_SL1;
	LONG invert_wheel_axis = 0; // allowed values: 0 = not inverted, -1 = inverted
	LONG invert_left_trigger = 0; // allowed values: 0 = not inverted, -1 = inverted
	LONG invert_right_trigger = 0; // allowed values: 0 = not inverted, -1 = inverted

	// this structure holds pointers to the above, this structure is often passed into functions
	s_aliaslist aliaslist = {
		&iInterface,
		&controller_number,
		&thumbstick_deadzone,
		&virtual_wheel_max_angle,
		&any_key_to_quit,
		&bind_mode,
		&bind_mode_increment,
		&tick_delay,
		&use_right_stick,
		&help_detected,
		&wheel_axis,
		&left_trigger_axis,
		&right_trigger_axis,
		&invert_wheel_axis,
		&invert_left_trigger,
		&invert_right_trigger
	};

	/******************************************************************************/
	/* Parse command line arguments                                               */
	/******************************************************************************/
	
	printf("\n");
	parse_cmdline(aliaslist, argc, argv);
	if (help_detected)
		return 0;
	printf("=== asw settings ===\n");
	printf("using vJoy virtual interface: %d   (recomended value: 1 to 16)\n", iInterface);
	printf("using xinput controller number: %d   (recomended value: 1 to 4)\n", controller_number + 1);
	printf("using thumbstick deadzone: %d   (must be 0 to 100)\n", thumbstick_deadzone);
	printf("rotation: %d\n", virtual_wheel_max_angle);
	printf("use any key to quit: %s\n", any_key_to_quit ? "true" : "false");
	printf("bindmode: %d\n", bind_mode);
	printf("bindmodeincrement: %d\n", bind_mode_increment);
	printf("tick delay: %d\n", tick_delay);
	printf("use right stick: %s\n", use_right_stick ? "true" : "false");
	printf("wheel axis: 0x%x", wheel_axis);
	printf("   left trigger axis: 0x%x", left_trigger_axis);
	printf("   right trigger axis: 0x%x\n", right_trigger_axis);
	printf("axis inverted: wheel: %s", invert_wheel_axis ? "yes" : "no");
	printf("   left trigger: %s", invert_left_trigger ? "yes" : "no");
	printf("   right trigger: %s\n", invert_right_trigger ? "yes" : "no");

	printf("=== starting asw ===\n");

	/******************************************************************************/
	/* Start of vjoy setup code                                                   */
	/******************************************************************************/
	
	// this code is taken from the "How to write a vJoy Feeder (C/C++)" document of version "Updated: 19-Oct-2015 (v2.1.6)"
	// this is available here: http://vjoystick.sourceforge.net/site/includes/SDK_ReadMe.pdf

	// Get the driver attributes (Vendor ID, Product ID, Version Number)
	if (!vJoyEnabled())
	{
		printf("Failed Getting vJoy attributes.\n");
		return -2;
	}
	else
	{
		#ifdef EXTRA_LINES
		printf("Vendor: %S\nProduct: %S\nVersion Number: %S\n", 
			(wchar_t*)GetvJoyManufacturerString(),
			(wchar_t*)GetvJoyProductString(),
			(wchar_t*)GetvJoySerialNumberString());
		#else
		printf("using vJoy version: %S\n", (wchar_t*)GetvJoySerialNumberString());
		#endif
	}
	
	// Test interface DLL matches vJoy driver
	// Compare versions
	WORD VerDll, VerDrv;
	if (!DriverMatch(&VerDll, &VerDrv))
		printf("Failed\r\nvJoy Driver (version %04x) does not match vJoyInterface DLL (version %04x)\n", VerDrv, VerDll);
	else
		printf("OK - vJoy Driver and vJoyInterface DLL match vJoyInterface DLL (version %04x)\n", VerDrv);

	// Get the state of the requested device (iInterface)
	VjdStat status = GetVJDStatus(iInterface);
	switch (status)
	{
	case VJD_STAT_OWN:
		printf("vJoy Device %d is already owned by this feeder\n", iInterface);
		break;
	case VJD_STAT_FREE:
		printf("vJoy Device %d is free\n", iInterface);
		break;
	case VJD_STAT_BUSY:
		printf("vJoy Device %d is already owned by another feeder\nCannot continue\n", iInterface);
		return -3;
	case VJD_STAT_MISS:
		printf("vJoy Device %d is not installed or disabled\nCannot continue\n", iInterface);
		return -4;
	default:
		printf("vJoy Device %d general error\nCannot continue\n", iInterface);
		return -1;
	};

	// Acquire the target if not already owned
	if ((status == VJD_STAT_OWN) || ((status == VJD_STAT_FREE) && (!AcquireVJD(iInterface))))
	{
		printf("Failed to acquire vJoy device number %d.\n", iInterface);
		return -1;
	}
	else
		printf("Acquired: vJoy device number %d.\n", iInterface);

	// Reset this device to default values
	ResetVJD(iInterface);
	
	/******************************************************************************/
	/* Start of xinput setup code                                                 */
	/******************************************************************************/

	// this is just a check to see if the controller is connected
	// the program will continue even if the controller is not connected
	if (XInputGetState(controller_number, &controller_state) == ERROR_DEVICE_NOT_CONNECTED)
		printf("controller %u is not connected, the program will continue but controller input may not be read correctly\n", controller_number+1);
	else
		printf("controller %u is connected\n", controller_number+1);

	printf("asw version %s successfully started, ", VERSION_STRING);
	if (!any_key_to_quit)
		printf("press esc to quit\n");
	else
		printf("press any key to quit\n");


	/******************************************************************************/
	/* Set vjoy wheel axis to center and triggers to 0 before using bind mode     */
	/******************************************************************************/

	if (bind_mode != 0)
	{
		SetAxis(16384, iInterface, wheel_axis); // virtual wheel
		SetAxis(0, iInterface, left_trigger_axis); // left trigger
		SetAxis(0, iInterface, right_trigger_axis); // right trigger
	}

	// set the start position of movement generation for bind mode to the center of the axis in case 
	// bind_mode is 1 (bind_mode 1 means virtial wheel, 2 and 3 means triggers)
	if (bind_mode == 1)
		generated_movement = 16384;

	/******************************************************************************/
	/* Start of main loop                                                         */
	/******************************************************************************/
	
	while (1)
	{
		// this for loop makes the app react to console input events, used mainly for keyboard
		GetNumberOfConsoleInputEvents(console_input, &console_events_count);
		for (unsigned int i = 0; i < console_events_count; i++)
		{
			//printf("number of console events left: %u\n", console_events_count);
			ReadConsoleInput(console_input, &console_input_record, 1, &console_events_read);
			//printf("events read: %u\n", console_events_read);
			//printf("event type: %X\n", console_input_record.EventType);
			switch (console_input_record.EventType)
			{
			case FOCUS_EVENT:
				#ifdef EXTRA_LINES
				printf("FOCUS_EVENT\n");
				#endif
				break;
			case KEY_EVENT:
				#ifdef EXTRA_LINES
				printf("KEY_EVENT");
				#endif
				if (console_input_record.Event.KeyEvent.bKeyDown)
				{
					#ifdef EXTRA_LINES
					printf("   key pressed: %u\n", console_input_record.Event.KeyEvent.wVirtualKeyCode);
					#endif
					if ( console_input_record.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE || any_key_to_quit )
					{
						printf("exiting...\n");
						return 0;
					}
				}
				else
				{
					#ifdef EXTRA_LINES
					printf("   a key was released\n");
					#endif
				}
				break;
			case MENU_EVENT:
				#ifdef EXTRA_LINES
				printf("MENU_EVENT\n");
				#endif
				break;
			case MOUSE_EVENT:
				#ifdef EXTRA_LINES
				printf("MOUSE_EVENT\n");
				#endif
				break;
			case WINDOW_BUFFER_SIZE_EVENT:
				#ifdef EXTRA_LINES
				printf("WINDOW_BUFFER_SIZE_EVENT\n");
				#endif
				break;
			}
		}
		
		if (bind_mode == 0)
		{
			XInputGetState(controller_number, &controller_state);
			
			// virtual wheel code
			if (!use_right_stick)
			{
				if (in_deadzone(controller_state.Gamepad.sThumbLX, controller_state.Gamepad.sThumbLY, thumbstick_deadzone))
					raw_angle = last_raw_angle;
				else
					raw_angle = calculate_angle_from_x_y(controller_state.Gamepad.sThumbLX, controller_state.Gamepad.sThumbLY);
			}
			else
			{
				if (in_deadzone(controller_state.Gamepad.sThumbRX, controller_state.Gamepad.sThumbRY, thumbstick_deadzone))
					raw_angle = last_raw_angle;
				else
					raw_angle = calculate_angle_from_x_y(controller_state.Gamepad.sThumbRX, controller_state.Gamepad.sThumbRY);
			}

			check_if_rotated_past_180(raw_angle, last_raw_angle, &rotations);
			clip_angle_and_rotations(virtual_wheel_max_angle, &raw_angle, &rotations);
			real_angle = calculate_real_angle(raw_angle, virtual_wheel_max_angle, &rotations, &steering_lock);

			SetAxis(invert_vjoy_axis_value(angle_to_vjoy(real_angle, virtual_wheel_max_angle),invert_wheel_axis), iInterface, wheel_axis);
			last_raw_angle = raw_angle;

			SetAxis( invert_vjoy_axis_value( long(round(((double)controller_state.Gamepad.bLeftTrigger)*(32767.0 / 255.0) + 1)), invert_left_trigger), iInterface, left_trigger_axis);
			SetAxis( invert_vjoy_axis_value( long(round(((double)controller_state.Gamepad.bRightTrigger)*(32767.0 / 255.0) + 1)), invert_right_trigger), iInterface, right_trigger_axis);
		}
		else
		{
			switch (bind_mode)
			{
			case 1:
				SetAxis(generated_movement, iInterface, wheel_axis); // virtual wheel
				break;
			case 2:
				SetAxis(generated_movement, iInterface, left_trigger_axis); // left trigger
				break;
			case 3:
				SetAxis(generated_movement, iInterface, right_trigger_axis); // right trigger
				break;
			}

			generated_movement = generated_movement + (bind_mode_increment*generated_movement_direction);
			if (generated_movement > 32768)
			{
				generated_movement = 32768;
				generated_movement_direction = -1;
			}
			else if (generated_movement < 0)
			{
				generated_movement = 1;
				generated_movement_direction = 1;
			}
		}

		Sleep(tick_delay); // the default sleep value for a feeder is 20, I prefer to set it to 8
	}

	//std::system("pause");

	RelinquishVJD(iInterface);
	return 0;
}
