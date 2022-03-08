#include "thinningmainwindow.h"
#include "ui_thinningmainwindow.h"

ThinningMainWindow::ThinningMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ThinningMainWindow)
{
    ui->setupUi(this);
}

ThinningMainWindow::~ThinningMainWindow()
{
    delete ui;
}

