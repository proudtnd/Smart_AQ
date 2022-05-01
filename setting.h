#define ANALOGPIN A4    //  Define Analog PIN on Arduino Board
#define SERVOPIN 25 // 180 open , 90 close
#define MQ135PIN A4
#define BUZZERPIN 7 // when fan open buzzer on
#define DHTPIN 9 
#define DHTTYPE DHT11 // DHT 11

const int RELAYPIN = 8; // active low
const byte interruptPin1 = 18;
const byte interruptPin2 = 19;
const byte interruptPin3 = 2;
const byte interruptPin4 = 3;
const int RPIN = 6;
const int GPIN = 5;
const int BPIN = 4;
const int WINLEDPIN = 22;
const int FANLEDPIN = 23;
const int AUTOLEDPIN = 24;

// set time to open and close window - fan
const int timeOnHour = 12;
const int timeOffHour = 13;

int r1 = 33, r2 = 32, r3 = 31, r4 = 30;
