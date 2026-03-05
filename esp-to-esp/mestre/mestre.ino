/**
 * @file Arduino_RS485_2.ino
 * Criado por: Saulo Aislan
 * @brief Firmware para receber e enviar dados via RS485 (Arduino Mega).
 * @version 1.0
 * 
 * @copyright Copyright (c) 2022
 * 
*/

#include <SoftwareSerial.h>
#include <string.h>

#define LED_PIN    2     // Pino do LED BuiltIn

const unsigned long BAUD = 9600;
unsigned long currentMillis;

const unsigned long tempoPraComando = 5000;
bool temComando = false;
unsigned long prevResponseMillis = 0;
const char question[] = "Por favor, insira o comando LED ou o comando TEMPERATURA";

String comando = "";

int i = 0;

// Nao há necessidade de definir se a ESP está transmitindo ou recebendo pois
// a comunicação RS232 é full-duplex

/**
 * @brief setup
 */
void setup ()
{
  delay(500);
  Serial.flush();

  Serial.begin (12*BAUD);
  Serial2.begin(BAUD, SERIAL_8N1, 16, 17);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, 0);  

  Serial.println("\nComunicação Iniciada!");
}


/**
 * @brief loop
 */
void loop ()
{
  currentMillis = millis();

  pedirComando();
  receberComando();
  
  transmitir();
  receber();
}

void pedirComando()
{
  if (temComando == true) {
    return;
  }
  
  if (currentMillis - prevResponseMillis >= tempoPraComando) {
    Serial.println(question);
    temComando = true;
    comando = "";
  }
}

void receberComando()
{
  if (temComando == false) {
    return;
  }

  if(Serial.available() == 0) {
    return;
  }

  comando = Serial.readStringUntil('\n'); // lê até pressionar ENTER
  comando.trim(); // remove espaços e quebras de linha
   
  if (comando != "LED" && comando != "TEMPERATURA") {
    Serial.println ("Comando inválido");
    temComando = false;
    comando = "";
    prevResponseMillis = currentMillis;
  }
}
  
void transmitir () {
  // Iniciando transmissão

  if (temComando && comando != "")
  {
    Serial.print("Transmitindo comando ");
    Serial.println(comando);
    Serial2.print(comando);         // Valor do potenciomento
    Serial2.flush();
    delay(tempoPraComando);
    temComando = false;
    comando = "";
    prevResponseMillis = currentMillis;
  }

}  
  
void receber (){
  int temperatura = Serial2.parseInt();
  // Iniciando recebimento
  if(Serial2.available() > 0 && temperatura != 0)
  {
       // Transforma em inteiro o valor lido
      Serial.print("\nTemperatura atual em °C é:");
      Serial.println(temperatura);
  }
}