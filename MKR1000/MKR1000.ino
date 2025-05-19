#include <WiFi101.h>

const char* ssid = "MKR1000_AP";   
WiFiServer server(80);

void setup() {
    Serial.begin(115200);
    while (!Serial);

    Serial.print("🛠 Creazione AP...");
    WiFi.beginAP(ssid);

    while (WiFi.status() != WL_AP_LISTENING) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\n✅ AP creato! IP:");
    Serial.println(WiFi.localIP());
    server.begin();
}

void loop() {
    WiFiClient client = server.available();
    if (client) {
        Serial.println("📩 Client connesso!");
        String request = "";

        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                request += c;
                if (c == '\n' && request.indexOf("\r\n\r\n") != -1) break;
            }
        }

        Serial.println("📨 Ricevuto: " + request);

        if (request.indexOf("GET /RIGHT") != -1) {
            Serial.println("🔄 Testa ruotata a DESTRA!");
        } 
        else if (request.indexOf("GET /LEFT") != -1) {
            Serial.println("🔄 Testa ruotata a SINISTRA!");
        }

        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/plain");
        client.println();
        client.println("OK");
        client.stop();
    }
}
