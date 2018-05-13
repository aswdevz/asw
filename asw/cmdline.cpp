// cmdline.cpp : This file contains functions that parse the command line arguments
//

#include "stdafx.h" // for precompiled headers, must be present in every source file

#include "asw.h" // I'm including this in every source file
#include "cmdline.h"

bool parse_cmdline(s_aliaslist aliaslist, int argc, char* argv[])
{
	int int_scan_val = 0;

	#ifdef EXTRA_LINES
	printf("argument count: %d\n", argc);
	#endif
	for (int i = 0; i < argc; i++)
	{
		#ifdef EXTRA_LINES
		printf("argument %d : %s\n", i, argv[i]);
		#endif
	}

	for (int i = 1; i < argc; i++)
	{
		#ifdef EXTRA_LINES
		printf(">>cmdline parse loop<<\n");
		#endif

		if ((strcmp(argv[i], "-interface") == 0) || (strcmp(argv[i], "-i") == 0))
		{
			//printf("-interface detected\n");
			if ((i + 1) < argc)
			{
				if (sscanf_s(argv[i + 1], "%d", &int_scan_val) == 1)
				{
					// valid vjoy interface numbers are 1-16
					*(aliaslist.iInterface) = int_scan_val;
				}
				else
				{
					printf("-interface value not present or in incorrect format\n");
				}
			}
			else
			{
				printf("-interface value not present\n");
			}
			continue;
		}

		if ((strcmp(argv[i], "-controller") == 0) || (strcmp(argv[i], "-c") == 0))
		{
			//printf("-controller detected\n");
			if ((i + 1) < argc)
			{
				if (sscanf_s(argv[i + 1], "%d", &int_scan_val) == 1)
				{
					// valid xinput controller number is 0-3
					// 0 corresponds to controller number 1 etc.
					*(aliaslist.controller_number) = int_scan_val-1;
				}
				else
				{
					printf("-controller value not present or in incorrect format\n");
				}
			}
			else
			{
				printf("-controller value not present\n");
			}
			continue;
		}

		if ((strcmp(argv[i], "-deadzone") == 0) || (strcmp(argv[i], "-d") == 0))
		{
			//printf("-deadzone detected\n");
			if ((i + 1) < argc)
			{
				if (sscanf_s(argv[i + 1], "%d", &int_scan_val) == 1)
				{
					// valid deadzone setting is 0-100
					if ((int_scan_val >= 0) && (int_scan_val <= 100))
					{
						*(aliaslist.thumbstick_deadzone) = int_scan_val;
					}
					else
					{
						printf("-deadzone value out of range, min=0, max=100\n");
					}
				}
				else
				{
					printf("-deadzone value not present or in incorrect format\n");
				}
			}
			else
			{
				printf("-deadzone value not present\n");
			}
			continue;
		}

		if ( (strcmp(argv[i], "-rotation") == 0) || (strcmp(argv[i], "-r") == 0) )
		{
			//printf("-rotation detected\n");
			if ( (i + 1) < argc)
			{
				if (sscanf_s(argv[i + 1], "%d", &int_scan_val) == 1)
				{
					// I should probably clip this to 0-32767 just to be sure
					*(aliaslist.virtual_wheel_max_angle) = int_scan_val;
				}
				else
				{
					printf("-rotation value not present or in incorrect format\n");
				}
			}
			else
			{
				printf("-rotation value not present\n");
			}
			continue;
		}

		if ((strcmp(argv[i], "-bindmode") == 0) || (strcmp(argv[i], "-b") == 0))
		{
			//printf("-bindmode detected\n");
			if ((i + 1) < argc)
			{
				if (sscanf_s(argv[i + 1], "%d", &int_scan_val) == 1)
				{
					// bindmode should be 0-3
					if ((int_scan_val >= 0) && (int_scan_val <= 3))
					{
						*(aliaslist.bind_mode) = int_scan_val;
					}
					else
					{
						printf("-bindmode value out of range, min=0, max=3\n");
					}
				}
				else
				{
					printf("-bindmode value not present or in incorrect format\n");
				}
			}
			else
			{
				printf("-bindmode value not present\n");
			}
			continue;
		}

		if ((strcmp(argv[i], "-bindmodeincrement") == 0) || (strcmp(argv[i], "-bmi") == 0))
		{
			//printf("-bindmodeincrement detected\n");
			if ((i + 1) < argc)
			{
				if (sscanf_s(argv[i + 1], "%d", &int_scan_val) == 1)
				{
					// bindmodeincrement should be 1-8192
					if ((int_scan_val >= 1) && (int_scan_val <= 8192))
					{
						*(aliaslist.bind_mode_increment) = int_scan_val;
					}
					else
					{
						printf("-bindmodeincrement value out of range, min=1, max=8192\n");
					}
				}
				else
				{
					printf("-bindmodeincrement value not present or in incorrect format\n");
				}
			}
			else
			{
				printf("-bindmodeincrement value not present\n");
			}
			continue;
		}

		if ((strcmp(argv[i], "-bindmodemovementdirection") == 0) || (strcmp(argv[i], "-bmmd") == 0))
		{
			//printf("-bindmodemovementdirection detected\n");
			if ((i + 1) < argc)
			{
				if (sscanf_s(argv[i + 1], "%d", &int_scan_val) == 1)
				{
					// bindmodemovementdirection should be 1 or -1
					if ((int_scan_val == 1) || (int_scan_val == -1))
					{
						*(aliaslist.bind_mode_movement_direction) = int_scan_val;
					}
					else
					{
						printf("-bindmodemovementdirection value out of range, allowed values: 1 and -1\n");
					}
				}
				else
				{
					printf("-bindmodemovementdirection value not present or in incorrect format\n");
				}
			}
			else
			{
				printf("-bindmodemovementdirection value not present\n");
			}
			continue;
		}
		
		if ((strcmp(argv[i], "-bindmodenonreversible") == 0) || (strcmp(argv[i], "-bmnr") == 0))
		{
			//printf("-bindmodenonreversible detected\n");
			*(aliaslist.bind_mode_non_reversible) = 1;
			continue;
		}

		if ((strcmp(argv[i], "-bindmoderesetwait") == 0) || (strcmp(argv[i], "-bmrw") == 0))
		{
			//printf("-bindmoderesetwait detected\n");
			if ((i + 1) < argc)
			{
				if (sscanf_s(argv[i + 1], "%d", &int_scan_val) == 1)
				{
					*(aliaslist.bind_mode_reset_wait) = int_scan_val;
				}
				else
				{
					printf("-bindmoderesetwait value not present or in incorrect format\n");
				}
			}
			else
			{
				printf("-bindmoderesetwait value not present\n");
			}
			continue;
		}

		if ((strcmp(argv[i], "-wheelresetbuttonsflag") == 0) || (strcmp(argv[i], "-wrbf") == 0))
		{
			//printf("-wheelresetbuttonsflag detected\n");
			if ((i + 1) < argc)
			{
				if (sscanf_s(argv[i + 1], "%x", &int_scan_val) == 1)
				{
					*(aliaslist.wheel_reset_buttons_flag) = int_scan_val;
				}
				else
				{
					printf("-wheelresetbuttonsflag value not present or in incorrect format\n");
				}
			}
			else
			{
				printf("-wheelresetbuttonsflag value not present\n");
			}
			continue;
		}

		if ((strcmp(argv[i], "-disablewheelreset") == 0) || (strcmp(argv[i], "-dwr") == 0))
		{
			//printf("-disablewheelreset detected\n");
			*(aliaslist.disable_wheel_reset) = 1;
			continue;
		}


		if ((strcmp(argv[i], "-anykey") == 0) || (strcmp(argv[i], "-a") == 0))
		{
			//printf("-anykey detected\n");
			*(aliaslist.any_key_to_quit) = 1;
			continue;
		}

		if ((strcmp(argv[i], "-tickdelay") == 0) || (strcmp(argv[i], "-t") == 0))
		{
			//printf("-tickdelay detected\n");
			if ((i + 1) < argc)
			{
				if (sscanf_s(argv[i + 1], "%d", &int_scan_val) == 1)
				{
					// tickdelay should be 1-1000
					if ((int_scan_val >= 1) && (int_scan_val <= 1000))
					{
						*(aliaslist.tick_delay) = int_scan_val;
					}
					else
					{
						printf("-tickdelay value out of range, min=1, max=1000\n");
					}
				}
				else
				{
					printf("-tickdelay value not present or in incorrect format\n");
				}
			}
			else
			{
				printf("-tickdelay value not present\n");
			}
			continue;
		}

		if ((strcmp(argv[i], "-rightstick") == 0) || (strcmp(argv[i], "-rs") == 0))
		{
			//printf("-rightstick detected\n");
			*(aliaslist.use_right_stick) = 1;
			continue;
		}

		if ((strcmp(argv[i], "-wheelaxis") == 0) || (strcmp(argv[i], "-wa") == 0))
		{
			//printf("-wheelaxis detected\n");
			if ((i + 1) < argc)
			{
				if (sscanf_s(argv[i + 1], "%x", &int_scan_val) == 1)
				{
					*(aliaslist.wheel_axis) = int_scan_val;
				}
				else
				{
					printf("-wheelaxis value not present or in incorrect format\n");
				}
			}
			else
			{
				printf("-wheelaxis value not present\n");
			}
			continue;
		}

		if ((strcmp(argv[i], "-lefttriggeraxis") == 0) || (strcmp(argv[i], "-lta") == 0))
		{
			//printf("-lefttriggeraxis detected\n");
			if ((i + 1) < argc)
			{
				if (sscanf_s(argv[i + 1], "%x", &int_scan_val) == 1)
				{
					*(aliaslist.left_trigger_axis) = int_scan_val;
				}
				else
				{
					printf("-lefttriggeraxis value not present or in incorrect format\n");
				}
			}
			else
			{
				printf("-lefttriggeraxis value not present\n");
			}
			continue;
		}

		if ((strcmp(argv[i], "-righttriggeraxis") == 0) || (strcmp(argv[i], "-rta") == 0))
		{
			//printf("-righttriggeraxis detected\n");
			if ((i + 1) < argc)
			{
				if (sscanf_s(argv[i + 1], "%x", &int_scan_val) == 1)
				{
					*(aliaslist.right_trigger_axis) = int_scan_val;
				}
				else
				{
					printf("-righttriggeraxis value not present or in incorrect format\n");
				}
			}
			else
			{
				printf("-righttriggeraxis value not present\n");
			}
			continue;
		}

		if ((strcmp(argv[i], "-invertwheel") == 0) || (strcmp(argv[i], "-iw") == 0))
		{
			//printf("-invertwheel detected\n");
			*(aliaslist.invert_wheel_axis) = 1;
			continue;
		}

		if ((strcmp(argv[i], "-invertletfttrigger") == 0) || (strcmp(argv[i], "-ilt") == 0))
		{
			//printf("-invertletfttrigger detected\n");
			*(aliaslist.invert_left_trigger) = 1;
			continue;
		}

		if ((strcmp(argv[i], "-invertrighttrigger") == 0) || (strcmp(argv[i], "-irt") == 0))
		{
			//printf("-invertrighttrigger detected\n");
			*(aliaslist.invert_right_trigger) = 1;
			continue;
		}

		if ((strcmp(argv[i], "-help") == 0) || (strcmp(argv[i], "-h") == 0))
		{
			//printf("-help detected\n");
			*(aliaslist.help_detected) = true;
			printf("Usage: asw [-i vjoyid] [-c controller] [-d size] [-r range] [-b]\n");
			printf("           [-bmi increment] [-a] [-t delay] [-rs] [-wa axis]\n");
			printf("           [-lta axis] [-rta axis] [-iw] [-ilt] [-irt] [-h]\n");
			printf("\n");
			printf("Options:\n");
			printf("  -i vjoyid       vJoy interface to use [1-16]\n");
			printf("  -c controller   controller to use [1-4]\n");
			printf("  -d size         deadzone for the thumbstick input in percent\n");
			printf("  -r range        number of degrees the virtual wheel can be rotated to\n");
			printf("  -b              run program in bind mode\n");
			printf("  -bmi increment  adjust the speed of bind mode\n");
			printf("  -a              make any key press quit the program\n");
			printf("  -t delay        delay between consecutive vJoy device updates\n");
			printf("  -rs             use right thumbstick as input for the virtual wheel\n");
			printf("  -wa axis        vJoy device axis used as output for the virtual wheel\n");
			printf("  -lta axis       vJoy device axis used as output for the left trigger\n");
			printf("  -rta axis       vJoy device axis used as output for the right trigger\n");
			printf("  -iw             invert virtual wheel axis output\n");
			printf("  -ilt            invert left trigger output\n");
			printf("  -irt            invert right trigger output\n");
			printf("  -h              display this help text\n");
			printf("  -v              display program version, current version: %s\n", VERSION_STRING);
			continue;
		}

		if ((strcmp(argv[i], "-version") == 0) || (strcmp(argv[i], "-v") == 0))
		{
			//printf("-version detected\n");
			*(aliaslist.help_detected) = true;
			printf("asw version %s\n", VERSION_STRING);
			continue;
		}

		// add extra comparisons here
	}

	return true;
}
