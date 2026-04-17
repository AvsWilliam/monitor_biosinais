#include <NimBLEDevice.h>
#include "database_manager.h"
#include "models.h"
#include "bluetooth_manager.h"
#include <NimBLEDevice.h>


class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
        std::string value = pCharacteristic->getValue();

        if (value == "SYNC") {
            Serial.println("Iniciando exportação do banco para o App...");
            db_export_to_bluetooth();
            Serial.println("Exportação concluída.");
        }
    }
};
//========================================================================================

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;

// UUIDs únicos (Você pode gerar os seus em uuidgenerator.net)
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"




class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) { deviceConnected = true; };
    void onDisconnect(BLEServer* pServer) { 
        deviceConnected = false;
        BLEDevice::startAdvertising(); // Reinicia visibilidade
    }
};

void bluetooth_init() {
BLEDevice::init("ESP32_Wearable");
    BLEServer* pServer = BLEDevice::createServer();
    
    BLEService* pService = pServer->createService(SERVICE_UUID);
    
    // Criar a característica com permissão de ESCRITA (WRITE)
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        NIMBLE_PROPERTY::READ | 
        NIMBLE_PROPERTY::WRITE | 
        NIMBLE_PROPERTY::NOTIFY
        );

    // VINCULA O CALLBACK AQUI
    pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

    pService->start();
    BLEDevice::startAdvertising();
}

void bluetooth_data_require (){
    
}

void bluetooth_health_data(const HealthData &data) {
    if (deviceConnected) {
        char payload[64];
        snprintf(payload, sizeof(payload), "T:%.1f|H:%.1f|B:%d", 
                 data.temperature, data.humidity, data.bpm);
        
        pCharacteristic->setValue(payload);
        pCharacteristic->notify(); // "Empurra" o dado para o celular automaticamente
        Serial.println("Notificação BLE enviada!");
    }
}