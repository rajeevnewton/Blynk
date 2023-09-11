/*************************************************************

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  WARNING :
  For this example you'll need Adafruit DHT sensor libraries:
    https://github.com/adafruit/Adafruit_Sensor
    https://github.com/adafruit/DHT-sensor-library

  App dashboard setup:
    Value Display widget attached to V5
    Value Display widget attached to V6
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID   "TMPL3baaYeAdL"
#define BLYNK_TEMPLATE_NAME "D1Mini"
#define BLYNK_AUTH_TOKEN    "7t7WshRSzqQXFuP882jL6mW4Zl5m-QrI"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Servo.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Dinesh";
char pass[] = "Dinesh@517";

Servo servo;

#define DHTPIN 2          // What digital pin we're connected to
#define relay1 16
#define relay2 5
#define soil   A0
#define irsensor  4
// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("Humidity: ");
  Serial.print(t);
  Serial.println();

  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
}

void myTimerEvent()
{
  // int moisture = analogRead(soil);
  int moisture = map(analogRead(soil),1023,0,0,100);
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V4, moisture);
}

void irSensor()
{
  // int irval = digitalRead(irsensor);
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  int irval = map(digitalRead(irsensor),0,1,1,0);
  Serial.print("ir sensor");
  Serial.println(irval);
  Blynk.virtualWrite(V3, irval);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  pinMode(soil,INPUT);
  servo.attach(13);
  pinMode(irsensor,INPUT);
  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,HIGH);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(1000L, myTimerEvent);
  timer.setInterval(1000L, irSensor);
}

BLYNK_WRITE(V10)
{ 
  int servoVal = param.asInt();
  Serial.print("V1 Servo value is: ");
  Serial.println(servoVal);

  servo.write(param.asInt());
}

BLYNK_WRITE(V1)
{
  int relay1val = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V1 value is: ");
  Serial.println(relay1val);

  if(relay1val == 1){
    digitalWrite(relay1,LOW);
    Serial.println("relay 1 on");
  }
  else{
    digitalWrite(relay1,HIGH);
    Serial.println("relay 1 off");
  } 
}

BLYNK_WRITE(V2)
{
  int relay2val = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V2 value is: ");
  Serial.println(relay2val);
  
  if(relay2val == 1){
    digitalWrite(relay2,LOW);
    Serial.println("relay 2 on");

  }
  else{
    digitalWrite(relay2,HIGH);
    Serial.println("relay 2 off");
  } 
}


void loop()
{
  Blynk.run();
  timer.run();
}
