#include <ModbusRTU.h>

ModbusRTU mb;

// variáveis de temperatura
float temperatura = 0;
uint16_t temperatura_lida = 0;
uint16_t temperatura_atual = 0;
uint16_t temperaturas[2]; // buffer dos holding registers

typedef union {
  float f;
  uint16_t i[2];
} FloatModbus;

FloatModbus result_union;

// controle de tempo
static unsigned long anterior = 0;

// flags
bool attTemperatura = false;
bool msg = false;

void lerFloatModbus(uint8_t slaveID, uint16_t regStart) 
{
  uint16_t temp_regs[2]; // Array para armazenar os dois registradores
  FloatModbus result_union;
}
// callback responsável por comparar a temperatura lida pelo sensor e a temperatura mostrada pro usuário
bool cbCompara(Modbus::ResultCode event, uint16_t, void*) 
{
  if (event == Modbus::EX_SUCCESS) 
  {
    temperatura_lida = temperaturas[0];
    temperatura_atual = temperaturas[1];

    Serial.printf("Temperatura lida pelo sensor: %d\n", temperatura_lida);

    if (temperatura_lida != temperatura_atual)
    {
      attTemperatura = true;
      msg = true;
      Serial.println("Atualizando temperatura...");
    }

    else if (msg)
    {
      Serial.println("O tanque já está na temperatura lida!");
      anterior = millis();
      msg = false;
      
    }
  }

  else 
  {
    Serial.printf("Erro na leitura: %02X\n", event);
  }

  return true;
}

// callback responsável por indicar a atualização da temperatura
bool cbAtualiza(Modbus::ResultCode event, uint16_t, void*) 
{
  if (event == Modbus::EX_SUCCESS) 
  {
    Serial.println("Temperatura atualizada!");
  }
  
  else 
  {
    Serial.printf("Erro na escrita: %02X\n", event);
  }

  return true;
}

// callback responsável pela indicação da leitura recebida pela ESP
bool cbLer(Modbus::ResultCode event, uint16_t, void*)
{
  if (event == Modbus::EX_SUCCESS) 
  {
    result_union.i[0] = temperaturas[0]; // Low-Word (CD) vai para a posição i[0]
    result_union.i[1] = temperaturas[1]; // High-Word (AB) vai para a posição i[1]
    temperatura = result_union.f;
    Serial.printf("Temperatura atual: %f\n", temperatura, 3);
  }
  
  else 
  {
    Serial.printf("Erro na escrita: %02X\n", event);
  }

  return true;
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);  // RX2, TX2
  mb.begin(&Serial2);
  mb.master();
}

void loop() 
{
  mb.task();

  ler(); // Função apenas pra leitura
  // comparar(); // Função para igualar o primeiro e o segundo registrador

  yield();
}

// função responsável por ler o valor no primeiro registrador
void ler()
{
  static unsigned long anterior = 0;
  if (millis() - anterior > 5000 && !mb.slave()) 
  {
    anterior = millis();
    Serial.println("Lendo Hreg[0] do Slave...");
    // mb.readHreg(7, 0, &temperatura, 1, cbLer);
    mb.readHreg(7, 0, temperaturas, 2, cbLer);
  }
}

// função que lê a temperatura e atualiza caso necessário
void comparar()
{
  if (millis() - anterior > 5000 && !mb.slave()) 
  {
    anterior = millis();
    Serial.println("Lendo Hreg[0] do Slave...");
    mb.readHreg(1, 0, temperaturas, 2, cbCompara);
  }

  // se a leitura terminou, faz a escrita agora
  if (attTemperatura && !mb.slave()) 
  {
    attTemperatura = false;
    mb.writeHreg(1, 1, temperatura_lida, cbAtualiza);
  }
}
