#include <WiFi101.h>
#include <Servo.h>

#define Serv_pin1 3
#define Serv_pin2 2
#define Pin_Switch 6

Servo servo1;
Servo servo2;

const char* ssid = "MKR1000_AP";
WiFiServer server(80);

bool flag1 = 0;
bool flag2 = 0;
int VAR_spruzzo = 0;
unsigned long t1 = 0;
unsigned long t2 = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  servo1.attach(Serv_pin1);
  servo2.attach(Serv_pin2);
  pinMode(Pin_Switch, INPUT);

  WiFi.beginAP(ssid);
  while (WiFi.status() != WL_AP_LISTENING) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\n✅ Access Point attivo!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        if (c == '\n' && request.indexOf("\r\n\r\n") != -1) break;
      }
    }

    Serial.println("📨 Richiesta ricevuta:");
    Serial.println(request);

    if (request.indexOf("GET /RIGHT") != -1) {
      Serial.println("➡️ Spray a destra");
      Spray2();
    }
    else if (request.indexOf("GET /LEFT") != -1) {
      Serial.println("⬅️ Spray a sinistra");
      Spray1();
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println();
    client.println("Comando ricevuto!");
    client.stop();
  }

  // Controlla quando riportare i servo alla posizione base
  if (flag1 && millis() - t1 > 500) {
    servo1.write(0);
    flag1 = 0;
  }

  if (flag2 && millis() - t2 > 500) {
    servo2.write(165);
    flag2 = 0;
  }
}

void Spray1() {           // __________________ Spray1 function
  VAR_spruzzo++;          
  servo1.write(165);      
  t1 = millis();          
  flag1 = 1;              
}                         // __________________ End Spray1 function

void Spray2() {           // __________________ Spray2 function
  VAR_spruzzo++;          
  servo2.write(0);        
  t2 = millis();          
  flag2 = 1;              
}                         // __________________ End Spray2 function
