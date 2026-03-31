#ifndef MAINSCREENGEOMETRY_H
#define MAINSCREENGEOMETRY_H

// geometry defines for main screen buttons

// shared dimensions
#define TOGGLE_W 70
#define TOGGLE_H 120

#define CONTROL_W 48
#define CONTROL_H 42

// individual positions + use shared sizes
#define FRONT_UP_X 0
#define FRONT_UP_Y 0
#define FRONT_UP_W TOGGLE_W
#define FRONT_UP_H TOGGLE_H

#define FRONT_DOWN_X 0
#define FRONT_DOWN_Y 120
#define FRONT_DOWN_W TOGGLE_W
#define FRONT_DOWN_H TOGGLE_H

#define BACK_UP_X 263
#define BACK_UP_Y 0
#define BACK_UP_W TOGGLE_W
#define BACK_UP_H TOGGLE_H

#define BACK_DOWN_X 263
#define BACK_DOWN_Y 120
#define BACK_DOWN_W TOGGLE_W
#define BACK_DOWN_H TOGGLE_H

#define SETTINGS1_X 132
#define SETTINGS1_Y 174
#define SETTINGS1_W CONTROL_W
#define SETTINGS1_H CONTROL_H

#define RIDE_X 84
#define RIDE_Y 174
#define RIDE_W CONTROL_W
#define RIDE_H CONTROL_H

#define PARK_X 186
#define PARK_Y 174
#define PARK_W CONTROL_W
#define PARK_H CONTROL_H

#endif // MAINSCREENGEOMETRY_H
