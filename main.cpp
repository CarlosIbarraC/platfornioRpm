#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "CELMIRA78";
const char* password =  "celmira 19%19";

const char *mqtt_server = "suwebya.ml";
const int mqtt_port = 1883 ;
const char *mqtt_user = "web_client";
const char *mqtt_pass = "121212";

/* const char *root_topic_subscribe = "Temperatura/prueba";
const char *root_topic_publish = "Temperatura/prueba";
const char *salida = "Temperatura/salida";
const char *entrada = "Temperatura/entrada";
 */
WiFiClient SuwebyaESP;
PubSubClient client(SuwebyaESP);
long lastMsg=0; 
char msg[50];
char msgr[25];
int temp1 = 0;
int temp2 = 1;
int volts = 2;
int LED = 18;
int LED2 = 19;
volatile int contador = 0;
int Sensor1 = 23;
long num=0;
int minuto=0;

//*****************************
//***    CONTADOR RPM      ***
//*****************************

void contar(){  
  if(digitalRead(Sensor1)==0){
    contador++;
    Serial.println(contador);
         
      
   }  
}  
//******************************************
//***D E C L A R A R -- F U N C I O N E S ***
//******************************************

void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void setup() {
pinMode(LED,OUTPUT);
pinMode(LED2,OUTPUT);
attachInterrupt(digitalPinToInterrupt(Sensor1),contar,FALLING);
Serial.begin(115200);
randomSeed(micros());
setup_wifi();
client.setServer(mqtt_server, mqtt_port);
client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
   
    
  long now = millis();
  if(now - lastMsg>1000){
    lastMsg = now;
    temp1=rand()%51;
    temp2=1+rand()%(101-1);
    volts=100+rand()%(150-100);
    
    String to_send = String(temp1)+","+ String(temp2)+","+String(volts);
    to_send.toCharArray(msg,25);
    Serial.println("Plubish message:");
    Serial.println(msg);
    client.publish("values", msg);
    int rpm=contador;
String str = "LAS RPM SON ->" + String(rpm);
str.toCharArray(msgr,25);

Serial.print("rpm-> ");
Serial.println(str);

  minuto++;
  if(minuto==60){
    client.publish("revoluciones",msgr);
    contador=0;
    minuto=0;   
  }
  Serial.println(minuto);
  }
}

//************************
//** F U N C I O N E S ***
//************************

//*****************************
//***    CONEXION WIFI      ***
//*****************************
void setup_wifi(){
  delay(10);
  // Nos conectamos a nuestra red Wifi
  Serial.println();
  Serial.print("Conectando a ssid: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado a red WiFi!");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

//*****************************
//***       CALLBACK        ***
//*****************************

void callback(char* topic, byte* payload, unsigned int length){
  String strTopic = String((char*)topic); 
  String incoming = "";
  Serial.print("Mensaje recibido desde -> ");
  Serial.print(topic);  
  Serial.println("");
  
  for (int i = 0; i < length; i++) {
    incoming += (char)payload[i];
  }
  incoming.trim();
  Serial.println("Mensaje -> " + incoming);
  if( strTopic == "led1"){
  if(incoming =="on"){
    digitalWrite(LED,HIGH);
    Serial.println("\n Led Encendido");
  }else if(incoming=="off"){
    digitalWrite(LED,LOW);
    Serial.println("\n Led Apagado");
  }
  }
   if(strTopic=="led2"){
  if(incoming=="on"){
     digitalWrite(LED2,HIGH);
     Serial.println("\n Led Encendido");
  }else if(incoming=="off"){
    digitalWrite(LED2,LOW);
    Serial.println("\n Led Apagado");
  }
   }
    if (strTopic == "values"){
     Serial.println("recivido desdes values->"+incoming);
     
     
   }
}

//*****************************
//***    CONEXION MQTT      *****///////
//*****************************

void reconnect() {

  while (!client.connected()) {
    Serial.print("Intentando conexión Mqtt...");
    // Creamos un cliente ID
    String clientId = "ESP32_";
    clientId += String(random(0xffff), HEX);
    // Intentamos conectar
    if (client.connect(clientId.c_str(),mqtt_user,mqtt_pass)) {
      Serial.println("Conectado!");
      client.subscribe("led1");
      client.subscribe("led2");
      // Nos suscribimos      
      }else{
        Serial.println("fallo Suscripciión: con error->");
         Serial.print(client.state());
         Serial.println(" Intentamos de nuevo en 5 segundos");
         delay(5000);
      }      
    }
  }









