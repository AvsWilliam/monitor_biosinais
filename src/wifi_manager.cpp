// #include "wifi_manager.h"

// void wifi_init(const char* ssid, const char* password) {
//     WiFi.begin(ssid, password);
//     Serial.println("Conectando ao Wi-Fi...");
// }

// void wifi_connect(const char* ssid, const char* password) {
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//         Serial.print(".");
//         int watchdogCounter = 0; // Contador para evitar loop infinito
//         while (watchdogCounter < 20) { // Limita o tempo de espera
//             delay(500);
//             Serial.print(".");
//             watchdogCounter++;
//         }
//         if (watchdogCounter >= 20) {
//             Serial.println("\nFalha ao conectar ao Wi-Fi. Verifique as credenciais e o sinal.");
//             return;
//         }
//     }
//     Serial.println("\nWi-Fi conectado!");
//     Serial.print("Endereço IP: ");
//     Serial.println(WiFi.localIP());
// }

// void  wifi_print_status() {
//     wl_status_t status = WiFi.status();
//     switch (status) {
//         case WL_CONNECTED:
//             Serial.println("Status: Conectado");
//             break;
//         case WL_NO_SSID_AVAIL:
//             Serial.println("Status: SSID não disponível");
//             break;
//         case WL_CONNECT_FAILED:
//             Serial.println("Status: Falha na conexão");
//             break;
//         case WL_DISCONNECTED:
//             Serial.println("Status: Desconectado");
//             break;
//         default:
//             Serial.println("Status: Desconecido");
//     }
// }