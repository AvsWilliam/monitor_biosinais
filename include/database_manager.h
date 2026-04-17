#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <sqlite3.h>
#include "models.h"

// Inicializa o banco de dados e cria as tabelas (Code First)
bool db_init();

// Salva uma instância do modelo no banco
bool db_save_health_data(HealthData data);

// Função de debug para listar os últimos registros do banco
void db_debug_list_data();


// Exporta os dados para o Bluetooth
void db_export_to_bluetooth();

// Função para monitorar o uso do armazenamento
void monitorar_memorias();


#endif