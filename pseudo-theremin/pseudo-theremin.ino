int speakerpin = 2;
int photocellpin=0;
void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
int reading=analogRead(photocellpin);
int pitch=200+reading/4;
Serial.println(pitch);
tone(speakerpin,pitch);
}
