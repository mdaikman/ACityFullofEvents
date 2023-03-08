// CSC 485E - A2
// A City Full of Events
// A project by Mohammad Shakirul Islam, Donovan Aikman, Nazanin Shamshiri
// Code, 3d printing, hardware set up by Donovan Aikman
// 3D design by Mohammad Shakirul Islam
// Texts and support material by Nazanin Shamshiri
// Code for Arduino Mega 2560 board, LEDs, buttons, screen, and potentiometer

// This requires teh Adafruit GFX and SSD1306 libraries for display.

////////////////////////////////////////////////////////////////
// Globals

// Screen (Date)
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1  ); 

// LEDs (Venues)
// use a base pin for each venue plus a colour pin for clarity in code.
#define REDPIN          0
#define GREENPIN        1
#define BLUEPIN         2

// NOTE: In this configuration venues 1-6 are 1-6 with 0 being a holder
// NOTE: Venue 6 is also handled on a digital pin and needs to written to differently.
int venuePin[7]         = {0,2,5,8,11,44,47};

// Event colours in rgb vector (red, blue, green, cyan)
int eventColour[4][3]   = {{255,0,0}, {0,255,0}, {0,0,255}, {0,128,128}};

// Venue details to display when time velocity == 0 and a venue button is pushed.
char *venueCapacity [7] = { "0", "200", "500", "1k", "4k", "7k", "20k" };
char *venueCost [7]     = { "0", "$500", "$1,250", "$1,000", "$5,000", "$50k", "$100k" };

// Potentiometer (Time velocity)
// - range definitions over potentiometer (0~1023)
//      + lower than TIME_NEG maps to a negative speed in time
//      + higher than TIME_POS maps to a positive speed in time
//      + anything between is considered a stop position 
// - MONTH delay represents how long it should take roughly to scan through a month at top speed
//      + 2000 ~= 2sec
//      + also sets the slowest delay possible so +/-1 day in 2seconds

#define VELO_PIN        A0
#define TIME_NEG        386
#define TIME_POS        639
#define MONTH_DELAY     2000

// Buttons (Venue detail selection)
int venueButton[7]      = {0, 22, 23, 24, 25, 26, 27};
int selectedVenue       = -1;

// Data variables
// Generally useful data.
int  calendarData[256][5]  = {{0,5,21,4,2},{1,5,25,4,0},{2,5,28,2,3},{3,5,29,2,3},{4,5,31,3,1},{5,6,1,3,1},{6,6,2,3,1},{7,6,3,0,0},{8,6,3,3,1},{9,6,4,0,0},{10,6,4,1,0},{11,6,4,2,0},{12,6,4,3,1},{13,6,5,0,0},{14,6,5,1,0},{15,6,5,2,0},{16,6,5,3,1},{17,6,7,3,1},{18,6,8,3,1},{19,6,9,3,1},{20,6,10,0,0},{21,6,10,3,1},{22,6,11,0,0},{23,6,11,1,0},{24,6,11,2,3},{25,6,11,3,1},{26,6,12,0,0},{27,6,12,1,0},{28,6,12,2,3},{29,6,12,3,1},{30,6,14,3,1},{31,6,15,3,1},{32,6,16,3,1},{33,6,16,4,2},{34,6,17,0,0},{35,6,17,3,1},{36,6,17,4,2},{37,6,18,0,0},{38,6,18,3,1},{39,6,18,4,2},{40,6,19,0,0},{41,6,19,1,0},{42,6,19,3,1},{43,6,19,4,2},{44,6,21,3,1},{45,6,22,3,1},{46,6,22,4,2},{47,6,22,5,0},{48,6,23,1,0},{49,6,23,3,1},{50,6,23,5,0},{51,6,24,0,0},{52,6,24,1,0},{53,6,24,3,1},{54,6,24,5,0},{55,6,25,0,0},{56,6,25,1,0},{57,6,25,3,1},{58,6,25,5,0},{59,6,26,0,0},{60,6,26,1,0},{61,6,26,3,1},{62,6,26,5,0},{63,6,27,1,0},{64,6,28,3,1},{65,6,29,3,1},{66,6,30,3,1},{67,7,1,0,0},{68,7,1,3,1},{69,7,1,5,3},{70,7,2,0,0},{71,7,2,1,0},{72,7,2,3,1},{73,7,3,0,0},{74,7,3,1,0},{75,7,3,3,1},{76,7,4,1,0},{77,7,4,3,1},{78,7,4,3,1},{79,7,5,1,0},{80,7,6,1,0},{81,7,6,2,0},{82,7,6,3,1},{83,7,7,1,0},{84,7,8,0,0},{85,7,8,1,0},{86,7,8,3,1},{87,7,9,0,0},{88,7,9,1,0},{89,7,9,2,3},{90,7,9,3,1},{91,7,10,0,0},{92,7,10,1,0},{93,7,10,1,0},{94,7,10,2,3},{95,7,10,3,1},{96,7,11,1,0},{97,7,12,1,0},{98,7,12,3,1},{99,7,13,1,0},{100,7,13,1,0},{101,7,13,2,0},{102,7,13,3,1},{103,7,14,1,0},{104,7,14,3,1},{105,7,15,0,0},{106,7,15,1,0},{107,7,15,3,1},{108,7,16,0,0},{109,7,16,1,0},{110,7,16,2,3},{111,7,16,3,1},{112,7,16,4,2},{113,7,17,0,0},{114,7,17,1,3},{115,7,17,1,0},{116,7,17,1,0},{117,7,17,3,1},{118,7,18,1,3},{119,7,18,1,0},{120,7,19,1,3},{121,7,19,1,0},{122,7,19,3,1},{123,7,20,1,3},{124,7,20,1,0},{125,7,20,2,0},{126,7,20,3,1},{127,7,21,1,3},{128,7,21,1,0},{129,7,21,3,1},{130,7,22,0,0},{131,7,22,1,3},{132,7,22,1,0},{133,7,22,3,1},{134,7,23,0,0},{135,7,23,1,3},{136,7,23,1,0},{137,7,23,3,1},{138,7,24,1,3},{139,7,24,1,0},{140,7,24,1,0},{141,7,24,3,1},{142,7,25,1,0},{143,7,26,1,0},{144,7,26,3,1},{145,7,27,1,0},{146,7,27,1,0},{147,7,27,2,0},{148,7,27,3,1},{149,7,28,3,1},{150,7,29,0,0},{151,7,29,1,0},{152,7,29,3,1},{153,7,30,0,0},{154,7,30,1,0},{155,7,30,3,1},{156,7,31,0,0},{157,7,31,1,0},{158,7,31,3,1},{159,7,31,5,0},{160,8,1,1,0},{161,8,1,3,1},{162,8,2,1,0},{163,8,2,1,0},{164,8,3,1,0},{165,8,3,2,0},{166,8,4,1,0},{167,8,4,1,0},{168,8,5,0,0},{169,8,5,1,0},{170,8,6,0,0},{171,8,6,1,0},{172,8,7,0,0},{173,8,7,1,0},{174,8,7,1,0},{175,8,7,2,3},{176,8,8,1,0},{177,8,9,1,0},{178,8,9,3,1},{179,8,10,1,0},{180,8,10,1,0},{181,8,10,2,0},{182,8,11,1,0},{183,8,12,0,0},{184,8,12,1,0},{185,8,12,1,2},{186,8,12,5,1},{187,8,13,0,0},{188,8,13,1,0},{189,8,13,1,2},{190,8,13,5,1},{191,8,14,0,0},{192,8,14,1,0},{193,8,14,1,0},{194,8,14,5,1},{195,8,15,1,0},{196,8,16,1,0},{197,8,17,1,0},{198,8,17,2,0},{199,8,18,1,0},{200,8,18,1,0},{201,8,19,0,0},{202,8,19,1,0},{203,8,19,1,2},{204,8,20,0,0},{205,8,20,1,0},{206,8,20,1,2},{207,8,20,5,3},{208,8,21,0,0},{209,8,21,1,0},{210,8,21,1,0},{211,8,22,1,0},{212,8,23,1,0},{213,8,24,1,0},{214,8,24,1,0},{215,8,24,2,0},{216,8,24,5,0},{217,8,25,4,1},{218,8,25,5,0},{219,8,26,0,0},{220,8,26,1,0},{221,8,26,1,2},{222,8,26,5,0},{223,8,27,0,0},{224,8,27,1,0},{225,8,27,1,2},{226,8,27,5,0},{227,8,28,0,0},{228,8,28,1,0},{229,8,28,1,0},{230,8,28,5,0},{231,8,29,1,0},{232,9,1,1,0},{233,9,2,0,0},{234,9,2,1,0},{235,9,2,4,2},{236,9,3,0,0},{237,9,3,1,0},{238,9,4,0,0},{239,9,4,1,0},{240,9,5,1,0},{241,9,8,4,2},{242,9,9,1,0},{243,9,9,3,3},{244,9,10,1,0},{245,9,10,3,3},{246,9,11,1,0},{247,9,12,1,0},{248,9,15,3,0},{249,9,15,3,0},{250,9,16,3,0},{251,9,17,3,0},{252,9,18,3,0},{253,9,22,4,2},{254,9,23,4,1},{255,9,24,4,1}};
char *eventNames[256]      = { "Live show", "Shania Twain", "AfriCa Fest", "AfriCa Fest", "Harbourcats", "Harbourcats", "Harbourcats", "Afternoon Concerts", "Harbourcats", "Afternoon Concerts", "Afternoon Concert", "Folktoria", "Harbourcats", "Afternoon Concerts", "Afternoon Concert", "Folktoria", "Harbourcats", "Harbourcats", "Harbourcats", "Harbourcats", "Afternoon Concerts", "Harbourcats", "Afternoon Concerts", "Afternoon Concert", "Mabuhay!", "Harbourcats", "Afternoon Concerts", "Afternoon Concert", "Mabuhay!", "Harbourcats", "Harbourcats", "Harbourcats", "Harbourcats", "Cique du Soliel", "Afternoon Concerts", "Harbourcats", "Cique du Soliel", "Afternoon Concerts", "Harbourcats", "Cique du Soliel", "Afternoon Concerts", "Afternoon Concert", "Harbourcats", "Cique du Soliel", "Harbourcats", "Harbourcats", "Live show", "Ska & Reggae Festival", "Swingin' in the Shell", "Harbourcats", "Ska & Reggae Festival", "Afternoon Concerts", "Afternoon Concert", "Harbourcats", "Ska & Reggae Festival", "Afternoon Concerts", "Afternoon Concert", "Harbourcats", "Ska & Reggae Festival", "Afternoon Concerts", "Afternoon Concert", "Harbourcats", "Ska & Reggae Festival", "Afternoon Concert", "Harbourcats", "Harbourcats", "Harbourcats", "Afternoon Concerts", "Harbourcats", "Canada Day", "Afternoon Concerts", "Afternoon Concerts", "Harbourcats", "Afternoon Concerts", "Afternoon Concerts", "Harbourcats", "Afternoon Concerts", "Harbourcats", "Harbourcats", "Folk Music Concerts", "Family Fun Music", "Eventide Music", "Harbourcats", "Swingin' in the Shell", "Afternoon Concerts", "Afternoon Concerts", "Harbourcats", "Afternoon Concerts", "Afternoon Concerts", "Festival Mexicano", "Harbourcats", "Afternoon Concerts", "Guitar Jam Singalong", "Afternoon Concerts", "Festival Mexicano", "Harbourcats", "Afternoon Concerts", "Folk Music Concerts", "Harbourcats", "Garden City Harmony", "Family Fun Music", "Eventide Music", "Harbourcats", "Indigenious Concert", "Harbourcats", "Afternoon Concerts", "Afternoon Concerts", "Harbourcats", "Afternoon Concerts", "Afternoon Concerts", "Flamenco Festival", "Harbourcats", "Live show", "Afternoon Concerts", "Flamenco Festival", "Guitar Jam Singalong", "Afternoon Concerts", "Harbourcats", "Flamenco Festival", "Afternoon Concerts", "Flamenco Festival", "Folk Music Concerts", "Harbourcats", "Flamenco Festival", "Family Fun Music", "Eventide Music", "Harbourcats", "Flamenco Festival", "Swingin' in the Shell", "Harbourcats", "Afternoon Concerts", "Flamenco Festival", "Afternoon Concerts", "Harbourcats", "Afternoon Concerts", "Flamenco Festival", "Afternoon Concerts", "Harbourcats", "Flamenco Festival", "Guitar Jam Singalong", "Afternoon Concerts", "Harbourcats", "Afternoon Concerts", "Folk Music Concerts", "Harbourcats", "Garden City Harmony", "Family Fun Music", "Eventide Music", "Harbourcats", "Harbourcats", "Afternoon Concerts", "Afternoon Concerts", "Harbourcats", "Afternoon Concerts", "Afternoon Concerts", "Harbourcats", "Afternoon Concerts", "Afternoon Concerts", "Harbourcats", "Splash Around Town: Finale", "Afternoon Concerts", "Harbourcats", "Garden City Harmony", "Folk Music Concerts", "Family Fun Music", "Eventide Music", "Swingin' in the Shell", "Summer Youth Stages", "Afternoon Concerts", "Afternoon Concerts", "Afternoon Concert", "Afternoon Concerts", "Afternoon Concert", "Guitar Jam Singalong", "Afternoon Concerts", "ViVa Victoria Latin Festival", "Afternoon Concerts", "Folk Music Concerts", "Harbourcats", "Garden City Harmony", "Family Fun Music", "Eventide Music", "Summer Youth Stages", "Afternoon Concerts", "Afternoon Concerts", "Free B Film Festival", "Dragon Boat", "Afternoon Concerts", "Afternoon Concerts", "Free B Film Festival", "Dragon Boat", "Afternoon Concerts", "Guitar Jam Singalong", "Afternoon Concerts", "Dragon Boat", "Afternoon Concerts", "Folk Music Concerts", "Family Fun Music", "Eventide Music", "Swingin' in the Shell", "Summer Youth Stages", "Afternoon Concerts", "Afternoon Concerts", "Free B Film Festival", "Afternoon Concerts", "Afternoon Concerts", "Free B Film Festival", "Function Festival", "Afternoon Concerts", "Guitar Jam Singalong", "Afternoon Concerts", "Afternoon Concerts", "Folk Music Concerts", "Garden City Harmony", "Family Fun Music", "Eventide Music", "Harbour Blues & Roots", "Basketball", "Harbour Blues & Roots", "Afternoon Concerts", "Afternoon Concerts", "Free B Film Festival", "Harbour Blues & Roots", "Afternoon Concerts", "Afternoon Concerts", "Free B Film Festival", "Harbour Blues & Roots", "Afternoon Concerts", "Guitar Jam Singalong", "Afternoon Concerts", "Harbour Blues & Roots", "Afternoon Concerts", "Swingin' in the Shell", "Afternoon Concerts", "Afternoon Concers", "Live show", "Afternoon Concerts", "Afternoon Concers", "Afternoon Concerts", "Afternoon Concers", "Afternoon Concers", "Live show", "Afternoon Concers", "Victoria Beer Festival", "Afternoon Concers", "Victoria Beer Festival", "Afternoon Concers", "Afternoon Concers", "Rifflandia", "Rifflandia", "Rifflandia", "Rifflandia", "Rifflandia", "Live show", "Victoria Royals", "Victoria Royals"};
int  currentMonth          = 8;
int  currentDay            = 12;
int  timeVelocity          = 1000;
// lead with a zero so our month can match the cardinal month number 
int daysInMonth[]          = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


////////////////////////////////////////////////////////////////
// Helper functions


// Reads buttons for venue selection.
int readEventButtons()
{
  if (timeVelocity != 0) {
    selectedVenue = -1;
    return;
  }

  for (int i = 1; i <= 6; i++){
    if (digitalRead(venueButton[i]) == HIGH) {
      selectedVenue = i;
    }
  }
}

// Reads potentiometer for time velocity and delays appropriately.
// Uses the TIME_NEG and TIME_POS values to map backwards in time.
// Between those two values is a rest state .
// https://arduinogetstarted.com/tutorials/arduino-potentiometer
void velocityUpdate()
{
  int knobValue = analogRead(VELO_PIN);
  // Discard a junk value and return later for a good one
  if ((knobValue > 1023) || (knobValue < 0)) return;
  if ((knobValue < TIME_POS) && (knobValue > TIME_NEG)){
    timeVelocity = 0;
  } else if (knobValue < TIME_NEG) {
    // map(value, fromLow, fromHigh, toLow, toHigh)
    knobValue    = TIME_NEG - knobValue;
    timeVelocity = 0 - map(knobValue, 0, TIME_NEG, MONTH_DELAY, (int)MONTH_DELAY/30);
  } else {
    knobValue    = knobValue - TIME_POS;    
    timeVelocity = map(knobValue, 0, 1023 - TIME_POS, MONTH_DELAY, (int)MONTH_DELAY/30);
  }
}

// Advances date if speed velocity is not zero.
void advanceDate()
{
  if (timeVelocity == 0) {
    delay(100);
    return;
  }
  delay(abs(timeVelocity));
  // advance day
  if (timeVelocity > 0) {
    currentDay += 1;
    if (currentDay > daysInMonth[currentMonth]) {
      currentMonth += 1;
      currentDay   =  1;
      if (currentMonth > 9) {
        currentMonth = 5;
      }
    }
  } else if (timeVelocity < 0) {
    currentDay -= 1;
    if (currentDay < 1) {
      currentMonth -= 1;
      currentDay   =  daysInMonth[currentMonth];
      if (currentMonth < 5) {
        currentMonth = 9;
        currentDay   = 30;
      }
    }
  }
}

// Updates venue lights using selected venues, the dates events, and their light values.
void venuesUpdate()
{
  allVenuesOff();
  for (int i = 0; i <= 255; i++){
    if ((calendarData[i][1] == currentMonth) && (calendarData[i][2] == currentDay)){
      lightUpVenue(calendarData[i][3] + 1, calendarData[i][4]);
    }
  }
}

void lightUpVenue(int venue, int event){
  if (venue == 6){
    for (int i=0; i < 3; i++){
      if (eventColour[event][i] != 0) {
        digitalWrite(venuePin[6] + i, HIGH);
      }
    }
  } else {
    for (int i=0; i < 3; i++){
      analogWrite((venuePin[venue] + i), (eventColour[event][i]));
    }
  }
}

// This updates the screen data with the date.
// Or if time velocity is not zero, it will show the venue info of event name (if available), capacity, and cost.
void displayUpdate()
{
  oled.clearDisplay();

  if (selectedVenue == -1){
    oled.setTextSize(4);
    oled.setCursor(0, 0);
    oled.print(currentMonth);
    oled.print("/");
    oled.print(currentDay);
    if (timeVelocity == 0) {
      oled.setCursor(96, 0);
      oled.print("#");
    }
  } else {
    char *currentEvent = getCurrentEvent();
    oled.setTextSize(1);
    oled.setCursor(0, 0);
    oled.print(currentEvent);
    oled.setCursor(0, 16);
    oled.setTextSize(2);
    oled.print(venueCapacity[selectedVenue]);
    oled.print("/");
    oled.print(venueCost[selectedVenue]);
  }
  
  oled.display();
}

char *getCurrentEvent(){
  for (int i = 0; i <= 255; i++){
    if ((calendarData[i][1] == currentMonth) && (calendarData[i][2] == currentDay) && (calendarData[i][3] == selectedVenue - 1)) {
      return eventNames[i];
    }
  }
  return "No events today.";
}

void allVenuesOff() {
  for (int i = 1; i < 6; i++) {
      analogWrite(venuePin[i] + REDPIN, 0);
      analogWrite(venuePin[i] + GREENPIN, 0);
      analogWrite(venuePin[i] + BLUEPIN, 0);
  }
  digitalWrite(venuePin[6] + REDPIN, LOW);
  digitalWrite(venuePin[6] + GREENPIN, LOW);
  digitalWrite(venuePin[6] + BLUEPIN, LOW);  
}

// This lights all lights for ~1 second as test.
void testLED(){
  allVenuesOff();

  for (int i = 0; i < 3; i++) {
    for (int j = 1; j < 6; j++){
      analogWrite(venuePin[j] + i, 255);
    }
    digitalWrite(venuePin[6] + i, HIGH);
    delay(300);
  }
  Serial.println("LEDs OK!");
  allVenuesOff();
}

////////////////////////////////////////////////////////////////
// Main functions
void setup()
{
  Serial.begin(9600);

  // initialize screen
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  } else {
    Serial.print("Display...");
  }

  delay(2000);
  oled.clearDisplay();
  oled.setTextSize(4);          
  oled.setTextColor(WHITE);
  oled.display();
  Serial.println("OK!");

  // Venue Buttons
  for (int i = 1; i <= 6; i++) {
    pinMode(venueButton[i], INPUT);
  }
  Serial.println("Buttons assigned.");

  // LEDs setup and test
  for (int i = 1; i <= 6; i++) {
      pinMode(venuePin[i] + REDPIN, OUTPUT);
      pinMode(venuePin[i] + GREENPIN, OUTPUT);
      pinMode(venuePin[i] + BLUEPIN, OUTPUT);
  }
  testLED();
}

void loop() {
  readEventButtons();
  velocityUpdate();
  venuesUpdate();
  displayUpdate();
  advanceDate();
}
