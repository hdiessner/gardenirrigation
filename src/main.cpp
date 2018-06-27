#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <avr/wdt.h>

#define DHTPIN         A7 // Because of wire length
#define SOIL1         A13
#define SOIL2         A14
#define LEAFSENSOR    A15
#define REL1           22
#define REL2           24
#define REL3           26 
#define REL4           28
#define WATERLEVEL1    31
#define WATERLEVEL2    33
#define WATERLEVEL3    35
#define WATERLEVEL4    37
#define WATERLEVEL5    39
#define MOTION         41
#define DOOR           43
#define DONOTUSE1      51 // MQTT Disconnects when used
#define SENSORLOOP 100000

uint32_t       sensorLoopCount          = 0;
byte           mac[]                    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress      ip(192, 168, 2, 236);
IPAddress      server(192, 168, 2, 5);
DHT_Unified    dht(DHTPIN, DHT22);
EthernetClient ethClient;
PubSubClient   client(ethClient);
char           returnmsg[100];

///////////////////////////////
/////// MQTT Handling /////////
///////////////////////////////
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("gartenErection")) {
      Serial.println("connected");
      client.publish("garden/waterbox/system", "startup");
      client.subscribe("garden/irrigation/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 seconds");
      delay(1000);
    }
  }
}

void callback(char* topicchars, byte* payloadbytes, unsigned int length) {
  String topic = String(topicchars);
  char payload[100];
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (uint8_t i = 0; i < length; i++) {
    payload[i] = payloadbytes[i];
  }
  payload[length] = '\0';
  Serial.println(payload);
  //double dval = atof(vchar);
   
  if (String("garden/irrigation/rainpump") == topic){

    if(String("on") == String(payload)){
      digitalWrite(REL1, false);
    }
    
    if(String("off") == String(payload)){
      Serial.println("off");
      digitalWrite(REL1, true);
    }
    
  }

  if (String("garden/irrigation/rel2") == topic){

    if(String("on") == String(payload)){
      digitalWrite(REL2, false);
    }
    
    if(String("off") == String(payload)){
      Serial.println("off");
      digitalWrite(REL2, true);
    }
    
  }

  if (String("garden/irrigation/rel3") == topic){

    if(String("on") == String(payload)){
      digitalWrite(REL3, false);
    }
    
    if(String("off") == String(payload)){
      Serial.println("off");
      digitalWrite(REL3, true);
    }
    
  }

  if (String("garden/irrigation/rel4") == topic){

    if(String("on") == String(payload)){
      digitalWrite(REL4, false);
    }
    
    if(String("off") == String(payload)){
      Serial.println("off");
      digitalWrite(REL4, true);
    }
    
  }

}

void prepareIntVal(int val){
  snprintf (returnmsg, sizeof(returnmsg), "%d", val);
}

void prepareFloatVal(float val){
  int  vala = floor(val); 
  int  valb = (val-vala) * pow(10,2);  
  snprintf (returnmsg, sizeof(returnmsg), "%d.%02d", vala, valb);
}

/////////////////////////////////
////// SENSOR Handling //////////
/////////////////////////////////
float getAnalog(int pin){
  uint64_t val = 0;
  for(uint8_t i = 0; i < 200; i++){
    val += analogRead(pin);
  }
  val = floor(val/200);
  return val;
}

void publishTemperature(){
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  prepareFloatVal(event.temperature);
  client.publish("garden/waterbox/temperature", returnmsg);
}

void publishHumidity(){
  sensors_event_t event;  
  dht.humidity().getEvent(&event);
  prepareFloatVal(event.relative_humidity);
  client.publish("garden/waterbox/humidity", returnmsg);
}

void publishWaterlevel(){
  uint8_t level = 0;
  if (!digitalRead(WATERLEVEL1)){
      level = 100;
  }else{
    if (!digitalRead(WATERLEVEL2)){
      level = 75;
    }else{
      if (!digitalRead(WATERLEVEL3)){
        level = 50;
      }else{
        if (!digitalRead(WATERLEVEL4)){
          level = 25;
        }else{
          if (!digitalRead(WATERLEVEL5)){
            level = 15;
          }else{
            level = 0;
          }
        } 
      } 
    }    
  }
  prepareIntVal(level);
  client.publish("garden/irrigation/waterlevel", returnmsg);
}

void publishLeaf(){
  prepareFloatVal(getAnalog(LEAFSENSOR));
  client.publish("garden/irrigation/leaf", returnmsg);
}

void publishSoilfront(){
  prepareFloatVal(getAnalog(SOIL1));
  client.publish("garden/irrigation/soilfront", returnmsg);
}

void publishSoilback(){
  prepareFloatVal(getAnalog(SOIL2));
  client.publish("garden/irrigation/soilback", returnmsg);
}

void publishDoor(){
  if(digitalRead(DOOR)){
    client.publish("garden/waterbox/door", "OPEN");
  } else {
    client.publish("garden/waterbox/door", "CLOSED");
  }
}

void publishMotion(){
  if(digitalRead(DOOR)){
    client.publish("garden/cellarentrance/motion", "now");
  }
}

void publishSensors(){

  publishTemperature();
  wdt_reset();
  client.loop();
  
  publishHumidity();
  wdt_reset();
  client.loop();
  
  publishWaterlevel();  
  wdt_reset();
  client.loop();

  publishLeaf();
  wdt_reset();
  client.loop();
  
  publishSoilfront();
  wdt_reset();
  client.loop();
  
  publishSoilback();
  wdt_reset();
  client.loop();
  
  publishDoor();
  wdt_reset();
  client.loop();
  
  publishMotion();
  wdt_reset();
  client.loop();
  
  wdt_reset();
  client.loop();
}

////////////////////////
//////// MAIN  /////////
////////////////////////
void loop(){
  
  // Watchdog
  wdt_reset();

  // MQTT Handling
  if (!client.connected()) {
    reconnect();
    yield();
    wdt_reset();
  }
  yield();
  wdt_reset();
  client.loop();
  wdt_reset();
  yield();
  // Sensor Handling
  if (sensorLoopCount++ > SENSORLOOP){
      publishSensors();
      sensorLoopCount = 0;
  }
}

//////////////////////////////////
////////  System setup  //////////
//////////////////////////////////

void configureGPIOs(){
  pinMode(REL1, OUTPUT);
  pinMode(REL2, OUTPUT);
  pinMode(REL3, OUTPUT);
  pinMode(REL4, OUTPUT);
  digitalWrite(REL1, true);
  digitalWrite(REL2, true);
  digitalWrite(REL3, true);
  digitalWrite(REL4, true);
  pinMode(WATERLEVEL1, INPUT_PULLUP);
  pinMode(WATERLEVEL2, INPUT_PULLUP);
  pinMode(WATERLEVEL3, INPUT_PULLUP);
  pinMode(WATERLEVEL4, INPUT_PULLUP);
  pinMode(WATERLEVEL5, INPUT_PULLUP);
  pinMode(DOOR,        INPUT_PULLUP);
  pinMode(MOTION,      INPUT);
}


void setup(){
  wdt_enable(WDTO_8S);
  wdt_reset();
  configureGPIOs();
  Serial.begin(57600);
  dht.begin();
  wdt_reset();
  Ethernet.begin(mac, ip);
  wdt_reset();
  delay(1500); // Give the network some time to startup
  wdt_reset();
  client.setServer(server, 1883);
  client.setCallback(callback);
  wdt_reset();
}
