#include <PubSubClient.h>
#include <WiFi.h>
#include <ESP32Servo.h>


const char* ssid = "UNIFOR_WIFI";
const char* password =  "";
const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

WiFiClient espClient;
PubSubClient client(espClient);

Servo myservo;  // create servo object to control a servo
//

int pos = 0;    // Posição do Servo
int pinoSensor = 12;

void setup() {
  myservo.attach(13);  // Servo ligado no pin 13
  pinMode(pinoSensor, INPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Iniciando conexao com a rede WiFi..");
  }
  //  //
  Serial.println("Conectado na rede WiFi!");
}

void loop() {
  reconectabroker();

  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
//    Serial.println(pos);
    myservo.write(pos);             // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
    if (digitalRead(pinoSensor) == HIGH) { //SE A LEITURA DO PINO FOR IGUAL A HIGH, FAZ
      client.publish("/pipoca/mic", "1");
      Serial.println("OUVIU");
    }
    else{
      client.publish("/pipoca/mic", "0");
      Serial.println("OUVIU NÃO");
    }
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//    Serial.println(pos);
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position

    if (digitalRead(pinoSensor) == HIGH) { //SE A LEITURA DO PINO FOR IGUAL A HIGH, FAZ
      client.publish("/pipoca/mic", "1");
      Serial.println("Ouviu");
    }
    else{
      client.publish("/pipoca/mic", "0");
      Serial.println("OUVIU NÃO");
    }
  }
}

void reconectabroker() {
  //Conexao ao broker MQTT
  client.setServer(mqttServer, mqttPort);
  while (!client.connected())
  {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP32Client"))
    {
      Serial.println("Conectado ao broker!");
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
