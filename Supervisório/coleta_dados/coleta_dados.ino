#include <ModbusRTUSlave.h>

// ==== CONFIGURAÇÕES ====
#define TX_LED 2             // Pino que controla DE e RE do MAX232
#define RXD2 16                 // RX2 do ESP32
#define TXD2 17                 // TX2 do ESP32
#define SLAVE_ID 1              // Endereço do escravo Modbus
#define NUM_HOLDING 20          // Quantidade de registradores de retenção

// ==== OBJETOS ====
HardwareSerial RS232Serial(2);
ModbusRTUSlave modbus(RS232Serial, TX_LED);

// Registradores de exemplo (16 bits cada)
uint16_t holdingRegisters[NUM_HOLDING];

// ==== SETUP ====
void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando Escravo Modbus RTU (ESP32 + MAX232)...");

  // Inicializa registradores com valores iniciais
  for (int i = 0; i < NUM_HOLDING; i++) {
    holdingRegisters[i] = i * 10;
  }

  // Inicializa porta serial RS-232
  RS232Serial.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Configura Modbus
  modbus.begin(SLAVE_ID, 9600);
  modbus.configureHoldingRegisters(holdingRegisters, NUM_HOLDING);  // Adiciona 20 registradores holding (endereços 0–19)

  // inicializa com valores
  for (int i = 0; i < NUM_HOLDING; i++) {
    holdingRegisters[i] = i * 10;  // ex: 0, 10, 20, ...
  }

  Serial.println("Modbus RTU iniciado. Aguardando mestre...");
}

// ==== LOOP ====
void loop() {
  // Processa requisições do mestre Modbus
  modbus.poll();

  // exemplo: iterar sobre todos os holding registers
  for (int i = 0; i < NUM_HOLDING; i++) {
    Serial.print("Hreg[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(holdingRegisters[i]);
  }

}