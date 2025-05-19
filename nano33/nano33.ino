#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>

const char* ssid = "MKR1000_AP";    
IPAddress serverIP(192, 168, 1, 1);  
IPAddress staticIP(192, 168, 1, 2);  
IPAddress gateway(192, 168, 1, 1);   
IPAddress subnet(255, 255, 255, 0);

WiFiClient client;

void setup() {
    Serial.begin(115200);
    while (!Serial);

    if (!IMU.begin()) {
        Serial.println("❌ Errore: LSM6DS3 non trovato!");
        while (1);
    }
    Serial.println("✅ LSM6DS3 pronto!");

    WiFi.config(staticIP, gateway, subnet);  

    Serial.print("🔌 Connessione a AP...");
    WiFi.begin(ssid);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\n✅ Connesso a AP!");
    Serial.print("📡 IP assegnato: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    float gx, gy, gz;
    if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(gx, gy, gz);

        if (gz > 100) {  // ↪️ Gira a destra (meno sensibile)
            sendCommand("RIGHT");
            delay(300);  // evita invii multipli
        } else if (gz < -100) {  // ↩️ Gira a sinistra (meno sensibile)
            sendCommand("LEFT");
            delay(300);
        }
    }
}


void sendCommand(const char* command) {
    Serial.print("📤 Invio comando: ");
    Serial.println(command);

    if (client.connect(serverIP, 80)) {
        client.print("GET /");
        client.print(command);
        client.println(" HTTP/1.1");
        client.println("Host: MKR1000");
        client.println("Connection: close");
        client.println();
        client.stop();
    } else {
        Serial.println("❌ Errore connessione!");
    }
}
