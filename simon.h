#include <Arduino.h>

// for convenience, define a quadrant type that is an array of integers
typedef int *quadrant;

/**
 * This is a generic function to repeat a given function for a defined amount of time.
 * It takes a pointer to a function as an argument (int), as well as the actual argument 
 * to the function, and repeats the function with that argument for ms milliseconds.
 */
void repeatFor(unsigned long ms, void (*func)(int), int argument);

/**
 * This function returns the quadrant number (1, 2, 3, or 4) based on the 2D coordinates
 * passed in the int array current. Quadrants are numbered based on convention in the 
 * Cartesian 2D coordinate system.
 */
int getQuadrantNumber(quadrant current);

/**
 * This function takes in a quadrant and changes the position of the quadrant based
 * on the current position of theh joystick. The quadrant is a pointer to an array, 
 * so all modifications persist. xPos and yPos represent the x- and y-positions of the 
 * joystick as mapped from (0, 1023) to (-JOYSTICK_BOUND, JOYSTICK_BOUND). 
 */
void moveQuadrant(quadrant current, int xPos, int yPos);