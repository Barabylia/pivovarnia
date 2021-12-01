#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h> // Библиотека для OTA-прошивки
#include <PubSubClient.h>
#include "DHT.h"
#include <max6675.h>

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




/*
#define DHT1PIN D4     // DHT22 1 data what digital pin we're connected to
#define DHT2PIN D6     // DHT22 2 data what digital pin we're connected to
#define DHTTYPE DHT22  // DHT 22  (AM2302), AM2321

DHT dht1(DHT1PIN, DHTTYPE);
DHT dht2(DHT2PIN, DHTTYPE);
*/


// TERMO #1
#define thermoDO1 D2       // SO
#define thermoCS1 D3       // CS
#define thermoCLK1 D4      // SCK
// TERMO #2
#define thermoDO2 D5       // SO
#define thermoCS2 D6       // CS
#define thermoCLK2 D7      // SCK

MAX6675 thermocouple1(thermoCLK1, thermoCS1, thermoDO1);
MAX6675 thermocouple2(thermoCLK2, thermoCS2, thermoDO2);



void setup() {


/*
  dht1.begin();
  dht2.begin();
*/
int my_test = 0;
  String my_ip; 
  String my_name;
if (my_test == 0) {my_ip = 110; my_name = "110";} else {my_ip = 153; my_name = "L293D";}
  String sketch_name = "NodeMcu_OTA_110_no_MQTT\"";

  
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



/****** MAX6675 *******/
   Serial.print("1_C = "); 
   Serial.print(thermocouple1.readCelsius());
   Serial.print("   1_F = ");
   Serial.println(thermocouple1.readFahrenheit());
 
   // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
   delay(1000);


   //Serial.print("     2_C = "); 
   //Serial.println(thermocouple2.readCelsius());
   //Serial.print("2_F = ");
   //Serial.println(thermocouple2.readFahrenheit());
 
   // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
   delay(1000);









/****** TEMP and HUD *******/
/*
  float hum1 = dht1.readHumidity();
  float hum2 = dht2.readHumidity();

  float temp1 = dht1.readTemperature();
  float temp2 = dht2.readTemperature();

  delay(2500);

  // Check if any reads failed and exit early (to try again).
  if (isnan(hum1) || isnan(temp1)) {
    Serial.println("Failed to read from DHT22 1 sensor!");
    return;
  }
  //float screen = hum1;
  //Serial.println(screen);



 String payload1 =""; payload1 += temp1;
  char send_mytemp1[8];
payload1.toCharArray( send_mytemp1, 8 );
client.publish( "mytemp1", send_mytemp1 );
Serial.println( send_mytemp1 );

 String payload2 =""; payload2 += temp2;
  char send_mytemp2[8];
payload2.toCharArray( send_mytemp2, 8 );
client.publish( "mytemp2", send_mytemp2 );
Serial.println( send_mytemp2 );

Sendmydata("mytemp1",temp1);
Sendmydata("mytemp2",temp2);

Sendmydata("hum1",hum1);
Sendmydata("hum2",hum2);


// Prepare a JSON payload string
 String payload = "{";
payload += "\"temp1\":"; payload += temp1; payload += ",";
payload += "\"hum1\":"; payload += hum1;
payload += "\"temp2\":"; payload += temp2; payload += ",";
payload += "\"hum2\":"; payload += hum2;
payload += "}";

// Send payload
 char attributes[100];
payload.toCharArray( attributes, 100 );
client.publish( "mytemp_all", attributes );
Serial.println( attributes );

//round()

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nTEMPERATURE - HUMIDITY:<br><br>";
  //s += (val)?"high":"low";
  //s += "<p><a href=\"/gpio/1\"><button style=\"background: #4CAF50;\">ON</button></a>&nbsp;<a href=\"/gpio/0\"><button style=\"background: #f44336;\">OFF</button></a></p>";
  //s += "<p>GPIO2 <a href=\"/gpio/0\"><button>100% light</button></a>&nbsp;<a href=\"?pin=50\"><button>50% light</button></a>&nbsp;<a href=\"?pin=10\"><button>10% light</button></a>&nbsp;<a href=\"?pin=OFF2\"><button>OFF</button></a></p>";
  //s += "</html>\n";

  // Prepare the response
  //String s1 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  //s += (val1)?"high":"low";
  //s += "<p><a href=\"/gpio1/1\"><button style=\"background: #4CAF50;\">ON4</button></a>&nbsp;<a href=\"/gpio1/0\"><button style=\"background: #f44336;\">OFF3</button></a></p>";
  //s += "<p>GPIO2 <a href=\"/gpio/0\"><button>100% light</button></a>&nbsp;<a href=\"?pin=50\"><button>50% light</button></a>&nbsp;<a href=\"?pin=10\"><button>10% light</button></a>&nbsp;<a href=\"?pin=OFF2\"><button>OFF</button></a></p>";
  s += (float)temperature; s += " *C - ";
  s += (float)humidity; s += " RH%";
  s += "</html>\n";
*/

String s = "http://192.168.42.1/154.php?temp=21&hum=75";
/*
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n{\"TEMP2\": {\"temp\":";
  s += temp1; s += ",\"hum\":";
  s += hum1; s += "},\"TEMP3\": {\"temp\":";
  s += temp2; s += ",\"hum\":";
  s += hum2; s += "}}";

  //"},\"TEMP3\": {\"temp\":822,\"hum\":13}}";

float aris = 99.25;
//char msg[50];
int lastMsg1 = 0;
*/
////////////////////////////////
    //string.toCharArray(s, len)
    //client.publish("outTopic", temp1);
 
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
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}