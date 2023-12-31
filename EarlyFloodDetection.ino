#include <Adafruit_Sensor.h>
#include <DHT.h> 

#define trigPin 3       
#define echoPin 4
#define DHTPin 5
#define DHTType DHT11
volatile int flow_frequency; 
unsigned int l_hour;
unsigned char flowsensor = 2; 
unsigned long currentTime;
unsigned long cloopTime;
const int buzzer = 7;
void flow () 
{
  flow_frequency++;
}

long duration;         
int distance;          
float speedofsound;

DHT dht = DHT(DHTPin, DHTType); 

void setup() {

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  pinMode(buzzer, OUTPUT);

  dht.begin(); 

  pinMode(flowsensor, INPUT);
  digitalWrite(flowsensor, HIGH); 

  attachInterrupt(0, flow, RISING);
  sei(); 
  currentTime = millis();
  cloopTime = currentTime;
  Serial.begin(115200);
}

void loop() {
  int value = analogRead(A0);

  if (value > 700) {
    Serial.print("High Water Level||");
  }
  else if ((value > 300) && (value <= 700)) {
    Serial.print("Normal Water Level||");
  }
  else {
    Serial.print("Low Water Level||");

    delay(2000);
  }
    currentTime = millis();
    
    if (currentTime >= (cloopTime + 1000))
    {
      cloopTime = currentTime; 
     
      l_hour = (flow_frequency * 60 / 7.5); 
      flow_frequency = 0; 
      Serial.print(" Water flowrate: ");
      Serial.print(l_hour, DEC); 
      Serial.print(" L/hour");
    }

    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);      

    digitalWrite(trigPin, HIGH);  
    delayMicroseconds(10);       
    digitalWrite(trigPin, LOW);  

    duration = pulseIn(echoPin, HIGH); 

    float temperature = dht.readTemperature();

    speedofsound = 331.3 + (0.606 * temperature); 
    distance =11. - duration * (speedofsound / 10000) / 2; 


    Serial.print(" ||Temperature: ");
    Serial.print(temperature);     
    Serial.print(" ||Celsius:");      
    Serial.print(" ||Water Level: ");
    Serial.print(distance);       
    Serial.println("cm"); 
    if(distance>=3 && l_hour >=120 && value >=400){
      Serial.println(" Flood is possible");
      tone(buzzer, 1000); 
      delay(5000);        
      noTone(buzzer);     
    }
    delay(2000);
  send_data();
}

void send_data()
{
//  int sensorValue = analogRead(A0);
  mySerial.print('*'); // Starting char
  mySerial.print(dht.readTemperature(), 0); //2 digit data
  mySerial.print(dht.readHumidity(), 0); //2 digit data
  // mySerial.print(w_level,0); //2 digit data
//  mySerial.print(bmp.readAltitude(seaLevelPressure_hPa * 100) * 100); //5 digit data
//  mySerial.print(sensorValue); //4 digit data
//  mySerial.print(map(analogRead(LDR), 0, 1024, 0, 99)); //2 digit data
//  mySerial.print(map(analogRead(mq135_pin), 0, 1024, 99, 0)); //2 digit data
  mySerial.println('#'); // Ending char
}
