#include "thinningmainwindow.h"
#include "ui_thinningmainwindow.h"

#include <QToolBar>

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
    createToolbar();
}

ThinningMainWindow::~ThinningMainWindow()
{
    delete gridWidget;
    delete ui;
}

void ThinningMainWindow::createToolbar()
{
    QToolBar *appToolBar = addToolBar(tr("App Toolbar"));

    QAction *clearAction = new QAction(tr("Clear Grid"), this);
    clearAction->setShortcut(QKeySequence("Ctrl+D"));
    clearAction->setToolTip("Clear the whole grid");
    connect(clearAction, SIGNAL(triggered()), gridWidget, SLOT(clear()));
    appToolBar->addAction(clearAction);

    QAction *oneStepAction = new QAction(tr("Next Thinning Step"), this);
    oneStepAction->setShortcut(QKeySequence("N"));
    oneStepAction->setToolTip("Next step or iteration of thinning algorithm");
    connect(oneStepAction, SIGNAL(triggered()), gridWidget, SLOT(step()));
    appToolBar->addAction(oneStepAction);

    QAction *finalResultAction = new QAction(tr("Thinning final result"));
    finalResultAction->setShortcut(QKeySequence("T"));
    finalResultAction->setToolTip("The final result of the thinning algorithm");
    connect(finalResultAction, SIGNAL(triggered()), gridWidget, SLOT(result()));
}

