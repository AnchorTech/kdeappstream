#ifndef APPLICATIONWRAPPERTHREAD_H
#define APPLICATIONWRAPPERTHREAD_H

#include <QThread>

class ApplicationWrapperThread : public QThread
{
        Q_OBJECT

        QString applicationName;

        bool _exists;

        bool _crashed;

    public:

        explicit ApplicationWrapperThread(QString applicationName, QObject * parent = 0);

        bool exists() const;

        bool crashed() const;

    protected:

        void run();

};

#endif // APPLICATIONWRAPPERTHREAD_H
