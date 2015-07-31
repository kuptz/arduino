int lightpin=0;
int ledpin=9;
int buzzpin=10;
int buttonpin=3;
int enabled=0;
int threshold=300;

const byte RED[] = {255, 0, 0}; 
const byte ORANGE[] = {83, 4, 0}; 
const byte YELLOW[] = {255, 255, 0}; 
const byte GREEN[] = {0, 255, 0}; 
const byte BLUE[] = {0, 0, 255}; 
const byte INDIGO[] = {4, 0, 19}; 
const byte VIOLET[] = {23, 0, 22}; 
const byte CYAN[] = {0, 255, 255}; 
const byte MAGENTA[] = {255, 0, 255}; 
const byte WHITE[] = {255, 255, 255}; 
const byte BLACK[] = {0, 0, 0}; 
const byte PINK[] = {158, 4, 79}; 


//RGB LED pins
int ledAnalogOne[] = {5,6,7}; //the three pins of the first analog LED 3 = redPin, 5 = greenPin, 6 = bluePin
                                //These pins must be PWM

void turnonalarm() {
  //digitalWrite(ledpin, HIGH);
  //digitalWrite(buzzpin, HIGH);
  analogWrite(buzzpin, 100);
  fadeToColor(ledAnalogOne, BLUE, RED, 5); 
  analogWrite(buzzpin, 0);
 }

void turnoffalarm() {
 // digitalWrite(ledpin, LOW);
 // digitalWrite(buzzpin, LOW); 
  analogWrite(buzzpin, 0);
  
  //for(int i = 0; i < 3; i++){
   setColor(ledAnalogOne, BLACK);       //Turn off led
  //}
 
}

void setup() 
{
   pinMode(ledpin,OUTPUT);
   pinMode(buzzpin, OUTPUT);
   pinMode(buttonpin, INPUT);

   turnoffalarm();

  // setup rgb led
  for(int i = 0; i < 3; i++){
   pinMode(ledAnalogOne[i], OUTPUT);   //Set the three LED pins as outputs

  }
   setColor(ledAnalogOne, BLACK);       //Turn off led
  Serial.begin(9600);
}

void blinkled(int blinks) {

  for(int i = 0; i < blinks; i++) {
    digitalWrite(ledpin, HIGH);
    delay(200);
    digitalWrite(ledpin, LOW); 
     
  }
}


void loop()
{
  int val = digitalRead(buttonpin);
  //Serial.println(val);
  
  if (val == LOW) {
    if (enabled == 0) {
      enabled = 1;

      fadeToColor(ledAnalogOne, MAGENTA, WHITE, 5); 
      //blinkled(2);
      //delay(5000);   
    } else {
      enabled = 0;
      fadeToColor(ledAnalogOne, BLACK, GREEN, 5); 
      //blinkled(3);
      
    }
   // Serial.println(enabled);
  } 

  if (enabled == 1) {
    Serial.println("Armed");
    int curval = analogRead(lightpin);
    //Serial.println(curval);
    
    if ( curval > threshold) {
      Serial.println("all is quiet");
      turnoffalarm();
    } else {
      turnonalarm();
      Serial.println("Sound the ALARM");
    }
  } else {
     turnoffalarm();
     Serial.println("Disarmed");
  }
  
  delay(500);
}

/* Sets the color of the LED to any RGB Value
   led - (int array of three values defining the LEDs pins (led[0] = redPin, led[1] = greenPin, led[2] = bluePin))
   color - (byte array of three values defing an RGB color to display (color[0] = new Red value, color[1] = new Green value, color[2] = new Red value
*/
void setColor(int* led, byte* color){
 for(int i = 0; i < 3; i++){             //iterate through each of the three pins (red green blue)
   analogWrite(led[i], 255 - color[i]);  //set the analog output value of each pin to the input value (ie led[0] (red pin) to 255- color[0] (red input color)
                                         //we use 255 - the value because our RGB LED is common anode, this means a color is full on when we output analogWrite(pin, 0)
                                         //and off when we output analogWrite(pin, 255). 
 }
}

/* A version of setColor that takes a predefined color (neccesary to allow const int pre-defined colors */
void setColor(int* led, const byte* color){
 byte tempByte[] = {color[0], color[1], color[2]};
 setColor(led, tempByte);
}

void fadeToColor(int* led, byte* startColor, byte* endColor, int fadeSpeed){
  int changeRed = endColor[0] - startColor[0];                            //the difference in the two colors for the red channel
  int changeGreen = endColor[1] - startColor[1];                          //the difference in the two colors for the green channel 
  int changeBlue = endColor[2] - startColor[2];                           //the difference in the two colors for the blue channel
  int steps = max(abs(changeRed),max(abs(changeGreen), abs(changeBlue))); //make the number of change steps the maximum channel change
  
  for(int i = 0 ; i < steps; i++){                                        //iterate for the channel with the maximum change
   byte newRed = startColor[0] + (i * changeRed / steps);                 //the newRed intensity dependant on the start intensity and the change determined above
   byte newGreen = startColor[1] + (i * changeGreen / steps);             //the newGreen intensity
   byte newBlue = startColor[2] + (i * changeBlue / steps);               //the newBlue intensity
   byte newColor[] = {newRed, newGreen, newBlue};                         //Define an RGB color array for the new color
   setColor(led, newColor);                                               //Set the LED to the calculated value
   delay(fadeSpeed);                                                      //Delay fadeSpeed milliseconds before going on to the next color
  }
  setColor(led, endColor);                                                //The LED should be at the endColor but set to endColor to avoid rounding errors
}

/* A version of fadeToColor that takes predefined colors (neccesary to allow const int pre-defined colors */
void fadeToColor(int* led, const byte* startColor, const byte* endColor, int fadeSpeed){
  byte tempByte1[] = {startColor[0], startColor[1], startColor[2]};
   byte tempByte2[] = {endColor[0], endColor[1], endColor[2]};
   fadeToColor(led, tempByte1, tempByte2, fadeSpeed);
}

