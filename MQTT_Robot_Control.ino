

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "ZTE_2.4G_9hTyU2";    
const char* password = "N35QywEJ";    
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
String msgStr = "";

int 
M11 = D4, //Connect Motor 1 Blue wire To Pin #13 
M12 = D6, //Connect Motor 1 Violet wire To Pin #12 
M21 = D7, //Connect Motor 2 Blue wire To Pin #11
M22 = D8; //Connect Motor 2 Violet wire To Pin #10 


void forward() {
     digitalWrite(M11, HIGH);
     digitalWrite(M12, LOW); 
     digitalWrite(M21, HIGH);
     digitalWrite(M22, LOW); 
     Serial.println("forward");
     }
     
void backward(){
     digitalWrite(M11, LOW);
     digitalWrite(M12, HIGH); 
     digitalWrite(M21, LOW);
     digitalWrite(M22, HIGH); 
     Serial.println("backward");
     
}
void left(){
     digitalWrite(M11, LOW);
     digitalWrite(M12, LOW); 
     digitalWrite(M21, HIGH);
     digitalWrite(M22, LOW); 
     Serial.println("left");
     }
void right(){
     digitalWrite(M11, HIGH);
     digitalWrite(M12, LOW); 
     digitalWrite(M21, LOW);
     digitalWrite(M22, LOW); 
     Serial.println("right");
     }
void stay(){
     digitalWrite(M11, LOW);
     digitalWrite(M12, LOW); 
     digitalWrite(M21, LOW);
     digitalWrite(M22, LOW); 
     Serial.println("stop");
     }


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    
    Serial.print(".");
    digitalWrite(2,0);
    delay(200);
    digitalWrite(2,1);
    delay(200);
    
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character

  if ((char)payload[0] == '0') {
    forward();   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else if ((char)payload[0] == '1') {
    left();  // Turn the LED off by making the voltage HIGH
  }
  else if ((char)payload[0] == '2') {
    stay();  // Turn the LED off by making the voltage HIGH
  }
  else if ((char)payload[0] == '3') {
    right();  // Turn the LED off by making the voltage HIGH
  }
  else if ((char)payload[0] == '4') {
    backward();  // Turn the LED off by making the voltage HIGH
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
//      client.publish("device/temp", "Temperature value");
//      client.publish("device/humidity", "humidity value");
      
      // ... and resubscribe
      client.subscribe("robot/movement");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
//  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(D4, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);

  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    
    msgStr = "";
    delay(50);
  }
}
