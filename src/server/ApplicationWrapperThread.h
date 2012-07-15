#ifndef APPLICATIONWRAPPERTHREAD_H
#define APPLICATIONWRAPPERTHREAD_H

#include <QThread>

class ApplicationWrapperThread : public QThread
{
        Q_OBJECT

        QString applicationName;

    public:

        explicit ApplicationWrapperThread(QString applicationName, QObject * parent = 0);

    protected:

        void run();

};

#endif // APPLICATIONWRAPPERTHREAD_H
