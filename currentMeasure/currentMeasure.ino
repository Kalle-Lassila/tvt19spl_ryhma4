
int value = 0;
int x = 0;
unsigned long StartTime = millis();


void setup() {
  Serial.begin(9600);
  pinMode(A1,INPUT);
}

void loop() {
  value = analogRead(A1);
  x = map(value, 0, 1024, 0, 5000);
  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;
  int jannite = x*3;
  Serial.print(ElapsedTime);
  //Serial.print("\t");
  //Serial.print(x);
  Serial.print("\t");
  Serial.println(jannite);
  delay(100);  
}
