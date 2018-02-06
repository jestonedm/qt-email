#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    class MainWindowData;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void init();

private:
    Ui::MainWindow *ui;
    MainWindowData *m_d;
};

#endif // MAINWINDOW_H
