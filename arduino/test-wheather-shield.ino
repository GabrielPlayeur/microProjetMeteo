
#include <Wire.h> 
#include "SparkFunMPL3115A2.h" 
#include "SparkFun_Si7021_Breakout_Library.h" 

MPL3115A2 myPressure; //Create an instance of the pressure sensor
Weather myHumidity;//Create an instance of the humidity sensor

//Hardware pin definitions
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
const byte STAT_BLUE = 7;
const byte STAT_GREEN = 8;

const byte REFERENCE_3V3 = A3;
const byte LIGHT = A1;
const byte BATT = A2;

// volatiles are subject to modification by IRQs
volatile float rainHour; //60 floating numbers to keep track of 60 minutes of rain
volatile unsigned long raintime, rainlast, raininterval, rain;

//Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
long lastSecond; //The millis counter to see when a second rolls by
int DELAY_MOYENNE= 2000; // ms
int NB_MOYENNE = 3;
String TOKEN = "token";

void rainIRQ()
{
    raintime = millis(); // grab current time
    raininterval = raintime - rainlast; // calculate interval between this and last event

    if (raininterval > 10) // ignore switch-bounce glitches less than 10mS after initial edge
    {
        rainHour += 0.2794; //Increase this minute's amount of rain

        rainlast = raintime; // set up for next event
    }
}

void setup()
{
  Serial.begin(9600);

  //pinMode(STAT_BLUE, OUTPUT); //Status LED Blue
  //pinMode(STAT_GREEN, OUTPUT); //Status LED Green

  pinMode(REFERENCE_3V3, INPUT);
  pinMode(LIGHT, INPUT);

  //Configure the pressure sensor
  myPressure.begin(); // Get sensor online
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags

  attachInterrupt(0, rainIRQ, FALLING);

  //Configure the humidity sensor
  myHumidity.begin();

  lastSecond = millis();
}

void loop()
{
  //Print readings every second
  if (millis() - lastSecond >= 1000)
  {

    lastSecond += 1000;
    String phrase = get_moyenne();

    for(int i = 0 ; i < 5 ; i++)
      Serial.println(phrase);

    rainHour=0;
  }

  delay(1000);
}

float get_light_level()
{
  float operatingVoltage = analogRead(REFERENCE_3V3);

  float lightSensor = analogRead(LIGHT);

  operatingVoltage = 3.3 / operatingVoltage; //The reference voltage is 3.3V

  lightSensor = operatingVoltage * lightSensor;

  return (lightSensor);
}
float get_pluie() {
  return rainHour;
}
float get_humidity(){
  float humidity = myHumidity.getRH();
  return humidity;
  
}
float get_temp(){
  float temp_h = myHumidity.getTempF();
  temp_h = (temp_h-32)*0.5556;
  return temp_h;
  
}
float get_pressure(){
  float pressure = myPressure.readPressure();
  return pressure;
  
}
float get_light(){
  float light_lvl = get_light_level();
  return light_lvl;
  
}
String get_moyenne(){
  float res_temp, res_press, res_hum, res_light;
  float min_temp=10000000 , min_press=10000000 , min_hum=10000000 , min_light=10000000 ;
  float max_temp, max_press, max_hum, max_light;
  float cur_temp, cur_press, cur_hum, cur_light;
  for (int i=0; i < NB_MOYENNE ; i++){
    cur_temp = get_temp();
    min_temp = min(min_temp,cur_temp);
    max_temp = max(max_temp, cur_temp);

    cur_press = get_pressure();
    min_press = min(min_press,cur_press);
    max_press = max(max_press, cur_press);

    cur_hum = get_humidity();
    min_hum = min(min_hum,cur_hum);
    max_hum = max(max_hum, cur_hum);

    cur_light = get_light();
    min_light = min(min_light,cur_light);
    max_light = max(max_light, cur_light);

    delay(DELAY_MOYENNE);
  }
  res_temp= moyenne(min_temp,max_temp);
  res_press= moyenne(min_press,max_press);
  res_hum= moyenne(min_hum,max_hum);
  res_light= moyenne(min_light,max_light);
  res_pluie=get_pluie();
  String phrase = "{'token':'"+TOKEN+"','temperature':"+res_temp+",'pression':"+res_press+",'humidite':"+res_hum+",'luminosite':"+res_light+",'pluie':"+res_pluie+"}" ;
  return phrase;
}
float moyenne(float mini,float maxi){
   return (mini+maxi)*0.5;
}
