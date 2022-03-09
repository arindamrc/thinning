#include "thinningmainwindow.h"
#include "ui_thinningmainwindow.h"

ThinningMainWindow::ThinningMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ThinningMainWindow)
{
    ui->setupUi(this);
    setWindowTitle(Config::winTitle);
    setMinimumHeight(Config::winHeight);
    setMinimumWidth(Config::winWidth);
    this->gridWidget = new GridWidget(this);
    setCentralWidget(gridWidget);
    createMenus();
}

ThinningMainWindow::~ThinningMainWindow()
{
    delete gridWidget;
    delete ui;
}

void ThinningMainWindow::createMenus()
{
}

