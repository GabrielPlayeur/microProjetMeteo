#include <Wire.h> 
#include "SparkFunMPL3115A2.h" 
#include "SparkFun_Si7021_Breakout_Library.h" 

MPL3115A2 myPressure; 
Weather myHumidity;

//Hardware pin definitions
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
const byte STAT_BLUE = 7;
const byte STAT_GREEN = 8;
const byte RAIN = 2;
const byte WSPEED = 3;

const byte REFERENCE_3V3 = A3;
const byte LIGHT = A1;
const byte BATT = A2;

// volatiles are subject to modification by IRQs
volatile float rainHour; 
volatile unsigned long raintime, rainlast, raininterval, rain;
long lastWindCheck = 0;
volatile long lastWindIRQ = 0;
volatile byte windClicks = 0;

//Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
long lastSecond; 
int DELAY_MOYENNE= 2000; // ms
int NB_MOYENNE = 3;
String TOKEN = "VlJhkpCZiqmDLeJvTYPCEoQVrlzpJvcSFTvTWjuVsEZAftRNiUdhiXKdpQMJ";


void wspeedIRQ()

{
    if (millis() - lastWindIRQ > 10) 
    {
        lastWindIRQ = millis(); 
        windClicks++; //There is 2.4Kmh for each click per second.
    }
}
void rainIRQ()
{
    raintime = millis(); 
    raininterval = raintime - rainlast; 

    if (raininterval > 10) 
    {
        rainHour += 0.2794; 

        rainlast = raintime; 
    }
}

void setup()
{
  Serial.begin(9600);

  pinMode(REFERENCE_3V3, INPUT);
  pinMode(LIGHT, INPUT);

  pinMode(RAIN, INPUT_PULLUP); 
  pinMode(WSPEED, INPUT_PULLUP); 

  //Configure the pressure sensor
  myPressure.begin(); // Get sensor online
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  myPressure.setOversampleRate(7); 
  myPressure.enableEventFlags(); 

  attachInterrupt(0, rainIRQ, FALLING);
  attachInterrupt(1, wspeedIRQ, FALLING);

  interrupts();

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

  delay(10000);
}

float get_light_level()
{
  float operatingVoltage = analogRead(REFERENCE_3V3);

  float lightSensor = analogRead(LIGHT);

  operatingVoltage = 3.3 / operatingVoltage;

  lightSensor = operatingVoltage * lightSensor;

  return (lightSensor);
}
float get_wind_speed()
{
    float deltaTime = millis() - lastWindCheck; 

    deltaTime /= 1000.0;
    float windSpeed = (float)windClicks / deltaTime;
  
    windClicks = 0; 
    lastWindCheck = millis();

    windSpeed *= 2.4; 

    return windSpeed;
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
  float res_temp, res_press, res_hum, res_light, res_pluie, res_vent;
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
  res_vent=get_wind_speed();
  String phrase = "{'token':'"+TOKEN+"','temperature':"+res_temp+",'pression':"+res_press+",'humidite':"+res_hum+",'luminosite':"+res_light+",'pluie':"+res_pluie+",'vent':"+res_vent+"}";
  return phrase;
}
float moyenne(float mini,float maxi){
   return (mini+maxi)*0.5;
}
