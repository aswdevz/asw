// xy_to_angle.h
//

#pragma once

bool in_deadzone(short x, short y, int deadzone);
double calculate_angle_from_x_y(short x, short y);
long angle_to_vjoy(double angle, int virtual_wheel_max_angle);
int check_if_rotated_past_180(double current, double last, int* rotations);
int clip_angle_and_rotations(int virtual_wheel_max_angle, double* angle_in, int* rotations);
double calculate_real_angle(double current, int virtual_wheel_max_angle, int* rotations, int* steering_lock);
LONG invert_vjoy_axis_value(LONG value, int invert_axis);