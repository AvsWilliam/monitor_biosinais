#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "database_manager.h"
#include "models.h"
#include "wifi_manager.h"
#include "bluetooth_manager.h"

// --- Objetos e Variáveis Globais ---
MAX30105 particleSensor;
Adafruit_AHTX0 aht;

const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute = 0;
int beatAvg = 0;

volatile bool flagLeituraAHT = false;
hw_timer_t *timer = NULL;

// Função de Interrupção do Timer
void IRAM_ATTR onTimer() {
    flagLeituraAHT = true; 
}

void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22);

    Serial.println("--- Inicializando Sistema ---");

    // 1. Inicializa Sensores
    if (!aht.begin()) {
        Serial.println("Erro: AHT10 não encontrado!");
        while (0);
    }

    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
        Serial.println("Erro: MAX30102 não encontrado!");
        while (0);
    }
    
    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x0A);
    particleSensor.setPulseAmplitudeIR(0x0A);

    // 2. Inicializa Banco de Dados (Code First)
    if (db_init()) {
        Serial.println("Banco de Dados SQLite OK!");
   
    } else {
        Serial.println("Erro ao inicializar SQLite (verifique LittleFS).");
    }
    // 3. Inicializa Wi-Fi
    // wifi_init("SEU_SSID", "SUA_SENHA");
    // wifi_connect("SEU_SSID", "SUA_SENHA");
    // wifi_print_status();
    
    // 4. Inicializa Bluetooth
    bluetooth_init();

    // 5. Configuração do Timer (10 segundos)
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 10000000, true); 
    timerAlarmEnable(timer);
}

void loop() {
    // --- PRIORIDADE 1: Monitoramento de Batimentos (Real-time) ---
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

    // --- PRIORIDADE 2: Persistência no Banco (A cada 10s via Timer) ---
    if (flagLeituraAHT) {
        sensors_event_t humidity, temp;
        aht.getEvent(&humidity, &temp);

        HealthData registro = {
            .temperature = temp.temperature,
            .humidity = humidity.relative_humidity,
            .bpm = (int)beatsPerMinute,
            .avgBpm = beatAvg,
            .timestamp = millis() // Marcar o tempo da leitura
        };

        // Salva no SQLite e mostra no Serial
        if (db_save_health_data(registro)) {
            Serial.println("\n>> DADOS SALVOS NO SQLITE <<");
            Serial.printf("Temp: %.2fC | BPM: %d\n", registro.temperature, registro.bpm);
            db_debug_list_data(); // Mostra os últimos registros para verificação 
            monitorar_memorias(); // Verifica uso do armazenamento
        } else {
            Serial.println("Erro ao salvar no banco!");
        }

        
        
        if (irValue < 50000) {
            Serial.println("Aviso: Sensor cardíaco sem leitura estável.");
        }

        flagLeituraAHT = false; 
    }
}