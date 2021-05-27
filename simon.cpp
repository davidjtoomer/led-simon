#include "simon.h"
#include "constants.h"

void repeatFor(unsigned long ms, void (*func)(int), int argument)
{
  unsigned long current_time = millis();
  while (millis() - current_time < ms)
    func(argument);
}

int getQuadrantNumber(quadrant current)
{
  return current[0] ? (current[1] ? 1 : 4) : (current[1] ? 2 : 3);
}

void moveQuadrant(quadrant current, int xPos, int yPos)
{
  if (xPos > MINIMUM_JOYSTICK_POSITION)
    current[0] = min(current[0] + 1, 1);
  else if (xPos < -MINIMUM_JOYSTICK_POSITION)
    current[0] = max(current[0] - 1, 0);
  if (yPos > MINIMUM_JOYSTICK_POSITION)
    current[1] = min(current[1] + 1, 1);
  else if (yPos < -MINIMUM_JOYSTICK_POSITION)
    current[1] = max(current[1] - 1, 0);
}