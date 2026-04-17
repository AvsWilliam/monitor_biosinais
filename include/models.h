#ifndef MODELS_H
#define MODELS_H

#include <Arduino.h>

// Entidade que representa uma amostra de dados do sensor
struct HealthData {
    float temperature;
    float humidity;
    int bpm;
    int avgBpm;
    long timestamp; // Para registrar quando a leitura ocorreu
};

#endif