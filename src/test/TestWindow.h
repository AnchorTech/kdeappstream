#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>

namespace Ui {
    class TestWindow;
}

class TestWindow : public QMainWindow
{
    Q_OBJECT

    public:

        explicit TestWindow(QWidget *parent = 0);
        ~TestWindow();

    protected:

        virtual bool event(QEvent * event);

    private slots:

        void on_pushButton_clicked();

    private:

        Ui::TestWindow * ui;
};

#endif // TESTWINDOW_H
