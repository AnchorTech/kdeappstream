#include "DatabaseService.h"

DatabaseService::DatabaseService() :
    QSqlDatabase("QSQLITE")
{
    this->setDatabaseName("connections.data");
    this->open();
}

DatabaseService::~DatabaseService()
{
    this->close();
}
