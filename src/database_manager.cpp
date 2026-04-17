#include "database_manager.h"
#include "bluetooth_manager.h"
#include <FS.h>
#include <LittleFS.h>

sqlite3 *db;

void db_debug_list_data() {
    sqlite3_stmt *res;
    const char *sql = "SELECT id, temp, bpm, timestamp FROM health_logs ORDER BY id DESC LIMIT 5;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &res, 0) == SQLITE_OK) {
        Serial.println("\n--- VERIFICANDO DADOS NO SQLITE ---");
        while (sqlite3_step(res) == SQLITE_ROW) {
            int id = sqlite3_column_int(res, 0);
            float t = sqlite3_column_double(res, 1);
            int b = sqlite3_column_int(res, 2);
            uint32_t ts = sqlite3_column_int64(res, 3);
            Serial.printf("ID: %d | Temp: %.1fC | BPM: %d | Time: %u\n", id, t, b, ts);
        }
        Serial.println("----------------------------------\n");
    }
    sqlite3_finalize(res);
}

bool db_init() {
    if (!LittleFS.begin(true)) return false;

    if (sqlite3_open("/littlefs/health.db", &db)) return false;

    // Lógica Code First: A tabela é reflexo da struct HealthData
    const char* sql_create = 
        "CREATE TABLE IF NOT EXISTS health_logs ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "temp REAL,"
        "humidity REAL,"
        "bpm INTEGER,"
        "avg_bpm INTEGER,"
        "timestamp INTEGER);";

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, sql_create, NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(zErrMsg);
        return false;
    }
    return true;
}

bool db_save_health_data(HealthData data) {
    char sql[150];
    snprintf(sql, sizeof(sql), 
        "INSERT INTO health_logs (temp, humidity, bpm, avg_bpm, timestamp) VALUES (%.2f, %.2f, %d, %d, %ld);",
        data.temperature, data.humidity, data.bpm, data.avgBpm, millis());

    return sqlite3_exec(db, sql, NULL, NULL, NULL) == SQLITE_OK;
}

void db_export_to_bluetooth() {
    sqlite3_stmt *res;
    // Buscamos todos os campos para preencher a struct HealthData completamente
    const char *sql = "SELECT temp, humidity, bpm, avg_bpm, timestamp FROM health_logs"; 

    if (sqlite3_prepare_v2(db, sql, -1, &res, 0) == SQLITE_OK) {
        Serial.println("[SQL] Iniciando exportação para Bluetooth...");
        
        while (sqlite3_step(res) == SQLITE_ROW) {
            HealthData temp_data;
            temp_data.temperature = sqlite3_column_double(res, 0);
            temp_data.humidity    = sqlite3_column_double(res, 1);
            temp_data.bpm         = sqlite3_column_int(res, 2);
            temp_data.avgBpm      = sqlite3_column_int(res, 3);
            temp_data.timestamp   = sqlite3_column_int64(res, 4);

            // Envia para a característica BLE
            bluetooth_health_data(temp_data);
            
            // Delay vital: o BLE precisa de tempo para processar cada pacote de notificação
            delay(40); 
        }
        sqlite3_finalize(res);
    }
    Serial.println("[SQL] Exportação para Bluetooth concluída.");
}

void monitorar_memorias() {
    // 1. Uso do LittleFS (onde o SQLite armazena os dados)
    size_t fs_total = LittleFS.totalBytes();
    size_t fs_used = LittleFS.usedBytes();
    float fs_perc = (fs_used / (float)fs_total) * 100.0;

    // 2. Uso da Flash de Programa 
    size_t sketch_size = ESP.getSketchSize();
    size_t sketch_total = ESP.getFreeSketchSpace() + sketch_size;
    float sketch_perc = (sketch_size / (float)sketch_total) * 100.0;

    Serial.println("\n--- MONITOR DE MEMÓRIA ---");
    Serial.printf("LittleFS (Dados): %.2f%% ocupado (%d / %d bytes)\n", fs_perc, fs_used, fs_total);
    Serial.printf("Flash (Código):  %.2f%% ocupado (%d / %d bytes)\n", sketch_perc, sketch_size, sketch_total);
    
    // Dica extra: Monitorar RAM livre também é útil para o BLE
    Serial.printf("RAM Livre:       %d bytes\n", ESP.getFreeHeap());
    Serial.println("--------------------------\n");
}
