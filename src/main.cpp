#include <Arduino.h>
#include "config.hpp"
#include "credentials.hpp"

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  wifiConnection = establishWifiConnection();
  client.setServer(mqtt_server, 1883);
  if(wifiConnection){
    mqttConnection = establishMQTTConnection();
  }
}

void loop() {
  if(millis()>sleep_delay){
    sendData();
    //sleep_delay = millis();
    //Sending data before dying
    //Serial.println("Going to sleep");
    ESP.deepSleep(sleepTimeInSeconds * 1000000);
    // It takes a while for the ESP to go to sleep, so this while() prevents it from doing anything else
    while(1){yield();}
  }
  //loops++;
  if(!wifiConnection){ // Kontrola připojení Wifi. Pokud není připojení aktivní, dojde ke kontrole aktuálního stavu
    if(WiFi.status() == WL_CONNECTED){
      wifiConnection = true; // Stav Wifi připojení se změní na funkční
      //getWifiInfo(); // Funkce vypíše informace o Wifi připojení
      mqtt_delay = millis(); // Při připojení dojde k pozdržení pokusu o připojení k MQTT serveru, aby nedošlo k chybě při příliš brzkém pokusu o připojení
    }
  }else{ // Pokud je Wifi aktivní dojde k případnému připojení k MQTT serveru
    if(!mqttConnection && (mqtt_delay + 500 < millis())){   
      mqttConnection = establishMQTTConnection();     // Funkce se pokusí o připojení na MQTT Server
      mqtt_delay = millis();   // Pokud nedošlo k úspěšnému připojení, je další pokus iniciován po 5 sekundách
    }
  }
}

void getWifiInfo(){
  Serial.println("");
	Serial.println("WiFi connected");
	Serial.print("IP address:  ");
	Serial.println(WiFi.localIP());
	Serial.println("");
}

bool establishWifiConnection(){
	/*Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);*/
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	if(WiFi.status() == WL_CONNECTED){
	//getWifiInfo();     //Výpis informací o připojení na sériovku
	return true;
	}
	else {
	//Serial.println("");
	//Serial.println("Couldn't establish Wifi connection at the moment.");
	return false;
	}
}

bool establishMQTTConnection(){
	if(wifiConnection){
    //Serial.print(F("Attempting MQTT connection..."));
    String clientId = mqttClient_hostname;
    if (client.connect(clientId.c_str(), mqttClient_id, mqttClient_password)) {
      //Serial.println("connected");
      // Once connected, publish an announcement...
      String statusToPublish = "alive";
      client.publish("home/temperature/station1/state", statusToPublish.c_str());
      // ... and resubscribe
      return true;
    } else {
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      //Serial.println("try again later");
      return false;
    }
	}
	return false;
}

void sendData(){
  //Serial.printf("Reported value: %f \n", temperature);
  //client.publish("home/temperature/station1/value",String(temperature).c_str());
  delay(300);
}