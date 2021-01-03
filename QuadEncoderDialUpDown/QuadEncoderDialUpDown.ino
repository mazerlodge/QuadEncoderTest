// QuadEncoderDialUpDown

/*
 * Goal - Given a quad encoder (QE) monitor turns, increase a val on clockwise (CW) 
 *          and decrease val on counterclockwise (CCW).
 *   
 * Usage: 
 *  Spin encoder slowly clockwise (CW), output will be 
 *    1 2 3 4... 
 *    
 *  Sping encoder slowly counterclockwise, (CCW), output will be 
 *    4 3 2 1...
 *    
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

int total = 0; // value increased/decreased by encoder turns. 

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
  if (currA != prevA) { 
    doEncoderTurn(); 

    prevA = currA;
    prevB = currB;
  }

  
} // loop

void doQEButtonPress() { 
  // QE Button sends a button down and button up signal, including on start up. 
  // Button down curr/prev = 01
  // Button up curr/prev = 10
  // Reset total on button down.

  // if not a button down event, just return. 
  if ((currQEButton == 1) && (prevQEButton == 0)) {
    prevQEButton = currQEButton;
    return;
  }



  if (currQEButton != prevQEButton) {
    // reset total 
    total = 0;
    
    Serial.print("Buttons curr/prev = ");
    Serial.print(currQEButton);
    Serial.println(prevQEButton);
  }

  // Turn off sampling until the next time the button is pressed.
  prevQEButton = currQEButton;
  
}


void doEncoderTurn() { 
  // output the QE vals to the serial console 

  // Only evaluate when the clock (A) when high
  if (currA == 0) {
    // Disregard this sampling.
    return;
  }

  // Only evaluate when B changed (jitter in button can cause it to be the same)
  if (currB == prevB) {
    // Disregard this sampling.
    return;
  }

  // if B when from high to low, turn was clockwise
  if (currB == 0)
    total++;
  else
    total--;

  Serial.print(currA, DEC);
  Serial.print(currB, DEC); 
  Serial.print(" P=");
  Serial.print(prevA, DEC);
  Serial.print(prevB, DEC); 
  Serial.print(" T=");
  Serial.println(total);
  
} // doEncoderTurn


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
