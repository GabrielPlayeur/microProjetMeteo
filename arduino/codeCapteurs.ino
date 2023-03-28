
const int sensor_temp = 0;
const int sensor_enso = 1;
const int sensor_vent = 2;
const int sensor_pluie = 3;
const int sensor_humi = 4;
const int sensor_pres = 5;

void setup() {
  pinMode(sensor_temp, INPUT);
  pinMode(sensor_enso, INPUT);
  pinMode(sensor_vent, INPUT);
  pinMode(sensor_pluie, INPUT);
  pinMode(sensor_humi, INPUT);
  pinMode(sensor_pres, INPUT);
  Serial.begin(9600);
}

void loop() {
  float temp = cap_temp();
  float enso = cap_enso();
  float vent = cap_vent();
  float pluie = cap_pluie();
  float humi = cap_humi();
  float pres = cap_pres();
  envoie();
  delay(3600000)
}

float cap_temp(){  
  int bit_temp = analogRead(sensor_temp);
  float temp = bit_temp*volt/bit*sensiCapt;
  return temp;
}

float cap_enso(){  
  int bit_enso = analogRead(sensor_enso);
  float enso = bit_enso*volt/bit*sensiCapt;
  return enso;
}

float cap_vent(){  
  int bit_vent = analogRead(sensor_vent);
  float vent = bit_vent*volt/bit*sensiCapt;
  return vent;
}

float cap_pluie(){  
  int bit_pluie = analogRead(sensor_pluie);
  float pluie = bit_pluie*volt/bit*sensiCapt;
  return pluie;
}

float cap_humi(){  
  int bit_humi = analogRead(sensor_humi);
  float humi = bit_humi*volt/bit*sensiCapt;
  return humi;
}

float cap_pres(){  
  int bit_pres = analogRead(sensor_pres);
  float enso = bit_pres*volt/bit*sensiCapt;
  return pres;
}

void envoie() {
  Serial.println("%f#%f#%f#%f#%f#%f", temp, enso, vent, pluie, humi, pres);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "./DHT-SENSOR/DHT.h">

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
  delay(2000);
}

