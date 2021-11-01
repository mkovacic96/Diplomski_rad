#include <WiFi.h>  
#include <PubSubClient.h>  
#include <BluetoothSerial.h>
#include <ESP32_MailClient.h>
  
const char* ssid = "SSID";  
const char* password = "pass";   
const char* mqtt_server = "server";
const int mqttPort = 1883; 
const char* mqttUser = "user";
const char* mqttPassword = "pass";
  
void callback(char* topic, byte* message, unsigned int length);
  
WiFiClient espClient;  
PubSubClient client(espClient);
BluetoothSerial bluetooth;
  
long lastMsg = 0;  
char msg[50];  
int value = 0;  
float temperatura,mbedUlaz;
float grijac,filter,rasvjeta,autoRasvjeta,autoFilter;
char ulaz,warning[]="W", akvarijID[] = "ds18b20";
  
void setup() {
  Serial.begin(9600);
  bluetooth.begin("ESP32board");
  Serial.println("The device started, now you can pair it with bluetooth!");
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi network");    
  client.setServer(mqtt_server,mqttPort);  
  client.setCallback(callback);
  while(!client.connected()){
    Serial.println("Connecting to MQTT...");
    if(client.connect("ESP32",mqttUser,mqttPassword)){
      Serial.println("connected");
    }else{
      Serial.print("failed with state");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe("output/rasvjeta");
  client.subscribe("output/grijac");
  client.subscribe("output/filter");
  client.subscribe("output/autoFilter");
  client.subscribe("output/autoRasvjeta");
  autoRasvjeta=1;
  autoFilter=1;
}

  
void callback(char* topic, byte* message, unsigned int length) {  
      
  String messageTemp;  
    
  for (int i = 0; i < length; i++) {   
    messageTemp += (char)message[i];  
  }   
  
  if (String(topic) == "output/rasvjeta") {    
    if(messageTemp == "on"){  
      Serial.write('T');
      rasvjeta=1;
      autoRasvjeta=0;
    }  
    else if(messageTemp == "off"){  
      Serial.write('F');
      autoRasvjeta=0;
      rasvjeta=0; 
    }  
  }
  if(String(topic)== "output/grijac"){
    if(messageTemp == "on"){
      Serial.write('H');
      grijac=1; 
    }
    else if(messageTemp == "off"){
      Serial.write('h');
      grijac=0;
    }
  }
  if(String(topic)== "output/filter"){
    if(messageTemp == "on"){
      Serial.write('P'); 
      autoFilter=0;
      filter=1;
    }
    else if(messageTemp == "off"){
      Serial.write('p');
      autoFilter=0;
      filter=0;
    }
  }
  if(String(topic)== "output/autoFilter"){
    if(messageTemp == "on"){
      Serial.write('s');
      autoFilter=1; 
    }
  }
  if(String(topic)== "output/autoRasvjeta"){
    if(messageTemp == "on"){
      Serial.write('S'); 
      autoRasvjeta=1;
    }  
  }       
} 

  
void loop() {
  if(Serial.available()){
    
    mbedUlaz=Serial.read();
    if(mbedUlaz>10 && mbedUlaz<35){
      temperatura=mbedUlaz;
      bluetooth.print(temperatura);
    }
    if(mbedUlaz == 'e') client.publish("esp32/warning",warning);
    if(mbedUlaz=='d'){
      rasvjeta=1;
      autoRasvjeta=0;
    } 
    if(mbedUlaz=='l'){
      rasvjeta=0;
      autoRasvjeta=0;
    }
    if(mbedUlaz=='v'){
      filter=0;
      autoFilter=0;
    } 
    if(mbedUlaz=='V'){
      filter=1;
      autoFilter=0;
    }
    if(mbedUlaz=='U') grijac=1;
    if(mbedUlaz=='u') grijac=0;
  }  
  if(bluetooth.available()){
    ulaz=bluetooth.read();
    if(ulaz=='H'){
      Serial.write('H');
      grijac=1;
    }
    if(ulaz=='h'){
      Serial.write('h');
      grijac=0;
    } 
    if(ulaz=='P'){
      Serial.write('P');
      filter=1;
      autoFilter=0;
    } 
    if(ulaz=='p'){
      Serial.write('p');
      filter=0;
      autoFilter=0;
    } 
    if(ulaz=='T'){
      Serial.write('T');
      rasvjeta=1;
      autoRasvjeta=0;
    } 
    if(ulaz=='F'){
      Serial.write('F');
      rasvjeta=0;
      autoRasvjeta=0;
    }
    if(ulaz=='r') Serial.write('r');
    if(ulaz=='g') Serial.write('g');
    if(ulaz=='b') Serial.write('b');
    if(ulaz=='o') Serial.write('o');
    if(ulaz=='c') Serial.write('c');
    if(ulaz=='m') Serial.write('m');
    if(ulaz=='w') Serial.write('w');
    if(ulaz=='0') Serial.write('0');
    if(ulaz=='1') Serial.write('1');
    if(ulaz=='2') Serial.write('2');
    if(ulaz=='3') Serial.write('3');
    if(ulaz=='4') Serial.write('4');
    if(ulaz=='5') Serial.write('5');
    if(ulaz=='6') Serial.write('6');
    if(ulaz=='7') Serial.write('7');
    if(ulaz=='8') Serial.write('8');
    if(ulaz=='9') Serial.write('9');
    if(ulaz=='Q') Serial.write('Q');
    if(ulaz=='q') Serial.write('q');
    if(ulaz=='X') Serial.write('X');
    if(ulaz=='x') Serial.write('x');
    if(ulaz=='Y') Serial.write('Y');
    if(ulaz=='y') Serial.write('y');
    if(ulaz=='Z') Serial.write('Z');
    if(ulaz=='z') Serial.write('z');
    if(ulaz=='S'){
      Serial.write('S');
      autoRasvjeta=1;
    } 
    if(ulaz=='s'){
      Serial.write('s');
      autoFilter=1;
    }
    if(ulaz=='A') Serial.write('A');
    if(ulaz=='a') Serial.write('a');
    if(ulaz=='L') Serial.write('L');
    if(ulaz=='K') Serial.write('K');
    if(ulaz=='k') Serial.write('k');
    if(ulaz=='D') Serial.write('D');
  }

  client.loop();  
  
  long now = millis();  
  if (now - lastMsg > 5000) {  
    lastMsg = now;

    char tempString[8];
    char grijacS[5],filterS[5],rasvjetaS[5],autoRasvjetaS[5],autoFilterS[5];

    dtostrf(temperatura,1,2,tempString);
    dtostrf(grijac,1,1,grijacS);
    dtostrf(filter,1,1,filterS);
    dtostrf(rasvjeta,1,1,rasvjetaS);
    dtostrf(autoRasvjeta,1,1,autoRasvjetaS);
    dtostrf(autoFilter,1,1,autoFilterS);
    client.publish("esp32/akvarij",akvarijID);
    client.publish("esp32/temperatura",tempString);
    client.publish("esp32/grijac", grijacS); 
    client.publish("esp32/filter",filterS);
    client.publish("esp32/rasvjeta",rasvjetaS);
    client.publish("esp32/autoRasvjeta",autoRasvjetaS);
    client.publish("esp32/autoFilter",autoFilterS);
    
  }   
}