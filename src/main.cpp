#include <Arduino.h>

#define TEMP_PIN 20
#define HUMIDITY_PIN 21
#define LOC_PIN 22

// --- Configurações do Timer ---
hw_timer_t *timer = NULL;
volatile float ultimaLeituraTemp = 0;
volatile float ultimaLeituraHumidity = 0;
volatile float ultimaLeituraLocation = 0;
volatile bool novoDadoDisponivel = false;

// Função de Interrupção (ISR) - Executada pelo Timer
void IRAM_ATTR onTimer() {
    // Leitura rápida dos sensores a cada 10 segundos
    ultimaLeituraTemp = analogRead(TEMP_PIN);
    ultimaLeituraHumidity = analogRead(HUMIDITY_PIN);
    ultimaLeituraLocation = analogRead(LOC_PIN);
    novoDadoDisponivel = true;
}

// Função auxiliar de leitura individual
float read_sensors(String type) {
    if (type == "temp") return (float)analogRead(TEMP_PIN);
    if (type == "humidity") return (float)analogRead(HUMIDITY_PIN);
    if (type == "location") return (float)analogRead(LOC_PIN);
    
    Serial.println("Tipo de sensor desconhecido!");
    return 0.0;
}

// --- Funções de Média (Windowing) ---
// Agora elas realmente usam a "window" para tirar uma média e estabilizar o valor

float get_temp_avg(int window) {
    float sum = 0;
    for(int i = 0; i < window; i++){
        sum += read_sensors("temp");
        delay(2); // Pequena pausa para o ADC estabilizar entre leituras
    }
    return sum / window;
}

float get_humidity_avg(int window) {
    float sum = 0;
    for(int i = 0; i < window; i++){
        sum += read_sensors("humidity");
        delay(2);
    }
    return sum / window;
}

void setup() {
    Serial.begin(115200);
    
    pinMode(TEMP_PIN, INPUT);
    pinMode(HUMIDITY_PIN, INPUT);
    pinMode(LOC_PIN, INPUT);

    // Configuração do Timer
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    
    // 10.000.000 us = 10 segundos
    timerAlarmWrite(timer, 10000000, true);
    timerAlarmEnable(timer);
    
    Serial.println("Sistema iniciado. Aguardando primeira leitura do Timer (10s)...");
}

void loop() {
    if(novoDadoDisponivel) {
        // Proteção para ler as variáveis voláteis
        noInterrupts(); 
        float t = ultimaLeituraTemp;
        float h = ultimaLeituraHumidity;
        float l = ultimaLeituraLocation;
        novoDadoDisponivel = false; 
        interrupts(); 

        Serial.println("============== RELATÓRIO TIMER (10s) ==============");
        Serial.printf("Temperatura ADC: %.2f\n", t);
        Serial.printf("Humidade ADC:    %.2f\n", h);
        Serial.printf("Localizacao ADC: %.2f\n", l);
        Serial.println("===================================================");
    }
    
    
}