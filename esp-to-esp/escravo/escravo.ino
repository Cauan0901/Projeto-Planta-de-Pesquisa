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
const char question[] = "Aguardando comando";

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
  
  Serial.begin(12*BAUD);
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
  
  piscar ();
  transmitir();
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

  if(Serial2.available() == 0) {
    return;
  }

  comando = Serial2.readStringUntil('\n'); // lê até pressionar ENTER
  comando.trim(); // remove espaços e quebras de linha

  Serial.println(comando);

  if (comando != "LED" && comando != "TEMPERATURA") 
  {
    Serial.println ("Comando inválido");
    temComando = false;
    comando = "";
    prevResponseMillis = currentMillis;
  }
}
  
// void transmitir () {
//   // Iniciando transmissão

//   if (temComando)
//   {
//     Serial2.print('I');                // Inicio do pacote de transmissão
//     Serial2.print(comando);         // Valor do potenciomento
//     Serial2.print('F');                // Fim do pacote de transmissão
//     Serial2.flush();  
//     delay(tempoPraComando);
//   }

// }  

void transmitir () 
{
  if (temComando && comando == "TEMPERATURA")
  {
  // Iniciando transmissão
    Serial.println ("Transmitindo temperatura");
    int temperatura = 26;
    Serial2.print('I');                // Inicio do pacote de transmissão
    Serial2.print(temperatura);         // Valor do potenciomento
    Serial2.print('F');                // Fim do pacote de transmissão
    Serial2.flush();  
    delay(tempoPraComando);
    temComando = false;
    comando = "";
    prevResponseMillis = currentMillis;
  }
}

void piscar ()
{
  if (temComando && comando == "LED")
  {
    Serial.println ("Piscando LED");
    i=0;
    while(i<5)
    {
      digitalWrite(LED_PIN, HIGH);
      delay(1000);
      digitalWrite(LED_PIN, LOW);
      delay(1000);
      i++;
    }
    temComando = false;
    comando = "";
    prevResponseMillis = currentMillis;
  }
}