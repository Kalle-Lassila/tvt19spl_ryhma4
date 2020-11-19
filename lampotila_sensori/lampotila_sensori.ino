#include <DHT.h> //Kirjaston sisällytys
#define DHTPIN 2 // Pinnin määristys
#define DHTTYPE DHT11 //Anturin tyyppi
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
} 

void loop() {
 float h = dht.readHumidity();        //Lue ilmankosteus
 float t = dht.readTemperature();     //Lue lämpötila
 float hic = dht.computeHeatIndex(t, h, false); //Laskee lämpöindeksin Celsius asteina
 
 if (isnan(h) || isnan(t)) {
    Serial.println(F("En pystynyt lukemaan DHT sensoria!"));
    return;
}
  Serial.print(F(" Kosteus: "));
  Serial.print(h);
  Serial.print(F("%  Lämpötila: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(F("Lämpö indeksi "));
  Serial.print(hic);
  Serial.println("°C");
}
