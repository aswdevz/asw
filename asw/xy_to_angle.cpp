// xy_to_rot.cpp : This file contains functions that take the X and Y values representing
// the position of the controller thumbstick and convertes them to an angle and then to
// an integer that can be sent to vjoy. Functions that help this process are also present.
//

//#define USE_ORIGINAL_DEADZONE_FUNCTION // there are two variations of the deadzone function, this switches between them

#include "stdafx.h" // for precompiled headers, must be present in every source file

#include "asw.h" // I'm including this in every source file
#include "xy_to_angle.h"

#ifdef USE_ORIGINAL_DEADZONE_FUNCTION
// the deadzone will be +1% higher with this implementation
bool in_deadzone(short x, short y, int deadzone)
{
	int x_percent = (int)(abs(x) / 327.67);
	int y_percent = (int)(abs(y) / 327.67);
	//cout << "xper= " << x_percent << " yper= " << y_percent << " total= " << (pow(x_percent, 2) + pow(y_percent, 2)) << " dz= " << pow(deadzone,2) << endl;
	if ( sqrt(pow(x_percent, 2) + pow(y_percent, 2) ) <= deadzone)
	{
		return TRUE;
	}
	return FALSE;
}

#else
// alternate deadzone function
bool in_deadzone(short x, short y, int deadzone)
{
	double x_percent = abs(x) / 327.67;
	double y_percent = abs(y) / 327.67;
	

	if ( sqrt(pow(x_percent, 2) + pow(y_percent, 2) )  <= deadzone)
	{
		//printf(">IN DEADZONE<");
		return TRUE;
	}
	return FALSE;
}
#endif

double calculate_angle_from_x_y(short x, short y)
{
	if (x == 0) // make sure we don't get a divide by zero
	{
		if (y >= 0)
			return 0.0;
		else
			return 180.0;
	}
	if (y == 0)
	{
		if (x >= 0)
			return 90.0;
		else
			return -90.0;
	}

	if (x > 0)
	{
		if (y > 0) // first quadrant
		{
			return (atan((double)x / (double)y) * 180.0 / M_PI);
		}
		if (y < 0) // fourth quadrant
		{
			return ((atan(-(double)y / (double)x) * 180.0 / M_PI) + 90.0);
		}

	}
	if (x < 0)
	{
		if (y > 0) // second quadrant
		{
			return ((-atan(-(double)x / (double)y)) * 180.0 / M_PI);
		}
		if (y < 0) // third quadrant
		{
			return ((-atan(-(double)y / -(double)x) * 180.0 / M_PI) - 90.0);
		}
	}

	return 0.0;
}

// this converts the angle of the pretend wheel to an input value in the range vjoy works with
// vjoy axis input works in this range: 0x0001 to 0x8000 (32768)
// the correct calcualtion for this conversion is: (angle + max_angle) * (vjoy_range / angle_range) + 1
// angle range is abs(min_angle) + abs(max_angle) which is equal to virtual_wheel_max_angle
// vjoy range is max_vjoy_input_value - min_vjoy_input_value = 32768 - 1 = 32767
// the result is rounded before conversion to long
long angle_to_vjoy(double angle, int virtual_wheel_max_angle)
{
	double max_angle = ((double)virtual_wheel_max_angle) / 2;
	double min_angle = -((double)virtual_wheel_max_angle) / 2;

	if (angle > max_angle)
		angle = max_angle;
	if (angle < min_angle)
		angle = min_angle;

	long finalvalue = (long)round(((angle + max_angle)*(32767.0 / ((double)virtual_wheel_max_angle)) + 1));
	//long finalvalue = (long)((angle + max_angle)*(32767.0 / ((double)virtual_wheel_max_angle)) + 1);

	return finalvalue;
}

int check_if_rotated_past_180(double current, double last, int* rotations)
{
	// the (current >= -180.0) condition can also be (current = -180.0)
	// this is because the function that prepares data for this one only
	// outputs a range of 180.0 (inclusive) to -180.0 (noninclusive)
	// the (current < 0.0) condition can also be (current <= 0.0) but this makes no difference
	// this is because when current is 0, the last condition can't ever be met
	// (current < (last - 180.0)) condition checked - it's ok
	if (((current >= -180.0) && (current < 0.0)) && (current < (last - 180.0))) // case 1
	{
		*rotations = *rotations + 1;
		//cout << current << " last " << last << endl;
		return 1;
	}

	// the (current <= 180.0) condition is correct, a value of 180.0 can appear
	// this is because the function that prepares data for this one
	// outputs a range of 180.0 (inclusive) to -180.0 (noninclusive)
	// the (current > 0.0) condition can also be (current >= 0.0) but this makes no difference // recently changed this - retest
	// this is because when current is 0, the last condition can't ever be met
	// (current > (last + 180.0)) condition checked - it's ok
	if (((current <= 180.0) && (current > 0.0)) && (current > (last + 180.0))) // case 3
	{
		*rotations = *rotations - 1;
		return -1;
	}
	return 0;
}

// there is a tiny mistake here, that does not cause issues
int clip_angle_and_rotations(int virtual_wheel_max_angle, double* angle_in, int* rotations)
{
	double right_lock = ((double)virtual_wheel_max_angle) / 2;
	double left_lock = -(((double)virtual_wheel_max_angle) / 2);

	// mistake is here: the following two lines will not assign the correct values to max_rotations and
	// min_rotations for a virtual_wheel_max_anglee of 360, 1080, 1800, 2520 etc., this is because at 360,
	// the number of times the stick can rotate past 180 (-180) is actually 0 but this code sets the maximum to 1 (-1)
	int max_rotations = (int)((right_lock + 180.0) / 360.0);
	int min_rotations = (int)((left_lock - 180.0) / 360.0);

	// the above mistake is not a problem since the following two lines will set the max_angle to
	// -180.0 if the above situation happens and a -180.0 and 1 rotation is the same as 180.0 and 0 rotations
	// as far as the code that takes the rusults of this function as input is concerned (in case the 
	// virtual_wheel_max_angle is 360 and a value of 180.0 is the input angle). A simmilar thing will happen
	// in all other cases.
	double max_angle = right_lock - (360.0 * max_rotations);
	double min_angle = left_lock + (360.0 * (-min_rotations));

	double angle = ((*angle_in) + (*rotations)*360.0);

	if (angle > ((double)virtual_wheel_max_angle) / 2)
	{
		angle = ((double)virtual_wheel_max_angle) / 2;
		*angle_in = max_angle;
		*rotations = max_rotations;
	}
	if (angle < (-((double)virtual_wheel_max_angle) / 2))
	{
		angle = -(((double)virtual_wheel_max_angle) / 2);
		*angle_in = min_angle;
		*rotations = min_rotations;
	}

	//cout << "rl = " << right_lock << " ll = " << left_lock << " maxr= " << max_rotations << " minr= " << min_rotations << " max-a " << max_angle << " min-a " << min_angle << " out= " << angle << endl;
	return 0;
}

// this calculates the real angle by taking the raw_angle which is a value from -180.0 (noninclusive)
// to 180.0 (inclusive) and adds the the number of full rotations to this value
double calculate_real_angle(double current, int virtual_wheel_max_angle, int* rotations, int* steering_lock)
{
	//	cout << "current " << current << endl;
	*steering_lock = 0;

	double angle = (current + (*rotations)*360.0);

	// this calcualtion is wrong because: clip_angle_and_rotations() function will always adjust the angle that enters this function
	// so that the angle is always at most the maximum possible value as defined by (double)virtual_wheel_max_angle) / 2
	// so these two condions will never be true, so change the conditons from (angle > ((double)virtual_wheel_max_angle) / 2)
	// to (angle = ((double)virtual_wheel_max_angle) / 2) or completely remove this because steering_lock variable
	// is not used anywhere, also this calculation can be moved to clip_angle_and_rotations()
	if (angle > ((double)virtual_wheel_max_angle) / 2)
	{
		angle = ((double)virtual_wheel_max_angle) / 2;
		*steering_lock = 1;
	}
	if (angle < (-((double)virtual_wheel_max_angle) / 2))
	{
		angle = -(((double)virtual_wheel_max_angle) / 2);
		*steering_lock = -1;
	}
	return angle;
}

LONG invert_vjoy_axis_value(LONG value, int invert_axis)
{
	if (invert_axis == 1)
		return 32769 - value;
	else
		return value;
}
