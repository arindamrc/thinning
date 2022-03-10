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

    QAction *toggleUpdateAction = new QAction(tr("Toggle Update"), this);
    toggleUpdateAction->setShortcut(QKeySequence("U"));
    toggleUpdateAction->setToolTip("Toggle grid update");
    toggleUpdateAction->setCheckable(true);
    toggleUpdateAction->setChecked(gridWidget->isUpdateEnabled());
    connect(toggleUpdateAction, SIGNAL(triggered()), gridWidget, SLOT(toggleUpdate()));
    appToolBar->addAction(toggleUpdateAction);

    QAction *oneIterationAction = new QAction(tr("Next Thinning Iteration"), this);
    oneIterationAction->setShortcut(QKeySequence("N"));
    oneIterationAction->setToolTip("Next step or iteration of thinning algorithm");
    connect(oneIterationAction, SIGNAL(triggered()), gridWidget, SLOT(iterate()));
    appToolBar->addAction(oneIterationAction);

    QAction *oneSubIterationAction = new QAction(tr("Next Thinning Sub-Iteration"), this);
    oneSubIterationAction->setShortcut(QKeySequence("Alt+N"));
    oneSubIterationAction->setToolTip("Next sub-step or sub-iteration of thinning algorithm");
    connect(oneSubIterationAction, SIGNAL(triggered()), gridWidget, SLOT(subIterate()));
    appToolBar->addAction(oneSubIterationAction);

    QAction *finalResultAction = new QAction(tr("Thinning final result"));
    finalResultAction->setShortcut(QKeySequence("T"));
    finalResultAction->setToolTip("The final result of the thinning algorithm");
    connect(finalResultAction, SIGNAL(triggered()), gridWidget, SLOT(result()));
    appToolBar->addAction(finalResultAction);
}

