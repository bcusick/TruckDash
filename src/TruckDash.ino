#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_RA8875.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>
#include <max6675.h>


// Library only supports hardware SPI at this time
// Connect SCLK to UNO Digital #13 (Hardware SPI clock)
// Connect MISO to UNO Digital #12 (Hardware SPI MISO)
// Connect MOSI to UNO Digital #11 (Hardware SPI MOSI)
#define RA8875_INT 8
#define RA8875_CS 10
#define RA8875_RESET 9
//#define ONE_WIRE_BUS 7 // Data wire is plugged into pin 2 on the Arduino

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);  //define display
//OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance
//DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature.

//thermocouple
int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

//int vccPin = 3;
//int gndPin = 2;
/////////thermocouple

int coolPin = 2;
int CoolantTemp = 0;
int coolantPos = 10;
//int coolantIndex = 0; //index on 1 wire bus


/*
float AmbientTemp = 0;
int ambientPos = 20;
int ambientIndex = 1; //index on 1 wire bus

float intakeTemp = 0;
int iatPos = 30;
int iatIndex = 2; //index on 1 wire bus
*/

float egtTemp = 0;
int egtPos = 40;

int indentColOne = 30;
long prevMillis = 0;
int updateInterval = 500;

void setup()
{
  Serial.begin(9600);
  Serial.println("initialized");

  /* Initialise the display using 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }

/*initialize sensors and display*/
  //sensors.begin(); // Start one wire sensors
  tft.displayOn(true);  //turn on display
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  tft.fillScreen(RA8875_BLACK);
  tft.textMode();// Switch to text mode

  //pinMode(vccPin, OUTPUT); digitalWrite(vccPin, HIGH); //thermo
  //pinMode(gndPin, OUTPUT); digitalWrite(gndPin, LOW); //thermo

/* setup static headings for sensor outputs*/
  tft.textSetCursor(indentColOne, coolantPos);
  tft.textTransparent(RA8875_WHITE);
  tft.textWrite(" : COOLANT");
/*
  tft.textSetCursor(indentColOne, ambientPos);
  tft.textTransparent(RA8875_WHITE);
  tft.textWrite(" : OUTSIDE");

  tft.textSetCursor(indentColOne, iatPos);
  tft.textTransparent(RA8875_WHITE);
  tft.textWrite(" : IAT");
*/

  tft.textSetCursor(indentColOne, egtPos);
  tft.textTransparent(RA8875_WHITE);
  tft.textWrite(" : EGT");
}

void displayValues(float val, int pos)
{
  char charBuffer[4];
  String valString = String(val, 1);
  valString.toCharArray(charBuffer, 4);
  tft.textSetCursor(10, pos);
  tft.textTransparent(RA8875_WHITE);
  tft.textWrite(charBuffer);
  Serial.print(val);  //use for debug
}

void loop(void)
{
  unsigned long currentMillis = millis();

  if (currentMillis - prevMillis >= updateInterval){
    prevMillis = currentMillis;

/*onewire stuff- implement later
    sensors.requestTemperatures(); // Send the command to get temperatures

    CoolantTemp = sensors.getTempCByIndex(coolantIndex);
    AmbientTemp = sensors.getTempCByIndex(ambientIndex);
    intakeTemp  = sensors.getTempCByIndex(iatIndex);

    */
    CoolantTemp = analogRead(coolPin);
    CoolantTemp = map(CoolantTemp, 0, 1023, -40, 120); //need to verify from Toyota manual
    egtTemp     = thermocouple.readCelsius();

    displayValues(CoolantTemp, coolantPos);
    //displayValues(AmbientTemp, ambientPos);
    //displayValues(intakeTemp, iatPos);
    displayValues(egtTemp, egtPos);
  }
}

