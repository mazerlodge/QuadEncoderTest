// QuadEncoderTest

/*
 * Goal - Given a quad encoder (QE) monitor turns, output pin1 and pin2 values 
 *   as they change. 
 *   
 * Usage: 
 *  Spin encoder slowly clockwise (CW), output will be 
 *    AB: 00 / 01 / 11 / 10 / repeating
 *    
 *  Sping encoder slowly counterclockwise, (CCW), output will be 
 *    AB: 00 / 10 / 11 / 01 / repeating 
 */

#define QE__PIN_A   7
#define QE__PIN_B   8
#define QE__BUTTON  2

// Encoder sampling variables, current and previous values. 
int currA = 0; 
int currB = 0; 
int prevA = 1; // on encoder boot both A & B go high.
int prevB = 1; 
int currQEButton = 0;
int prevQEButton = 0;

void setup() {
  // Init quad encoder (QE) and serial port.

  // set quad encoder pins to input.
  pinMode(QE__PIN_A, INPUT);
  pinMode(QE__PIN_B, INPUT);  
  pinMode(QE__BUTTON, INPUT);

  // set built in LED to output (Debug flashes) 
  pinMode(LED_BUILTIN, OUTPUT);

  // set serial port for debugging output 
  Serial.begin(9600);

  blinkFast(5);
    
}

void loop() {

  // sample the encoder pins 
  currA = digitalRead(QE__PIN_A); 
  currB = digitalRead(QE__PIN_B); 

  // Test the button press
  currQEButton = digitalRead(QE__BUTTON);
  if (currQEButton != prevQEButton) {
    doQEButtonPress();
  }

  // One quad encoder 'click' return 4 state changes.
  // Only process when a change causes Clock (A) to go high.
  if ((currA != prevA) || (currB != prevB)) { 
    debugMsgQEVals(); 

    prevA = currA;
    prevB = currB;
  }

  
} // loop

void doQEButtonPress() { 
  // QE Button sends a button down and button up signal, including on start up. 
  // Button down curr/prev = 01
  // Button up curr/prev = 10

  // if not a button down event, note it and return. 
  if ((currQEButton == 1) && (prevQEButton == 0)) {
    Serial.println("Button up event detected.");
    // NOTE: if you discard button up events, before returning
    //         remember to set prevQEButton = currQEButton (like below)
  }

  if (currQEButton != prevQEButton) {
    Serial.print("Buttons curr/prev = ");
    Serial.print(currQEButton);
    Serial.println(prevQEButton);
  }

  // Turn off sampling until the next time the button is pressed.
  prevQEButton = currQEButton;
  
}


void debugMsgQEVals() { 
  // output the QE vals to the serial console 

  Serial.print(currA, DEC);
  Serial.print(currB, DEC); 
  Serial.print(" P=");
  Serial.print(prevA, DEC);
  Serial.println(prevB, DEC); 
  
  
} // debugMsgQEVals


void blinkFast(int times) {
  // blink the built-in LED the specified number of times, quickly.

  for (int x=0; x<times; x++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW); 
    delay(250);
  
  } // for x

  delay(1000); 
  
} // blinkFast 

void blinkSlow(int times) { 
  // blink the built-in LED the specified number of times, slowly.

  for(int x=0; x<5; x++) { 
    digitalWrite(LED_BUILTIN, HIGH); 
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(2000); 
    
  } // for x

  delay(1000); 
  
} // blinkSlow
