#ifndef ACLPROVIDER_H
#define ACLPROVIDER_H

#include <QString>
#include <QStringList>

class ACLProvider
{
        QString _dir;

        QString _path;

        QStringList acceptedNames;

        QStringList rejectedNames;

        bool _acceptAll;

        bool _rejectAll;

        ACLProvider();

        static ACLProvider * m_instance;

    public:

        static ACLProvider * instance();

        void createEmptyACL();

        void accept(const QString & name);

        void reject(const QString & name);

        void acceptAll();

        void rejectAll();

        bool isAccepted(const QString & name);

        void save();

        void printInfo();

    private:
        void createPermissionsFile();
};

#endif // ACLPROVIDER_H
