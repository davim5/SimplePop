#include <PubSubClient.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// ************************ WIFI SETUP ************************

const char* ssid = "Aecio";
const char* password =  "01030704";
const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

WiFiClient espClient;
PubSubClient client(espClient);

Servo myservo;  // create servo object to control a servo

bool START = false;

unsigned long TI;
unsigned long TD;
int contador;

// ************************ DEFINIÇÃO DOS PINOS ************************

int pos = 0;    // Posição do Servo
int pinoSensor = 12;

char str[10];



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

//************************************ LOOP ************************************

void loop() {
  reconectabroker();


  //GIRAR HELICES
  //  if (START == true)
  //  {
  //    do {
  //      Serial.println("Startou");
  //
  //      //  PEGAR TEMPO E ZERAR CONTADOR
  //      //    OUVIR PIPOCADA

  //      ACRESCENTAR CONTADOR

  //      CHECAR TEMPO

  //   CHECAR CONTADOR

  //PARAR HELICES
  //    } while (START == true);

  do {

    contador = 0;
    TI = millis();
    //    OUVIR PIPOCADA
    Serial.print("TI: ");
    Serial.println(TI);


    do {

      sprintf(str, "%d", contador);
      client.publish("/pipoca/mic", str);

      for (pos = 0; pos <= 180; pos += 1) {
        Serial.println(pos);
        myservo.write(pos);
        delay(20);
        if (digitalRead(pinoSensor) == HIGH) {
          contador++;
          TD = millis();
        }
        else {
          TD = millis();
        }
      }
      sprintf(str, "%d", contador);
      client.publish("/pipoca/mic", str);

      for (pos = 180; pos >= 0; pos -= 1) {
        Serial.println(pos);
        myservo.write(pos);
        delay(20);
        if (digitalRead(pinoSensor) == HIGH) {
          contador++;
          TD = millis();
        }
        else {
          TD = millis();
        }
      }

      sprintf(str, "%d", contador);
      client.publish("/pipoca/mic", str);

      Serial.print("TD: ");
      Serial.println(TD);

      Serial.print("Contador chegou em: ");
      Serial.println(contador);

    } while ((TD - TI) < 10000);

    if (contador > 10) {
      Serial.println("Hora da pipoca! :)");
    }
    else {
      Serial.println("Pipoca ainda nao esta pronta! :(");
    }


  } while (contador < 10);




}


// ************************************************************  CONECXÃO COM O BROKER ************************************************************
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
