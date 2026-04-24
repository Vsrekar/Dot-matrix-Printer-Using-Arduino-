#include <Stepper.h>
#include <Servo.h>

// Arduino code to operate stepper motors and servo motors to work together as a printer
// X direction for movement of print head
// Y direction of movement of pen
// Z direction of movement of paper

// Define the dimensions of the 2D array
const int rows = 26;     // size of input bitmap array to be defined here
const int cols = 30;
const int stepsPerRevolution = 2048;  // for this stepper motor, each revoltution needs 2048 steps
const int step_sizex = 70;    // step_size for x axis 
const int step_sizez = 150;   // step_size for z axis

 // Declare the 2D array
int myArray[rows][cols] = {     // enter bitmap array of given size here //this code prints "KLE CEER" and 3 downwards arrow
{0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1},
{0,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,1,1},
{0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,0,0},
{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,0},
{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1},
{0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
{0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0}, 
{0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,0},
{0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,0},
{0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
{0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
{0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
{0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0}
};

Stepper printHead = Stepper(stepsPerRevolution, 8, 10, 9, 11); // set pins for different stepper motors
Stepper paper_roll = Stepper(stepsPerRevolution, 4, 6, 5, 7); // set pins for different stepper motors
Servo pen;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position

void setup() {
  // Start the serial communication
  pen.attach(12);  // attaches the servo on pin 12 to the servo object 
  Serial.begin(115200);
}

void loop() {
  
   // Turn on the start LED
  printHead.setSpeed(10); // min speed 1, max speed 10, the frequency at which each coil fires.
  paper_roll.setSpeed(10);
	
  // Read through each element of the 2D array and act according to the value
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      
      // Print the value of the current element
      Serial.println(myArray[i][j]);
      if(myArray[i][j] == 1){
        Serial.print(" Pen Down, ");
        Serial.print(" dot, ");
        dot();
        Serial.print(" pen up, ");
      }
      else{Serial.print(" pen up, next, ");}
      delay(100);
      Serial.println("step x, ");
      printHead.step(step_sizex);  // change the step_size proportional to the distance to be moved in each step
      delay(100);     // delay after stepper comand necessary to let the stepper move to next position, before getting any other signal
    }
    Serial.println(" pen up, ");
    delay(100);
    Serial.println("Reverse to x = 0, ");
    printHead.step(cols*(-1* step_sizex));
    delay(200);
    Serial.println(" Step z, ");
    delay(500);
    paper_roll.step(step_sizez);  // change step_sizez proprotional to the step of z axis
  }
 // Turn off the start LED
   Serial.println("done");
   
  // Add a delay if needed to control the rate of printing
  delay(1000);
}

void dot(){     // function defined to move servo up and down
  delay(50);
  pen.write(0);   // number in bracket is angular position of servo.
  delay(100);
  pen.write(50);  // adjust angle according to need
  delay(0);
}