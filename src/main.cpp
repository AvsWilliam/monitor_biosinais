#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h> // Biblioteca para o AHT10
#include "MAX30105.h"
#include "heartRate.h"

// --- Objetos dos Sensores ---
MAX30105 particleSensor;
Adafruit_AHTX0 aht;

// --- Variáveis do MAX30102 (Batimentos) ---
const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute;
int beatAvg;

// --- Variáveis de Controle e Timer ---
hw_timer_t *timer = NULL;
volatile bool flagLeituraAHT = false;

// Função de Interrupção (ISR)
void IRAM_ATTR onTimer() {
    flagLeituraAHT = true; 
}

void setup() {
    Serial.begin(115200);
    
    // Inicia o barramento I2C nos pinos padrão do ESP32
    // SDA: 21, SCL: 22
    Wire.begin(21, 22);

    Serial.println("--- Inicializando Sensores I2C ---");

    // 1. Inicializa AHT10
    if (!aht.begin()) {
        Serial.println("Erro: AHT10 não encontrado. Verifique a fiação.");
        while (1);
    }
    Serial.println("AHT10 detectado!");

    // 2. Inicializa MAX30102
    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
        Serial.println("Erro: MAX30102 não encontrado.");
        while (1);
    }
    Serial.println("MAX30102 detectado!");
    
    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x0A);
    particleSensor.setPulseAmplitudeIR(0x0A);

    // 3. Configuração do Timer (Interrupção a cada 10 segundos)
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 10000000, true); 
    timerAlarmEnable(timer);

    Serial.println("Sistema iniciado com sucesso.\n");
}

void loop() {
    // --- PRIORIDADE 1: Monitoramento de Batimentos (Constante) ---
    long irValue = particleSensor.getIR();

    if (checkForBeat(irValue) == true) {
        long delta = millis() - lastBeat;
        lastBeat = millis();
        beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20) {
            rates[rateSpot++] = (byte)beatsPerMinute;
            rateSpot %= RATE_SIZE;
            
            beatAvg = 0;
            for (byte x = 0; x < RATE_SIZE; x++) beatAvg += rates[x];
            beatAvg /= RATE_SIZE;
        }
    }

    // --- PRIORIDADE 2: Relatório Periódico (A cada 10s via Timer) ---
    if (flagLeituraAHT) {
        sensors_event_t humidity, temp;
        aht.getEvent(&humidity, &temp); // Leitura do AHT10 via I2C

        Serial.println("============== RELATÓRIO SENSORES ==============");
        Serial.printf("Temperatura: %.2f °C\n", temp.temperature);
        Serial.printf("Umidade:     %.2f %%\n", humidity.relative_humidity);
        Serial.printf("BPM Atual:   %.2f\n", beatsPerMinute);
        Serial.printf("Média BPM:   %d\n", beatAvg);
        
        if (irValue < 50000) {
            Serial.println("Status: Nenhum dedo detectado no sensor cardíaco.");
        }
        Serial.println("==============================================\n");

        flagLeituraAHT = false; // Reseta a flag para o próximo ciclo
    }
}