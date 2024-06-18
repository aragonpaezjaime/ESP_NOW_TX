#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
// Variables 
bool Led1,Led2,Led3,Led4=false;
float temperatura;
int humedad;

//* Pines 
#define botonRojo     13
#define botonAmarillo 27
#define botonAzul     25
#define botonNegro    23
#define ledRojo       15
#define ledAmarillo   4
#define ledAzul       5
#define ledBlanco     18 

//direcciones
uint8_t direccionMac1[] = {0xC8,0x2E,0x18,0x22,0x99,0x20};
uint8_t direccionMac2[] = {0xC8,0x2E,0x18,0x24,0x89,0x10};
uint8_t direccionMac3[] = {0xC8,0x2E,0x18,0x22,0x96,0x4C};
uint8_t direccionMac4[] = {0xC8,0x2E,0x18,0x26,0x66,0x5C};
uint8_t difucion[] = {0xff,0xff,0xff,0xff,0xff,0xff};

typedef struct estructuraDelMensaje {
  bool Led1;
  bool Led2;
  bool Led3;
  bool Led4; //no me dieron mucha info
  // pero domotica no les desagrado mucho
  float temperatura;
  int humedad;
} estructuraDelMensaje;
estructuraDelMensaje datosDeSalida;
estructuraDelMensaje datosDeEntrada;
// Crea una variable conla estructura esp_now_peer_info_t
// llamada peerInfo
esp_now_peer_info_t peerInfo;

// Esta función es llamada cuando se envia un dato
void enviarDatos(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nEstado del ultimo paquete de datos enviado:\t");
  if(status == ESP_NOW_SEND_SUCCESS) 
  {
    Serial1.println("Envio Exitoso"); 
  }else
  {
     Serial1.println("Fallo el envio");
  }
}
void llegoDato(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
  memcpy(&datosDeEntrada, incomingData, sizeof(datosDeEntrada));
  digitalWrite(2,HIGH);
  delay(500);
  digitalWrite(2,LOW);
}
void enviarA(int);
void setup() 
{
  pinMode(botonRojo,INPUT_PULLDOWN);
  pinMode(botonAmarillo,INPUT_PULLDOWN);
  pinMode(botonAzul,INPUT_PULLDOWN);
  pinMode(botonNegro,INPUT_PULLDOWN);
  pinMode(ledRojo,OUTPUT);
  pinMode(ledAmarillo,OUTPUT);
  pinMode(ledAzul,OUTPUT);
  pinMode(ledBlanco,OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);   // se selecciona modo estación
  // Inicializa ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error al inicializar ESP-NOW");
    return;
  }
  // Funcion que se ejecuta al enviar datos
  esp_now_register_send_cb(enviarDatos);
  // Funcion que se ejecuta al recibir datos
  esp_now_register_recv_cb(llegoDato);
  //Configurando canales y encriptacion 
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  //anadiendo los peers
  memcpy(peerInfo.peer_addr, direccionMac1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer 1");
    return;
  }
  memcpy(peerInfo.peer_addr, direccionMac2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer 2");
    return;
  }
  memcpy(peerInfo.peer_addr, direccionMac3, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer 3");
    return;
  }
  // memcpy(peerInfo.peer_addr, direccionMac4, 6);
  // if (esp_now_add_peer(&peerInfo) != ESP_OK){
  //   Serial.println("Failed to add peer 4");
  //   return;
  // }
  
}
void loop() {
  if (botonRojo)
  {
    Led1= !Led1;
    enviarA(4);
  }
  if (botonAmarillo)
  {
    Led2= !Led2;
    enviarA(4);
  }
  if (botonAzul)
  {
    Led3= !Led3;
    enviarA(4);
  }
  if (botonNegro)
  {
    Led4= !Led4;
    enviarA(5);
  }

}
void enviarA(int numero)
{
  // envia el mensaje  via ESP-NOW
  if (numero==1)
  {
    esp_err_t result = esp_now_send(direccionMac1, (uint8_t *) &datosDeSalida, sizeof(datosDeSalida));
    if (result == ESP_OK) 
    {
      Serial.println("Envio Confirmado a 1");
    }
    else 
    {
      Serial.println("Error de envio de datos a 1");
    }
  }
  if (numero==2)
  {
    esp_err_t result = esp_now_send(direccionMac2, (uint8_t *) &datosDeSalida, sizeof(datosDeSalida));
    if (result == ESP_OK) 
    {
      Serial.println("Envio Confirmado a 2");
    }
    else 
    {
      Serial.println("Error de envio de datos a 2");
    }
  }
  if (numero==3)
  {
    esp_err_t result = esp_now_send(direccionMac3, (uint8_t *) &datosDeSalida, sizeof(datosDeSalida));
    if (result == ESP_OK) 
    {
      Serial.println("Envio Confirmado a 3");
    }
    else 
    {
      Serial.println("Error de envio de datos a 3");
    }
  }
  if (numero==4)
  {
    esp_err_t result = esp_now_send(direccionMac3, (uint8_t *) &datosDeSalida, sizeof(datosDeSalida));
    if (result == ESP_OK) 
    {
      Serial.println("Envio Confirmado a 4");
    }
    else 
    {
      Serial.println("Error de envio de datos a 4");
    }
  }
}