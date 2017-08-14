/* Buttons to USB Keyboard Example

   You must select Keyboard from the "Tools > USB Type" menu

   This example code is in the public domain.
*/

#include <Bounce.h>

// Create Bounce objects for each button.  The Bounce object
// automatically deals with contact chatter or "bounce", and
// it makes detecting changes very simple.
Bounce button0 = Bounce(0, 100);
Bounce button1 = Bounce(1, 100);  // 10 = 10 ms debounce time
Bounce button2 = Bounce(2, 100);  // which is appropriate for
Bounce button3 = Bounce(3, 100);  // most mechanical pushbuttons
Bounce button4 = Bounce(4, 100);
Bounce button5 = Bounce(5, 100);  // if a button is too "sensitive"

void setup() {
  // Configure the pins for input mode with pullup resistors.
  // The pushbuttons connect from each pin to ground.  When
  // the button is pressed, the pin reads LOW because the button
  // shorts it to ground.  When released, the pin reads HIGH
  // because the pullup resistor connects to +5 volts inside
  // the chip.  LOW for "on", and HIGH for "off" may seem
  // backwards, but using the on-chip pullup resistors is very
  // convenient.  The scheme is called "active low", and it's
  // very commonly used in electronics... so much that the chip
  // has built-in pullup resistors!
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}

void loop() {
  // Update all the buttons.  There should not be any long
  // delays in loop(), so this runs repetitively at a rate
  // faster than the buttons could be pressed and released.
  button0.update();
  button1.update();
  button2.update();
  button3.update();
  button4.update();
  button5.update();

  // Check each button for "falling" edge.
  // Type a message on the Keyboard when each button presses
  // Update the Joystick buttons only upon changes.
  // falling = high (not pressed - voltage from pullup resistor)
  //           to low (pressed - button connects pin to ground)
  if (button0.fallingEdge()) {
    Keyboard.print("a");
  }
  if (button1.fallingEdge()) {
    Keyboard.print("s");
  }
  if (button2.fallingEdge()) {
    Keyboard.print("d");
  }
  if (button3.fallingEdge()) {
    Keyboard.print("f");
  }
  if (button4.fallingEdge()) {
    Keyboard.print("g");
  }
  if (button5.fallingEdge()) {
    Keyboard.print("h");
  }

  // Check each button for "rising" edge
  // Type a message on the Keyboard when each button releases.
  // For many types of projects, you only care when the button
  // is pressed and the release isn't needed.
  // rising = low (pressed - button connects pin to ground)
  //          to high (not pressed - voltage from pullup resistor)
//  if (button0.risingEdge()) {
//    Keyboard.print("a");
//  }
//  if (button1.risingEdge()) {
//    Keyboard.print("s");
//  }
//  if (button2.risingEdge()) {
//    Keyboard.print("d");
//  }
//  if (button3.risingEdge()) {
//    Keyboard.print("f");
//  }
//  if (button4.risingEdge()) {
//    Keyboard.print("g");
//  }
//  if (button5.risingEdge()) {
//    Keyboard.print("h");
//  }
}

