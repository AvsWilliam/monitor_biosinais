#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H
#include "models.h"
#include <Arduino.h>

// Biblioteca NimBLE para BLE no ESP32
void bluetooth_init();

// Função para enviar dados de saúde via BLE
void bluetooth_health_data(const HealthData &data);

#endif // BLUETOOTH_MANAGER_H