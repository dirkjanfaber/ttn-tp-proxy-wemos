#include <credentials.h>

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

MDNSResponder mdns;

ESP8266WebServer server(80);

String webPage = "";
String body = "";

int gpio0_pin = 0;
int gpio2_pin = 2;

void setup(void){
  webPage += "<!doctype html><html lang=\"en\">";
  webPage += "<head>\n<meta charset=\"utf-8\">";
  webPage += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">";
  webPage += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\">";
  webPage += "<title>TTN-TP Proxy</title><body><h1>TTN-TP Proxy</h1>";
  webPage += "<p>Post your json to <strong>/ttn/</strong> and it magically forwards the payload to thingspeak.</p>";
  webPage += "</body></html>";

  // preparing GPIOs
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);

  delay(1000);
  Serial.begin(115200);
  WiFi.begin(WIFI, WIFIPW);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("ttn-tp-proxy", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  server.on("/ttn",HTTP_POST, [](){
    digitalWrite(BUILTIN_LED, HIGH);
    StaticJsonBuffer<200> newBuffer;
    JsonObject& root = newBuffer.parseObject(server.arg("plain"));
    // strip the payload_fields as body for the thingspeak request
    body = root["payload_fields"].as<String>();

    // start a client session to thingspeak with only the body
    HTTPClient http;
    http.begin("https://api.thingspeak.com/update.json", "78:60:18:44:81:35:BF:DF:77:84:D4:0A:22:0D:9B:4E:6C:DC:57:2C");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(body);

    Serial.print("http result:");
    Serial.println(httpCode);
    http.writeToStream(&Serial);

    String payload = http.getString();
    http.end();

    server.send ( 200, "text/json", payload);
    delay(200);
    digitalWrite(BUILTIN_LED, LOW);
  });
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
