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
    // Lemos apenas o sensor crítico (ex: temperatura/biosinal) na interrupção
    ultimaLeituraTemp = analogRead(TEMP_PIN);
    ultimaLeituraHumidity = analogRead(HUMIDITY_PIN);
    ultimaLeituraLocation = analogRead(LOC_PIN);
    novoDadoDisponivel = true;
}

//Caso haja necessidade de ler os outros sensores fora da ISR.

float read_sensors(String type) {
    float value_get = 0.0;

    if (type == "temp") {
        value_get = analogRead(TEMP_PIN);
    } 
    else if (type == "humidity") {
        value_get = analogRead(HUMIDITY_PIN);
    }
    else if (type == "location") {
        value_get = analogRead(LOC_PIN);
    }
    else {
        Serial.println("Tipo de sensor desconhecido!");
    }
    return float(value_get);
}
float temp(int window) {
    for(int i = 0; i < window; i++){
      float temp = read_sensors("temp");
      Serial.printf("Leitura ADC: %f\n", temp);
    }
}

float humidity(int window) {
    for(int i = 0; i < window; i++){
      float humidity = read_sensors("humidity");
      Serial.printf("Leitura ADC: %f\n", humidity);
    }
}

float location(int window) {
    for(int i = 0; i < window; i++){
      float location = read_sensors("location");
      Serial.printf("Leitura ADC: %f\n", location);
    }
  } 


void setup() {
    Serial.begin(115200); // Aumentei para 115200 (padrão ESP32/Pycom)
    
    pinMode(TEMP_PIN, INPUT);
    pinMode(HUMIDITY_PIN, INPUT);
    pinMode(LOC_PIN, INPUT);

    // --- Configuração do Timer (Frequência de 100Hz = a cada 10ms) ---
    // 80 é o divisor para transformar o clock de 80MHz em 1MHz (1 tique = 1us)
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    
    // 10000000 microssegundos = 10 segundos
    timerAlarmWrite(timer, 10000000, true);
    timerAlarmEnable(timer);
}

void loop() {
    if(novoDadoDisponivel) {
        noInterrupts(); // Desabilitamos interrupções para ler os dados de forma consistente
        float tempValue = ultimaLeituraTemp;
        float humidityValue = ultimaLeituraHumidity;
        float locationValue = ultimaLeituraLocation;
        interrupts(); // Reabilitamos interrupções
        novoDadoDisponivel = false; // Resetamos a flag
        Serial.printf("Temperatura: %f | Humidade: %f | Localização: %f\n", tempValue, humidityValue, locationValue);
    }
}
