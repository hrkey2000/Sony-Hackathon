#include <WiFi.h>
#include <WiFiClientSecure.h>

// Set Wi-Fi
const char* ssid     = “SSID”; //Change
const char* password = “PASSWORD”; //Change

// Set LINE Notify
const char* host = “notify-api.line.me”;

//LINE Notify Access Token
const char* token = “Access Token”; //Change

//Notification content
const char* message = “ボタンが押されました!“;

// Set switch PIN
int sw_pin = 4; //Change
void setup()
{
    // Set switch PIN to input
    pinMode(sw_pin,INPUT);
    // Communication speed of serial monitor
    Serial.begin(115200);
    Serial.println();
    Serial.print(“Connecting to “);
    Serial.println(ssid);
    // Connect Wi-Fi
    WiFi.begin(ssid, password);
    // Check the Wi-Fi connection status
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(“.”);
    }
    Serial.println(“”);
    Serial.println(“WiFi connected.“);
    Serial.println(“IP address: “);
    Serial.println(WiFi.localIP());
}
// LINE Notification
void send_line() {
  // Library for accessing HTTPS (SSL communication)
  WiFiClientSecure client;
  // Required when connecting without validating the server certificate
  client.setInsecure();
  Serial.println(“Try”);
  // SSL connection to Line's API server (port 443: https)
  if (!client.connect(host, 443)) {
    Serial.println(“Connection failed”);
    return;
  }
  Serial.println(“Connected”);
  // Send request
  String query = String(“message=“) + String(message);
  String request = String(“”) +
               “POST /api/notify HTTP/1.1\r\n” +
               “Host: ” + host + “\r\n” +
               “Authorization: Bearer ” + token + “\r\n” +
               “Content-Length: ” + String(query.length()) +  “\r\n” +
               “Content-Type: application/x-www-form-urlencoded\r\n\r\n” +
                query + “\r\n”;
  client.print(request);
  // Wait until reception is completed
  while (client.connected()) {
    String line = client.readStringUntil(‘\n’);
    if (line == “\r”) {
      break;
    }
  }
  String line = client.readStringUntil(‘\n’);
  Serial.println(line);
}
// Switch variable declaration
int sw_val = 0;
void loop(){
  // Read the switch status
  sw_val = digitalRead(sw_pin);
  // When the switch is ON
  if (sw_val) {
    // Send a request to Line
    send_line();
  }
}
