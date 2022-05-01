
// ------------------------------------------------------ include library -------------------------------------------------------
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "DHT.h"
#include "setting.h"
#include <DS3231.h> 

// ------------------------------------------------------ define value ----------------------------------------------------------
volatile byte state1 = LOW;
volatile byte state2 = LOW;
volatile byte state3 = LOW;
volatile byte stateam = HIGH; // high = auto, 

LiquidCrystal_I2C lcd(0x27, 16,2);
Servo myservo;
DHT dht(DHTPIN, DHTTYPE);
DS3231  rtc(SDA, SCL);
Time T;

int MQ_RESISTOR = 1000;
long RO = 13143;
float MINRSRO = 0.358;
float MAXRSRO = 2.428;
float a = 116.602;
float b = -2.769;
int ppm = 0;
float h = 0;
float t = 0;
int valold = 0;
bool statebuz = false;
bool stateRTC = false;
bool keypadAlto = false;

// ------------------------------------------------------ set up --------------------------------------------------------------
void setup()
{ 
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW);
  myservo.attach(SERVOPIN);
  myservo.write(90);
  rtc.begin();
  lcd.begin();//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Welcome!");
  dht.begin();
  Serial.begin(9600);
  Serial.println("Pre-heat sensor 10 seconds");
  delay(10000);
  Serial.println("Sensor ready start reading");
  pinMode(interruptPin1, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  pinMode(interruptPin3, INPUT_PULLUP);
  pinMode(interruptPin4, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), man_fan, FALLING); 
  attachInterrupt(digitalPinToInterrupt(interruptPin2), man_window, FALLING); 
  attachInterrupt(digitalPinToInterrupt(interruptPin3), man_all, FALLING); 
  attachInterrupt(digitalPinToInterrupt(interruptPin4), man_auto, FALLING); 
  stateam = HIGH;
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);
  pinMode(RPIN, OUTPUT);
  pinMode(GPIN, OUTPUT);
  pinMode(BPIN, OUTPUT);
  pinMode(WINLEDPIN,OUTPUT);
  pinMode(FANLEDPIN,OUTPUT);
  pinMode(AUTOLEDPIN,OUTPUT);
  pinMode(BUZZERPIN, OUTPUT); 
  // set to auto
  digitalWrite(AUTOLEDPIN, HIGH);
  keypadAlto = true;
  // set time 
  rtc.setTime(11, 59, 00);
}
// ------------------------------------------------------ function --------------------------------------------------------------
// function when we have interrupt from keypad

// manual fan
void man_fan() {
  keypadAlto = false;
  digitalWrite(AUTOLEDPIN, LOW);
  Serial.print("IN fan");
  delay(1000);
  state1 = !state1;
  stateam = LOW;
  digitalWrite(RELAYPIN, !state1);
  digitalWrite(FANLEDPIN, state1);
}
// manual window
void man_window() {
  keypadAlto = false;
  digitalWrite(AUTOLEDPIN, LOW);
  Serial.print("IN window");
  delay(1000);
  state2 = !state2;
  digitalWrite(WINLEDPIN, state2);
  stateam = LOW;
  if (state2 == HIGH){
    myservo.write(myservo.read());
    delay(1000);
    myservo.write(180);      
  }
  else if (state2 == LOW){
    myservo.write(myservo.read());
    delay(1000);
    myservo.write(90);      
  }
}
// manual all 
void man_all() {
  keypadAlto = false;
  digitalWrite(AUTOLEDPIN, LOW);
  Serial.print("IN all");
  delay(1000);
  state3 = !state3;
  digitalWrite(FANLEDPIN, state3);
  digitalWrite(WINLEDPIN, state3);
  stateam = LOW;
  if (state3 == HIGH){
    digitalWrite(RELAYPIN, LOW);
    myservo.write(myservo.read());
    delay(1000);
    myservo.write(180);        
  }
  else if(state3 == LOW){
    digitalWrite(RELAYPIN, HIGH);
    myservo.write(myservo.read());
    delay(1000);
    myservo.write(90);    // sets the servo at 180 degree position    
  }
}
// select manual[led off] or auto[led on]
void man_auto() {
  
  Serial.print("IN");
  delay(1000);
  stateam = !stateam;
  if (stateam == HIGH){
    keypadAlto = true;
    digitalWrite(AUTOLEDPIN, HIGH);
  }
  else if (stateam == LOW){
    keypadAlto = false;
    digitalWrite(AUTOLEDPIN, LOW);
  }    
}

// function for read and show data 

// read temperature and humidity
void read_dht(){
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    Serial.print("Humidity: "); 
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(t);
    Serial.println(" *C");
  }
}
// read Co2
void read_co2(){
  int ADCRAW = analogRead(MQ135PIN);
  float RS = ((1024.0*MQ_RESISTOR)/ADCRAW) - MQ_RESISTOR;
  Serial.print("Rs: ");
  Serial.println(RS);
  
  float RSRO = RS/RO;
  Serial.print("Rs/Ro: ");
  Serial.println(RSRO);
  
  if(RSRO < MAXRSRO && RSRO > MINRSRO) {
   ppm = a*pow(RS/RO, b);
   Serial.print("CO2 : ");
   Serial.print(ppm);
   Serial.println(" ppm");
  } 
  else {
   Serial.println("Out of range.");
  }
 delay(5000);
}
// show all data in lcd
void show_lcd(){
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print(rtc.getDateStr());
  lcd.setCursor(4,1);
  lcd.print(rtc.getTimeStr());
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("H "+String(h)+", T "+String(t));
  lcd.setCursor(1,1);
  lcd.print("CO2 : ");
  lcd.print(ppm);
  lcd.print("PPM");
}
// show RGB color 
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(RPIN, red_light_value);
  analogWrite(GPIN, green_light_value);
  analogWrite(BPIN, blue_light_value);
}
// all control in mode auto
void control()
{ 
//  ppm = 1200; for assume ppm 
  if (stateam == HIGH and stateRTC == false){
    if (ppm >= 1500 and statebuz == false){
      statebuz = true;
      digitalWrite(BUZZERPIN , HIGH); 
      delay(1000); 
      digitalWrite(BUZZERPIN , LOW); 
      RGB_color(255, 0, 0); // Red
      Serial.println("inside ppm more than 1500");
      digitalWrite(RELAYPIN, LOW);
      state1 = HIGH;   //fan
      state2 = HIGH;  // window
      myservo.write(myservo.read());
      delay(1000);
      myservo.write(180);      
    }
    else if (ppm < 1500 and ppm >= 1000){ // close win , open fan
      statebuz = false;
      RGB_color(255, 255, 125);
      digitalWrite(RELAYPIN, LOW);
      state1 = LOW;   //fan
      state2 = HIGH;  // window
      myservo.write(myservo.read());
      delay(1000);
      myservo.write(90);
    }
    else if (ppm < 1000){
      Serial.println("ppm low than 1000");
      Serial.println(myservo.read());
      statebuz = false;
      RGB_color(0, 255, 0); // Green
      digitalWrite(RELAYPIN, HIGH);
      state1 = LOW;   //fan
      state2 = LOW;  // window
      myservo.write(myservo.read());
      delay(1000);
      myservo.write(90);
    }
  }
  show_led();
} 
// control follow by RTC in auto mode
// set time to 12.00 -13.00 will close fan and open window
void control_with_RTC(){
  Serial.println(rtc.getTimeStr());
  Serial.println(keypadAlto);
  if (keypadAlto == true){
    T = rtc.getTime();
    if (T.hour >= timeOnHour && T.hour <= timeOffHour){  
      stateRTC = true;
      Serial.println("on");
      state1 = LOW;   //fan
      state2 = HIGH;  // window
      digitalWrite(RELAYPIN, HIGH);
      myservo.write(myservo.read());
      delay(1000);
      myservo.write(180); 
    }
    else {
      Serial.println("off");
      stateRTC = false;
    }
  }
  else{
    stateRTC = false;
  }
  show_led();
}
// show led
void show_led(){
  digitalWrite(FANLEDPIN, state1);
  digitalWrite(WINLEDPIN, state2);
}

// ---------------------------------------------------- main loop ---------------------------------------------------
void loop() {
  control_with_RTC();
  read_co2();
  read_dht();
  show_lcd();
  control(); 
}
