#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h> // Библиотека для OTA-прошивки
#include <PubSubClient.h>
#include <DHT.h>
#include <max6675.h>
#include <OneWire.h>
#include <DallasTemperature.h>


// Wi-fi connection
IPAddress ip(192,168,31,110); 
IPAddress gateway(192,168,31,1); 
IPAddress subnet(255,255,255,0);

const char* ssid = "oksana"; //Имя точки доступа WIFI 
const char* password = "12061964"; //пароль точки доступа WIFI
const char* mqtt_server = "192.168.31.110";

WiFiClient espClient;
PubSubClient client(espClient);

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

/****** DS18B20 Temperature *******/
DeviceAddress Term01 = { 0x28, 0xFF, 0x74, 0x6E, 0xB0, 0x16, 0x4, 0x24 }; //black
DeviceAddress Term02   = { 0x28, 0xFF, 0x4D, 0x2F, 0xB0, 0x16, 0x5, 0x62 }; //red
DeviceAddress Term03   = { 0x28, 0xFF, 0xB9, 0x7F, 0xB0, 0x16, 0x4, 0x5D }; //green

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS D2
//#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

/****** Buttons + Relays *******/
#define BUTTON_PIN D4       // 
#define LED_PIN D5       // 
int relay1=0;
int relay2=0;
int relay3=0;
int counter=0;




/*
// TERMO #1
#define thermoDO1 D2       // SO
#define thermoCS1 D3       // CS
#define thermoCLK1 D4      // SCK
// TERMO #2
#define thermoDO2 D6       // SO
#define thermoCS2 D7       // CS
#define thermoCLK2 D8      // SCK

MAX6675 thermocouple1(thermoCLK1, thermoCS1, thermoDO1);
//MAX6675 thermocouple2(thermoCLK2, thermoCS2, thermoDO2);
*/
int my_MAX6675 = 0;
int my_DHT22 = 0;
int my_DS18B20 = 1;

void setup() {
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(BUTTON_PIN, HIGH);



int my_test = 0;


  String my_ip; 
  String my_name;
if (my_test == 0) {my_ip = 110; my_name = "110";} else {my_ip = 153; my_name = "L293D";}
  String sketch_name = "110_NodeMcu_OTA_no_MQTT\"";

  
  Serial.begin(115200);


  // Connect to WiFi network
  Serial.println();
  Serial.println();
  //Sketch name
  Serial.println("Sketch name is \"" + sketch_name);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
/*
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }  
 */
  WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());


  ArduinoOTA.setHostname("110-NodeMcu_OTA_110_no_MQTT"); // Задаем имя сетевого порта
  //ArduinoOTA.setPort(8266);
  //ArduinoOTA.setPassword((const char *)"0000"); // Задаем пароль доступа для удаленной прошивки
  ArduinoOTA.begin(); // Инициализируем OTA
    //Serial.println(WiFi.gateway());
/*
    ////////for vs code////////
upload_protocol = espota
upload_port = 192.168.31.110



     client.setServer(mqtt_server, 1883);
     client.setCallback(callback);
     */

      // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
}
/*
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");


  
   for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}
*/

/*
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
*/

void Sendmydata(String topic,int data) {
   String my_string =""; my_string += data;
  char send_data[8];
my_string.toCharArray( send_data, 8 );
  char send_topic[80];
topic.toCharArray( send_topic, 80 );


client.publish( send_topic, send_data );
//Serial.println( send_data );
  //Serial.println(topic);
  }


void loop() {
ArduinoOTA.handle(); // Всегда готовы к прошивке
/* 
 if (!client.connected()) {
    reconnect();
  }
  client.loop();
 */ 

/****** Buttons + Relays *******/

if(!digitalRead(BUTTON_PIN) == HIGH && relay1 == 0)
{
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  relay1 = 1;
  delay(1000);
}

if(!digitalRead(BUTTON_PIN) == LOW && relay1 == 1)
{
  relay1 = 0;
  counter = counter+1;
  //Serial.println("relay1 = " + String(relay1));
Serial.println("counter = " + String(counter));
}





/****** DS18B20 *******/
if (my_DS18B20 == 1) {
  sensors.requestTemperatures();

  
   Serial.print("Term01 C: ");
  float tempC1 = sensors.getTempC(Term01);
  Serial.print(tempC1);
  Serial.print(" black");
  Serial.println();
  
  Serial.print("Term02 C: ");
  float tempC2 = sensors.getTempC(Term02);
  Serial.print(tempC2);
  Serial.print(" red");
  Serial.println();

  Serial.print("Term03 C: ");
  float tempC3 = sensors.getTempC(Term03);
  Serial.print(tempC3);
  Serial.print(" green");
  Serial.println();
  
delay (2000);
}





/****** MAX6675 *******/
if (my_MAX6675 == 1) {
   Serial.print("1_C = "); 
   //Serial.print(thermocouple1.readCelsius());
   Serial.print("   1_F = ");
   //Serial.println(thermocouple1.readFahrenheit());
 
   // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
   delay(1000);
}

   //Serial.print("     2_C = "); 
   //Serial.println(thermocouple2.readCelsius());
   //Serial.print("2_F = ");
   //Serial.println(thermocouple2.readFahrenheit());
 
   // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
   //delay(1000);










 
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  // Send the response to the client
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}