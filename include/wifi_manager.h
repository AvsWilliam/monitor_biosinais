#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

// Função para inicializar o Wi-Fi
void wifi_init(const char* ssid, const char* password);

// Função para conectar ao Wi-Fi
void wifi_connect(const char* ssid, const char* password);

// Função para imprimir o status da conexão Wi-Fi
void wifi_print_status();
#endif


