//multi mode!
float tempK=0, tempC=0, tempF=0;    //variables to store temp readings

#define HBA 6 //HBridge A pin Green and RED LED
#define HBB 9 //HBridge B pin FAN and Blue LED PWM
#define Red 2 //HBridge Red LED pin
#define Green 4 //HBridge Green LED pin
#define Fan 5 //HBridge Fan pin
#define Button1 A0 //HBridge Fan pin
#define Button2 A1 //HBridge Fan pin

int button1State = 0;   //buttons
int button2State = 0;

int ledPins[] = {7,8,10,13};  //ledpins on board the arduino.
int ledCount = 4;
int mode = 1;    //default mode is 1
//------------------------------------------------------------
//------------------------------------------------------------

void setup(){
for(int ledPin=2;ledPin<14;ledPin++){
   pinMode(ledPin, OUTPUT) ;// initialize the LED pin as an output: 
}
pinMode(Button1, INPUT);
pinMode(Button2, INPUT);
Serial.begin(9600);
}
//------------------------------------------------------------
//------------------------------------------------------------
 
void loop(){
modecheck();

if(mode ==1){  //mode1 to use the LED strip to visualize music coming from the computer by using Processing
  ledOn(Red);  //to send signals through the COM port to the arduino.
  ledOff(Green);
  fanOff();
  byte brightness;
   if (Serial.available()) {
      // read the most recent byte (which will be from 0 to 255):
      brightness = Serial.read();
      // set the brightness of the LED:
      analogWrite(HBA, brightness);
      analogWrite(HBB, brightness);
  }
}

if(mode == 0){  
  tempRead();
  if(boardLED() != 0){
  for(int ledPin=0;ledPin<boardLED();ledPin++){
     digitalWrite(ledPins[ledPin], HIGH) ;    //turn on some LEDs depends on the temp readings
  }
  for(int ledPin=boardLED();ledPin<ledCount;ledPin++){
     digitalWrite(ledPins[ledPin], LOW) ;      //turn off some LEDs depends on the temp readings
  }
  } else{ if(mode !=1){ledBlink();}    //Blink if temp is too high
  }
  delay(1000);
  }
}
//------------------------------------------------------------
//------------------------------------------------------------

void modecheck(){      //reads the buttonstates to update the mode.
  button1State = digitalRead(Button1);
  button2State = digitalRead(Button2);
  if(mode==0){
    digitalWrite(12,LOW);
    digitalWrite(11,HIGH);
  }else if(mode ==1){
    digitalWrite(12,HIGH);
    digitalWrite(11,LOW);
  }
  
  if(button1State == HIGH){
    mode = 0;
  } else if(button2State == HIGH){
    mode = 1;
  }
}
//------------------------------------------------------------
//------------------------------------------------------------

int boardLED(){  //turns on and off the LED strip colors red and green depending on the temperature
  if(tempC<10){
    if(mode ==1){return 1;}
    fanOff();
    ledOff(Green);ledOff(Red);
    return 1;
  } else if(tempC<20){ 
    if(mode ==1){return 2;}
    fanOff();
    ledOn(Green);ledOff(Red); 
    return 2;
  } else if(tempC<30){
    if(mode ==1){return 3;}
    fanOn(230);
    ledOn(Green);ledOff(Red);
    return 3;
  } else if(tempC<40){
    if(mode ==1){return 4;}
    fanOn(240);
    ledOff(Green);ledOn(Red);
    return 4;
  } else if(tempC>40){
    if(mode ==1){return 0;}
    fanOn(255);
    ledOff(Green);ledOn(Red);
    return 0;
  } 
  
}
//------------------------------------------------------------
//------------------------------------------------------------

void ledBlink(){    //blinks the LEDs
for(int ledPin=0;ledPin<boardLED();ledPin++){
   digitalWrite(ledPins[ledPin], HIGH) ;// initialize the LED pin as an output: 
}
delay(1000);
for(int ledPin=boardLED();ledPin<ledCount;ledPin++){
   digitalWrite(ledPins[ledPin], LOW) ;// initialize the LED pin as an output: 
}
}

//------------------------------------------------------------
//------------------------------------------------------------
//Helper Methods
void fanOn(int spd){
  analogWrite(HBB, spd);
  digitalWrite(Fan, HIGH);  
}
//------------------------------------------------------------
//------------------------------------------------------------
void fanOff(){
  digitalWrite(Fan, LOW);
}
//------------------------------------------------------------
//------------------------------------------------------------
void tempRead(){
//int rawvoltage= analogRead(A4);
//float millivolts= (rawvoltage/1024.0) * 5000;
//tempK= (millivolts/10);
//tempC =  tempK - 273.15;

// read the input on analog pin 0:
//int sensorValue = analogRead(A4);
//float voltage = sensorValue * (5.0 / 1023.0);
//Serial.println(voltage);  
//tempC = ((5000*sensorValue)/1024/10)-273.15;
  
tempK = analogRead(A4) * 0.004882812 * 100-25;    //Read temperature in Kelvins first 
tempC = tempK - 273.15;    //Convert from Kelvin to Celsius 
tempF = ((tempK) * 9 / 5) - 459.67;    //Convert from Kelvin to Fahrenheit  
//Print all the values to Serial
//Serial.print("Kelvin: "); Serial.println(tempK);
Serial.print("Celsius: "); Serial.println(tempC);
//Serial.print("Fahrenheit: "); Serial.println(tempF);
//Serial.println();
}
//------------------------------------------------------------
//------------------------------------------------------------

void ledOn(int pin){
  digitalWrite(pin, LOW);
}
//------------------------------------------------------------
//------------------------------------------------------------
void ledOff(int pin){
  digitalWrite(pin, HIGH);
}
//------------------------------------------------------------
//------------------------------------------------------------

