#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>

int trigger = 0;
// your network name also called SSID
char ssid[] = "FiOS-9YS7T"; // Replaced SSID with your WiFi router Name
// your network password
char password[] = "jaw0596utah5610fan";  // Replace PASSWORD with your router password
// MQTTServer to use
char server[] = "io.adafruit.com";
char Adafruit_user_name[] = "Drantickles"; // Replace USER with your Adafruit User Name
char Adafruit_IO_KEY[] = "aio_bBUm70dzsAx5R9a0HMtYFB4ZnKez";  //Replace IO_KEY with the Adafruit IO_KEY   
String Adafruit_Feed_Name = "button_1"; // Replace button_1 with Adafruit feed name id different


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received message for topic ");
  Serial.print(topic);
  Serial.print("with length ");
  Serial.println(length);
  Serial.println("Message:");
  Serial.write(payload, length);
  Serial.println();
}

WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);

void setup()
{
  Serial.begin(115200);
  
  // Start Ethernet with the build in MAC Address
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  // We are connected and have an IP address.
  // Print the WiFi status.
  printWifiStatus();

  /* Enable internal pullup. 
   * Without the pin will float and the example will not work */
  pinMode(PUSH2, INPUT_PULLUP);
  attachInterrupt(PUSH2, sendRequest, FALLING); // Interrupt is fired whenever button is pressed
  
  Serial.println("Push the button!");
}


void loop() {
  if(trigger == 1){
    Adafruit_trigger(Adafruit_IO_KEY, Adafruit_Feed_Name); 
    Serial.println("Push the button!");
    trigger = 0;
  }

}

String Adafruit_trigger(String KEY, String EVENT) {

// Reconnect if the connection was lost
  printWifiStatus();
  if (!client.connected()) {
    Serial.println("Disconnected. Reconnecting....");

        if(!client.connect("energiaClient", Adafruit_user_name, Adafruit_IO_KEY)) {
      Serial.println("Connection failed");
    } else {
      Serial.println("Connection success");
      
      // In the line below ... replace [ADAFRUIT_USERNAME] with the actual name.  Don't include the [].  Change button_1 if you are using another topic
      if(client.subscribe("Drantickles/feeds/button_1")) {
        Serial.println("Subscription successful");
      }
    }
  }
  
  
  // In the line below ... replace [ADAFRUIT_USERNAME] with the actual name.  Don't include the [].  Change button_1 if you are using another topic
    if(client.publish("Drantickles/feeds/button_1","Button was pushed")) {
    Serial.println("Publish success");
  } else {
    Serial.println("Publish failed");
  }

  client.disconnect();
  delay(1000);
  return "SUCCESS";
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void sendRequest(){
  if(trigger == 0){
    trigger = 1;
  }
}
