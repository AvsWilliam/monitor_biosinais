#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h> 

#define TEMP_SDA 21
#define TEMP_SCL 22
#define HEARTBEAT_PIN 5
#define LOC_PIN 35      

// --- Instâncias Globais ---
Adafruit_AHTX0 aht;

// --- Variáveis de Controle do Timer ---
hw_timer_t *timer = NULL;
volatile bool read_now = false;

// Variáveis para armazenar as últimas leituras
float ultimaLeituraTemp = 0;
float ultimaLeituraHumidity = 0;
float ultimaLeituraLocation = 0;
float ultimaLeituraHeartbeat = 0;

// Função de Interrupção (ISR)
void IRAM_ATTR onTimer() {
    read_now = true; 
}

// Função de leitura centralizada
void atualizarLeituras() {
    // Leitura Digital (I2C) 
    sensors_event_t humidity, temp;
    if (aht.getEvent(&humidity, &temp)) {
        ultimaLeituraTemp = temp.temperature;
        ultimaLeituraHumidity = humidity.relative_humidity;
    } else {
        Serial.println("Falha ao ler o AHT10!");
    }

    // Leituras Analógicas (ADC)
    // ultimaLeituraLocation = analogRead(LOC_PIN);
    // ultimaLeituraHeartbeat = analogRead(HEARTBEAT_PIN);
}

void setup() {
    Serial.begin(115200);
    
    // Inicializa I2C
    Wire.begin(TEMP_SDA, TEMP_SCL);
    
    // Configura Pinos
    pinMode(HEARTBEAT_PIN, INPUT);
    pinMode(LOC_PIN, INPUT);

    // Inicializa o sensor AHT10
    if (!aht.begin()) {
        Serial.println("Erro: AHT10 não encontrado! Verifique SDA(21) e SCL(22).");
        while (1) delay(10);
    }
    Serial.println("AHT10 Inicializado com sucesso.");

    // Configura o Timer (10 segundos)
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 10000000, true);
    timerAlarmEnable(timer);
}

void loop() {
    if(read_now) {
        read_now = false; 
        
        // Executa a leitura completa
        atualizarLeituras();
        
        // Relatório
        Serial.println("\n--- Relatório de Sensores ---");
        Serial.printf("Temp: %.2f °C | Hum: %.2f %%\n", ultimaLeituraTemp, ultimaLeituraHumidity);
        Serial.printf("Localização: %.2f | Heartbeat: %.2f\n", ultimaLeituraLocation, ultimaLeituraHeartbeat);
        Serial.println("-----------------------------");
    }
}