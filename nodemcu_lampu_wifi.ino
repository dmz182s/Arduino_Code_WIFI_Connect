#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "ProjectDPZ";
const char* password = "hahahihi";

int LED = 18;
int LED_ESP = 9;
unsigned int time_now, time_pass;
unsigned int refresh_time = 1000; // 1 detik

int response_code;

String updateURL = "http://192.168.137.1/remote_led/espdata.php";

void setup() {
  delay(100);
  Serial.begin(115200);
  
  pinMode(LED, OUTPUT); 
  pinMode(LED_ESP, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi ...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println(WiFi.localIP());
}

void loop() {
  // lampu indikator
  digitalWrite(LED_ESP, !digitalRead(LED_ESP));

  time_now = millis();

  if(time_now - time_pass > refresh_time) {
    time_pass = time_now;
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      
      http.begin(updateURL);

      response_code = http.GET();

      if(response_code > 0) {
        String bodyContent = http.getString();

        Serial.print(response_code);
        Serial.println(bodyContent);

        if(bodyContent == "on") {
          digitalWrite(LED, HIGH);
        } else if (bodyContent == "off") {
          digitalWrite(LED, LOW);
        }
      } else {
        Serial.print("error code: ");
        Serial.println(response_code);
      }
      
      http.end(); 
    }//END of WIFI connected
    else{
      Serial.println("WIFI connection error");
    }
  }
}
