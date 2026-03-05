#include <ModbusRTU.h>
#include <String.h>

ModbusRTU mb;

#define RXD2 16
#define TXD2 17

unsigned long currentMillis;

const unsigned long tempoPraComando = 5000;
bool temComando = false;
unsigned long prevResponseMillis = 0;
const char question[] = "Qual a temperatura lida pelo sensor do tanque?";

int temperatura = 0;
String comando = "";

int i = 0;

// Callback chamado quando o master escreve em um registrador
uint16_t cbWrite(TRegister* reg, uint16_t val) {
  reg->value = val;
  Serial.printf("A temperatura atual do tanque é: %d\n", val);
  return val;
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  mb.begin(&Serial2);
  mb.slave(1); // ID do slave

  // Cria dois registradores Holding (endereços 0 e 1)
  mb.addHreg(0, 10);  // valor inicial 10 (para teste)
  mb.addHreg(1, 0);

  // Callback para quando houver escrita
  mb.onSetHreg(0, cbWrite);
  mb.onSetHreg(1, cbWrite);

  Serial.println("Slave Modbus iniciado (ID 1)");
}

void loop() {
  mb.task();

  currentMillis = millis();

  pedirComando();
  receberComando();

  attTemperatura();
}

void attTemperatura()
{
  if (temComando && temperatura > 0)
  {
    mb.Hreg(0, temperatura);
    Serial.println("Nova temperatura lida");
    delay(tempoPraComando);
    temComando = false;
    temperatura = 0;
    comando = "";
    prevResponseMillis = currentMillis;
  }
}

void pedirComando()
{
  if (temComando == true) {
    return;
  }
  
  if (currentMillis - prevResponseMillis >= tempoPraComando) {
    Serial.println(question);
    temComando = true;
    temperatura = 0;
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
  temperatura = comando.toInt();
  Serial.println(temperatura);

  if (round(temperatura) <= 0) {
    Serial.println ("Certifique-se de indicar um valor inteiro acima de 0!");
    temComando = false;
    temperatura = 0;
    comando = "";
    prevResponseMillis = currentMillis;
  }
}