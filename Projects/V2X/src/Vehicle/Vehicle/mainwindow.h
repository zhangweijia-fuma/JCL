#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "CString.h"
#include "MsgSocket.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    protected:
        CSocket             m_Socket;
        UINT                m_nRSUPort;

        void                SendMessage(int nChoiceIndex);

    private slots:
        void on_SPATBtn_clicked();
        void on_BSMBtn_clicked();
        void on_MapBtn_clicked();
        void on_RSIBtn_clicked();
        void on_RSMBtn_clicked();

private:
        Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
