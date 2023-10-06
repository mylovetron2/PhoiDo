/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 */ 
#include <SPI.h>
#include <ESP8266WiFi.h>
#define sensorPower 12
#define sensorPin 15

byte ledPin = 2;
char ssid[] = "Family F2";           // SSID of your home WiFi
char pass[] = "23456781";            // password of your home WiFi

unsigned long askTimer = 0;

IPAddress server(192,168,100,80);       // the fix IP address of the server
WiFiClient client;

void setup() {
  Serial.begin(115200);  
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);             // only for debug
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
/*  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());*/
  pinMode(ledPin, OUTPUT);
}

int readSensor() {
    digitalWrite(sensorPower, HIGH);    // Turn the sensor ON
    delay(100);                // Allow power to settle
    int val = digitalRead(sensorPin);    // Read the sensor output
    digitalWrite(sensorPower, LOW);        // Turn the sensor OFF
    return val;                // Return the value
}

void loop () {
  client.connect(server, 80);   // Connection to the server
  digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
  int val=readSensor();
  Serial.println(val);
  if(!val){    
    //client.println("Hello server! Are you sleeping?\r");  // sends the message to the server
    client.println("raining\r"); 
    //String answer = client.readStringUntil('\r');   // receives the answer from the sever
  }
  String answer = client.readStringUntil('\r');   // receives the answer from the sever
  Serial.println("from server: " + answer);
  client.flush();
  digitalWrite(ledPin, HIGH);
  delay(2000);                  // client will trigger the communication after two seconds
}
