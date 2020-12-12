//
// TODO DESCRIPTION
//

#ifndef MAIN
#define TEMPDEBUG 0
#include <DHT.h> //Kirjaston sisällytys
#define DHTPIN 14 // Pinnin määristys
#define DHTTYPE DHT11 //Anturin tyyppi
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  #if TEMPDEBUG
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  #endif
} 

void loop() { 
float temp;
float hum;
tempMeasure(temp,hum);
}
#endif

void tempMeasure(float &temp, float &hum)
{
  //dht.begin();
  hum = dht.readHumidity();        //Lue ilmankosteus
  temp = dht.readTemperature();     //Lue lämpötila
  float hic = dht.computeHeatIndex(temp, hum, false); //Laskee lämpöindeksin Celsius asteina
  
  if (isnan(hum) || isnan(temp)) {
    #if TEMPDEBUG
    Serial.println(F("En pystynyt lukemaan DHT sensoria!"));
    #endif
  }

  #if TEMPDEBUG
  Serial.print(F(" Kosteus: "));
  Serial.print(hum);
  Serial.print(F("%  Lämpötila: "));
  Serial.print(temp);
  Serial.print(F("°C "));
  Serial.print(F("Lämpö indeksi "));
  Serial.print(hic);
  Serial.println("°C");
  #endif
  return;
}
