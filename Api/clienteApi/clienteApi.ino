
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


#define SERVER_IP "192.168.0.104:8080"

#define STASSID "Dsd"
#define STAPSK  "12345Dsd."

void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    
    Serial.print("[HTTP] begin...\n");

    http.begin(client, "http://" SERVER_IP "/guardarData"); //HTTP
    http.addHeader("Content-Type", "application/json");

    // start connection and send HTTP header and body
    int httpCode = http.POST("{\"id_dispositivo\":\"1\",\"nombre_dispositivo\":\"nodemcu\",\"valor_1\":\"apagado\",\"valor_2\":\"encendido\"}");

    if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_CREATED) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  
  delay(10000);
}
