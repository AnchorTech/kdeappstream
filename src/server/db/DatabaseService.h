#ifndef DATABASESERVICE_H
#define DATABASESERVICE_H

#include <QSqlDatabase>

class DatabaseService : public QSqlDatabase
{
    public:

        DatabaseService();
        ~DatabaseService();
};

#endif // DATABASESERVICE_H
