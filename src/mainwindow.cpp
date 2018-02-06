#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "smtp.h"

#include <QDesktopWidget>

#include <QDebug>

class MainWindow::MainWindowData {
public:
    MainWindowData() {

    }
    ~MainWindowData() {

    }

};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();
    smtp* sample = new smtp( "sender@email.com","receiver@email.com","TEST MESSAGE","DON'T REPLY IN THIS MESSAGE" );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    this->setMinimumSize(600, 450);
    this->setGeometry(0, 0, 600, 450);
    move((QApplication::desktop()->width() - size().width()) / 2,
         (QApplication::desktop()->height() - size().height()) / 2);
}
