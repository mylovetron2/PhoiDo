/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Starts WiFi server with fix IP and listens
 *  Receives and sends messages to the client
 *  Communicates: wifi_client_01.ino
 */
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h> //import for websocket
#include <AccelStepper.h>

const int dirPin = 2;
const int stepPin = 0;
const int enable=4;
const int led_forward=5;
const int led_back=14;
int spd= 12000;
int sign=1;

#define motorInterfaceType 1

AccelStepper stepper(motorInterfaceType, stepPin, dirPin);
int state_stepper=1; //0 stop , 1 fordward , 2 back

//byte ledPin = 2;
char ssid[] = "Family F2";               // SSID of your home WiFi
char pass[] = "23456781";               // password of your home WiFi
WiFiServer server(80);                    

IPAddress ip(192, 168, 4, 80);            // IP address of the server
IPAddress gateway(192,168,4,1);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network



WebSocketsServer webSocket = WebSocketsServer(80); //websocket init with port 81

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
//webscket event method
    String cmd = "";
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("Websocket is disconnected");
            //case when Websocket is disconnected
            break;
        case WStype_CONNECTED:{
            //wcase when websocket is connected
            Serial.println("Websocket is connected");
            Serial.println(webSocket.remoteIP(num).toString());
            webSocket.sendTXT(num, "connected");}
            break;
        case WStype_TEXT:
            cmd = "";

            for(int i = 0; i < length; i++) {
                cmd = cmd + (char) payload[i]; 
            } //merging payload to single string
            Serial.println(cmd);

            if(cmd == "poweron"){ //when command from app is "poweron"
                //digitalWrite(ledpin, HIGH);   //make ledpin output to HIGH  
            }else if(cmd == "poweroff"){
                //digitalWrite(ledpin, LOW);    //make ledpin output to LOW on 'pweroff' command.
            }
            else if(cmd=="forward")
            {
                sign=1;
            }
            else if(cmd=="backward")
            {
              sign=-1;
            }
            else if(cmd=="stop")
            {
                sign=0;
            }
            else if(cmd=="run")
            {
                
            }
            stepper.setSpeed(sign * spd);

            //webSocket.sendTXT(num, cmd+":success");
            //send response to mobile, if command is "poweron" then response will be "poweron:success"
            //this response can be used to track down the success of command in mobile app.
            
            break;
        case WStype_FRAGMENT_TEXT_START:
            break;
        case WStype_FRAGMENT_BIN_START:
            break;
        case WStype_BIN:
            hexdump(payload, length);
            break;
        default:
            break;
    }
}

void setup() {
  Serial.begin(115200);                   // only for debug
  
  pinMode(enable,OUTPUT);
  pinMode(led_forward,OUTPUT);
  pinMode(led_back,OUTPUT);

  pinMode(dirPin,OUTPUT);
  digitalWrite(dirPin,HIGH);
  digitalWrite(enable,LOW);
  
  stepper.setMaxSpeed(20000);
  //stepper.setAcceleration(500);
  stepper.setSpeed(12000);
  //stepper.moveTo(500000);

  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
            //server.begin();                         // starts the server
           /*Serial.println("Connected to wifi");
            Serial.print("Status: "); Serial.println(WiFi.status());  // some parameters from the network
            Serial.print("IP: ");     Serial.println(WiFi.localIP());
            Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
            Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
            Serial.print("SSID: "); Serial.println(WiFi.SSID());
            Serial.print("Signal: "); Serial.println(WiFi.RSSI());
            Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());*/
            //pinMode(ledPin, OUTPUT);
  webSocket.begin(); //websocket Begin
  webSocket.onEvent(webSocketEvent); //set Event for websocket
  Serial.println("Websocket is started");
}

void loop () {

  webSocket.loop(); //keep this line on loop method
  //code Motor
  if(sign==-1){
    digitalWrite(led_back,HIGH);
    digitalWrite(led_forward,LOW);
  }
  else if(sign==1){
    digitalWrite(led_back,LOW);
    digitalWrite(led_forward,HIGH);
  }
  else if(sign==0)
  {
    digitalWrite(led_forward,LOW);
    digitalWrite(led_back,LOW);
    digitalWrite(enable,HIGH);
  }

  if(sign!=0)
    digitalWrite(enable,LOW);

  stepper.runSpeed();



  /*
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      digitalWrite(ledPin, LOW);  // to show the communication only (inverted logic)
      Serial.println(".");
      String request = client.readStringUntil('\r');    // receives the message from the client
      Serial.print("From client: "); Serial.println(request);
      client.flush();
      if(request=="raining")
        client.println("Ok client! I'm working.\r"); // sends the answer to the client
      digitalWrite(ledPin, HIGH);
    }
    client.stop();                // tarminates the connection with the client
  }
  // else
  //   Serial.print("nothing");
  */

}
