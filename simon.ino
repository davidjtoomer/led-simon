#include "constants.h"
#include "simon.h"

// LED I/O pins
const byte ANODE_PINS[8] = {13, 12, 11, 10, 9, 8, 7, 6};
const byte CATHODE_PINS[8] = {A3, A2, A1, A0, 5, 4, 3, 2};
// joystick pins
const byte XPOS = A4;
const byte YPOS = A5;
const byte SEL = 1;

// the order of quadrants for a given game
int ORDER[MAX_GAME_LENGTH] = {};

/**
 * General function to display a pattern on the LED array by passing
 * in a matrix of bytes detailing whether the light should be on or off
 * at that given matrix element.
 */
void display(const byte pattern[8][8])
{
  for (byte i = 0; i < 8; i++)
  {
    // activate cathode pin if pattern[i][j], otherwise deactivate
    for (byte j = 0; j < 8; j++)
      digitalWrite(CATHODE_PINS[j], pattern[i][j] ? LOW : HIGH);

    // temporarily activate anode pin
    digitalWrite(ANODE_PINS[i], LOW);
    delayMicroseconds(100);
    digitalWrite(ANODE_PINS[i], HIGH);
  }
}

/**
 * Displays a quadrant (1, 2, 3, or 4).
 */
void displayQuadrant(int current)
{
  if (current == 0)
    display(BLANK);
  else if (current == 1)
    display(QI);
  else if (current == 2)
    display(QII);
  else if (current == 3)
    display(QIII);
  else if (current == 4)
    display(QIV);
  else if (current == 5)
    display(FILLED);
}

/**
 * Reads the current joystick position and updates the displayed quadrant accordingly
 * until the pushbutton is pressed.
 */
void moveJoystick(quadrant current)
{
  int xPos, yPos;
  while (digitalRead(SEL) == HIGH)
  {
    xPos = map(analogRead(XPOS), 0, 1023, -JOYSTICK_BOUND, JOYSTICK_BOUND);
    yPos = map(analogRead(YPOS), 0, 1023, -JOYSTICK_BOUND, JOYSTICK_BOUND);
    moveQuadrant(current, xPos, yPos);
    displayQuadrant(getQuadrantNumber(current));
  }
  // after SEL is hit, delay to debounce.
  delay(DEBOUNCE_DELAY);
}

/**
 * Shows the user that the game is either restarting, or that they have successfully
 * completed an iteration of the game and can move onto the next iteration.
 */
void resetLevel()
{
  for (int i = 0; i < 3; i++)
  {
    repeatFor(RESET_DURATION, displayQuadrant, 5);
    repeatFor(RESET_DURATION, displayQuadrant, 0);
  }
  repeatFor(4 * RESET_DURATION, displayQuadrant, 5);
  repeatFor(RESET_DURATION, displayQuadrant, 0);
}

/**
 * Rapid flashing so that the user knows they have lost the game.
 */
void lose()
{
  for (int i = 0; i < 10; i++)
  {
    repeatFor(LOSE_DURATION, displayQuadrant, 5);
    repeatFor(LOSE_DURATION, displayQuadrant, 0);
  }
}

/**
 * Displays the first num_iterations quadrants contained in ORDER
 * for the user to replicate at a given level.
 */
void displayOrder(int num_iterations)
{
  for (int i = 0; i <= num_iterations; i++)
  {
    repeatFor(DISPLAY_DURATION, displayQuadrant, ORDER[i]);
    repeatFor(150, displayQuadrant, 0);
  }
  repeatFor(DISPLAY_DURATION, displayQuadrant, 0);
}

/**
 * Plays through one iteration of a game.
 */
bool playIteration(int iteration)
{
  displayOrder(iteration);
  quadrant current = new int[2];
  current[0] = current[1] = 0;
  for (int i = 0; i <= iteration; i++)
  {
    moveJoystick(current);
    if (getQuadrantNumber(current) != ORDER[i])
      return false;
  }
  delete current; // free heap-allocated memory to avoid leak
  return true;
}

/**
 * Sets ORDER so that each game has a pseudorandom order. The seed is based on the current time,
 * so the first game will always have the same order, but every game after that will have a 
 * different pattern.
 */
void setRandomOrder()
{
  randomSeed(millis());
  for (int i = 0; i < MAX_GAME_LENGTH; i++)
    ORDER[i] = int(random(1, 5));
}

void setup()
{
  // iterate through the anode/cathode pins, set them to OUTPUT, and turn them off (HIGH)
  for (byte i = 0; i < 8; i++)
  {
    pinMode(ANODE_PINS[i], OUTPUT);
    digitalWrite(ANODE_PINS[i], HIGH); // turn off
    pinMode(CATHODE_PINS[i], OUTPUT);
    digitalWrite(CATHODE_PINS[i], HIGH); // turn off
  }

  // set the joystick pins
  pinMode(XPOS, INPUT);
  pinMode(YPOS, INPUT);
  pinMode(SEL, INPUT_PULLUP);
}

void loop()
{
  setRandomOrder();
  for (int i = 0; i < MAX_GAME_LENGTH; i++)
  {
    resetLevel();
    if (!playIteration(i))
    {
      lose();
      break;
    }
  }
  repeatFor(3000, displayQuadrant, 0); // show that the game is resetting
}
