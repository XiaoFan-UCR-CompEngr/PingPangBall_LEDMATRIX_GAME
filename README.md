# PingPangBall_LEDMATRIX_GAME

## Project Overview
PingPangBall_LEDMATRIX_GAME is an innovative embedded system project leveraging an LED matrix and joysticks to simulate a Ping-Pong game. This project, coded in C, allows users to play a simple, engaging game, displayed on an LED matrix, offering a hands-on approach to understanding embedded systems and game logic.

[Link to Project Demonstration - Speed Up and Bottom Control](https://youtu.be/W09EiYxxPaw)  
[Link to Project Demonstration - Random AI Control Top Side](https://youtu.be/iXHnhQh8STk)

## Features
- Real-time game play on LED Matrix.
- Joystick Controls for user interactions.
- Ball direction and speed control logic.
- Collision detection with paddles.

## Technologies Used
- C Programming Language.
- AVR Microcontroller.
- LED Matrix Display.
- Joysticks for User Input.

## Code Snippet
```c
// Define the CPU frequency
#define F_CPU 1000000 
...
// Initialize A2D
void A2D_init() {
  ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}
...
// Main Loop
int main(void) {
  // Initialize DDR and PORT
  DDRC = 0xFF; PORTC = 0x00;
  ...
  // Initialize Timer
  TimerSet(100);
  TimerOn();
  ...
  // Main execution loop
  while(1) {
    ...
  }
  return 1;
}
Setup and Run
Setup Environment:

Configure AVR/GCC environment.
Connect the LED Matrix and Joysticks to the appropriate pins on the microcontroller.
Compile and Upload:

Compile the code using the AVR/GCC compiler.
Upload the compiled code to the microcontroller.
Play the Game:

Use the joysticks to control the paddles.
The ball's speed and direction change upon collision with paddles.
Contribution
This project is open for improvements and enhancements, and contributions are more than welcome. Please feel free to fork the project, make some updates, and submit pull requests.

Author
Xiao Fan
xfan029@ucr.edu
