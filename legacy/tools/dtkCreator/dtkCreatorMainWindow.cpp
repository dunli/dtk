/* dtkCreatorMainWindow.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Mon Aug  3 17:40:34 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Mon Jan 20 14:46:40 2014 (+0100)
 *           By: Selim Kraria
 *     Update #: 1845
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "dtkCreatorMainWindow.h"
#include "dtkCreatorMainWindow_p.h"

#include <dtkDistributed/dtkDistributor.h>

#include <dtkComposer/dtkComposer.h>
#include <dtkComposer/dtkComposerCompass.h>
#include <dtkComposer/dtkComposerControls.h>
#include <dtkComposer/dtkComposerEvaluator.h>
#include <dtkComposer/dtkComposerFactoryView.h>
#include <dtkComposer/dtkComposerGraph.h>
#include <dtkComposer/dtkComposerGraphView.h>
#include <dtkComposer/dtkComposerScene.h>
#include <dtkComposer/dtkComposerSceneModel.h>
#include <dtkComposer/dtkComposerSceneNodeEditor.h>
#include <dtkComposer/dtkComposerSceneView.h>
#include <dtkComposer/dtkComposerStack.h>
#include <dtkComposer/dtkComposerStackView.h>
#include <dtkComposer/dtkComposerView.h>

#include <dtkGui/dtkScreenMenu.h>
#include <dtkGui/dtkRecentFilesMenu.h>
#include <dtkGui/dtkSpacer.h>
#include <dtkGui/dtkSplitter.h>
#include <dtkGui/dtkViewManager.h>

#include <dtkCore/dtkGlobal.h>
#include <dtkCore/dtkPluginManager.h>

#if defined(DTK_BUILD_PLOT) && defined(DTK_HAVE_PLOT)
#include <dtkPlot/dtkPlotView.h>
#include <dtkPlot/dtkPlotViewSettings.h>
#endif

#include <dtkLog/dtkLog.h>
#include <dtkLog/dtkLogView.h>

#include <dtkNotification/dtkNotification.h>
#include <dtkNotification/dtkNotificationDisplay.h>

#include <QtCore>
#include <QtGui>

// /////////////////////////////////////////////////////////////////
// dtkCreatorMainWindowPrivate
// /////////////////////////////////////////////////////////////////

bool dtkCreatorMainWindowPrivate::maySave(void)
{
    if(this->closing)
        return true;

    if (q->isWindowModified()) {
        QMessageBox::StandardButton ret = QMessageBox::warning(q,
            q->tr("Creator"),
            q->tr("The composition has been modified.\n Do you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Save);

        if (ret == QMessageBox::Save)
            return q->compositionSave();
        else
            if(ret == QMessageBox::Cancel)
                return false;
    }

    return true;
}

void dtkCreatorMainWindowPrivate::setCurrentFile(const QString &file)
{
     this->current_composition = file;

     q->setWindowModified(false);

     QString shownName = this->current_composition;
     if (shownName.isEmpty())
         shownName = "untitled.dtk";

     q->setWindowFilePath(shownName);
}

void dtkCreatorMainWindowPrivate::setModified(bool modified)
{
    q->setWindowModified(modified);
}

// /////////////////////////////////////////////////////////////////
// dtkCreatorMainWindow
// /////////////////////////////////////////////////////////////////

dtkCreatorMainWindow::dtkCreatorMainWindow(QWidget *parent) : QMainWindow(parent), d(new dtkCreatorMainWindowPrivate)
{
    d->q = this;
    d->wl = 0;
    d->wr = 0;

    // --

    this->readSettings();

    // -- Elements

    // -- to be encupsulated within distributed layer

    d->distributor = new dtkDistributor(this);
    d->distributor->setVisible(false);

    //

    d->composer = new dtkComposer;
    d->composer->view()->setBackgroundBrush(QBrush(QPixmap(":dtkCreator/pixmaps/dtkComposerScene-bg.png")));
    d->composer->view()->setCacheMode(QGraphicsView::CacheBackground);

    d->controls = NULL;

    d->editor = new dtkComposerSceneNodeEditor(this);
    d->editor->setScene(d->composer->scene());
    d->editor->setStack(d->composer->stack());
    d->editor->setGraph(d->composer->graph());

    d->model = new dtkComposerSceneModel(this);
    d->model->setScene(d->composer->scene());

    d->scene = new dtkComposerSceneView(this);
    d->scene->setScene(d->composer->scene());
    d->scene->setModel(d->model);

    d->stack = new dtkComposerStackView(this);
    d->stack->setStack(d->composer->stack());

    d->nodes = new dtkComposerFactoryView(this);
    d->nodes->setFactory(d->composer->factory());

    d->graph = new dtkComposerGraphView(this);
    d->graph->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    d->graph->setScene(d->composer->graph());
    d->graph->setVisible(false);
    d->graph->setBackgroundBrush(QBrush(QPixmap(":dtkCreator/pixmaps/dtkComposerGraphView-bg.png")));

    d->log_view = new dtkLogView(this);
    d->log_view->setVisible(false);

    d->view_manager = new dtkViewManager;
#if defined(DTK_BUILD_PLOT) && defined(DTK_HAVE_PLOT)
    d->plot_view_settings = new dtkPlotViewSettings(d->view_manager);
    d->view_manager->addWidget(d->plot_view_settings);
#endif
    d->view_manager->setVisible(false);

    d->closing = false;

    // -- Actions

    d->composition_open_action = new QAction("Open", this);
    d->composition_open_action->setShortcut(QKeySequence::Open);

    d->composition_save_action = new QAction("Save", this);
    d->composition_save_action->setShortcut(QKeySequence::Save);

    d->composition_saveas_action = new QAction("Save As...", this);
    d->composition_saveas_action->setShortcut(QKeySequence::SaveAs);

    d->composition_insert_action = new QAction("Insert", this);
    d->composition_insert_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_I);

    d->composition_quit_action = new QAction("Quit", this);
    d->composition_quit_action->setShortcut(QKeySequence::Quit);

    d->undo_action = d->composer->stack()->createUndoAction(this);
    d->undo_action->setShortcut(QKeySequence::Undo);

    d->redo_action = d->composer->stack()->createRedoAction(this);
    d->redo_action->setShortcut(QKeySequence::Redo);

    QAction *switchToCompoAction = new QAction("Switch to composition perspective", this);
    QAction *switchToDstrbAction = new QAction("Switch to distributed perspective", this);
    QAction *switchToDebugAction = new QAction("Switch to debug perspective", this);
    QAction *switchToViewAction = new QAction("Switch to view perspective", this);

    switchToCompoAction->setShortcut(Qt::ControlModifier + Qt::AltModifier + Qt::Key_1);
    switchToDstrbAction->setShortcut(Qt::ControlModifier + Qt::AltModifier + Qt::Key_2);
    switchToDebugAction->setShortcut(Qt::ControlModifier + Qt::AltModifier + Qt::Key_3);
    switchToViewAction->setShortcut(Qt::ControlModifier + Qt::AltModifier + Qt::Key_4);

    this->addAction(switchToCompoAction);
    this->addAction(switchToDstrbAction);
    this->addAction(switchToDebugAction);
    this->addAction(switchToViewAction);

    // -- Toolbar

    QToolBar *mainToolBar = this->addToolBar(tr("Main"));
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mainToolBar->setIconSize(QSize(32, 32));

    QAction *run_action = mainToolBar->addAction(QIcon(":dtkCreator/pixmaps/dtkCreatorToolbarButton_Run_Active.png"), "Run");
    run_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_R);

    QAction *step_action = mainToolBar->addAction(QIcon(":dtkCreator/pixmaps/dtkCreatorToolbarButton_Step_Active.png"), "Step");
    step_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_N);

    QAction *continue_action = mainToolBar->addAction(QIcon(":dtkCreator/pixmaps/dtkCreatorToolbarButton_Continue_Active.png"), "Cont");
    continue_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_C);

    QAction *next_action = mainToolBar->addAction(QIcon(":dtkCreator/pixmaps/dtkCreatorToolbarButton_Continue_Active.png"), "Next");
    next_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_T);

    QAction *stop_action = mainToolBar->addAction(QIcon(":dtkCreator/pixmaps/dtkCreatorToolbarButton_Stop_Active.png"), "Stop");
    stop_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_Period);

    QAction *reset_action = mainToolBar->addAction(QIcon(":dtkCreator/pixmaps/dtkCreatorToolbarButton_Reset_Active.png"), "Reset");
    reset_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_D);

    QFrame *buttons = new QFrame(this);
    buttons->setObjectName("dtkCreatorMainWindowSegmentedButtons");

    d->compo_button = new QPushButton("Composition", buttons);
    d->compo_button->setObjectName("dtkCreatorMainWindowSegmentedButtonLeft");
    d->compo_button->setFixedSize(75, 25);
    d->compo_button->setCheckable(true);
    d->compo_button->setChecked(true);

    d->distr_button = new QPushButton("Distribution", buttons);
    d->distr_button->setObjectName("dtkCreatorMainWindowSegmentedButtonMiddle");
    d->distr_button->setFixedSize(75, 25);
    d->distr_button->setCheckable(true);

    d->debug_button = new QPushButton("Debug", buttons);
    d->debug_button->setObjectName("dtkCreatorMainWindowSegmentedButtonRight");
    d->debug_button->setFixedSize(75, 25);
    d->debug_button->setCheckable(true);

    d->view_button = new QPushButton("View", buttons);
    d->view_button->setObjectName("dtkCreatorMainWindowSegmentedButtonRight");
    d->view_button->setFixedSize(75, 25);
    d->view_button->setCheckable(true);
#if defined(DTK_BUILD_PLOT) && defined(DTK_HAVE_PLOT)
#else
    d->view_button->setVisible(false);
#endif

    QButtonGroup *button_group = new QButtonGroup(this);
    button_group->setExclusive(true);
    button_group->addButton(d->compo_button);
    button_group->addButton(d->distr_button);
    button_group->addButton(d->debug_button);
#if defined(DTK_BUILD_PLOT) && defined(DTK_HAVE_PLOT)
    button_group->addButton(d->view_button);
#endif

    QHBoxLayout *buttons_layout = new QHBoxLayout(buttons);
    buttons_layout->setMargin(0);
    buttons_layout->setSpacing(11);
    buttons_layout->addWidget(d->compo_button);
    buttons_layout->addWidget(d->distr_button);
    buttons_layout->addWidget(d->debug_button);
#if defined(DTK_BUILD_PLOT) && defined(DTK_HAVE_PLOT)
    buttons_layout->addWidget(d->view_button);
#endif

    mainToolBar->addWidget(new dtkSpacer(this));
    mainToolBar->addWidget(new dtkNotificationDisplay(this));
    mainToolBar->addWidget(new dtkSpacer(this));
    mainToolBar->addWidget(buttons);

    // -- Menus

    QMenuBar *menu_bar = this->menuBar();

    d->recent_compositions_menu = new dtkRecentFilesMenu("Open recent...", this);

    d->composition_menu = menu_bar->addMenu("Composition");
    d->composition_menu->addAction(d->composition_open_action);
    d->composition_menu->addMenu(d->recent_compositions_menu);
    d->composition_menu->addAction(d->composition_save_action);
    d->composition_menu->addAction(d->composition_saveas_action);
    d->composition_menu->addSeparator();
    d->composition_menu->addAction(d->composition_insert_action);
    d->composition_menu->addSeparator();
    d->composition_menu->addAction(d->composition_quit_action);


    d->edit_menu = menu_bar->addMenu("Edit");
    d->edit_menu->addAction(d->composer->view()->searchAction());
    d->edit_menu->addSeparator();
    d->edit_menu->addAction(d->undo_action);
    d->edit_menu->addAction(d->redo_action);
    d->edit_menu->addSeparator();
    d->edit_menu->addAction(d->composer->scene()->flagAsBlueAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsGrayAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsGreenAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsOrangeAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsPinkAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsRedAction());
    d->edit_menu->addAction(d->composer->scene()->flagAsYellowAction());
    d->edit_menu->addSeparator();
    d->edit_menu->addAction(d->composer->scene()->maskEdgesAction());
    d->edit_menu->addAction(d->composer->scene()->unmaskEdgesAction());

    QMenu *view_menu = menu_bar->addMenu("View");
    view_menu->addAction(switchToCompoAction);
    view_menu->addAction(switchToDstrbAction);
    view_menu->addAction(switchToDebugAction);
    view_menu->addAction(switchToViewAction);

    dtkScreenMenu *screen_menu = new dtkScreenMenu("Screen",this);
    menu_bar->addMenu(screen_menu);

    QAction *showControlsAction = new QAction("Show controls", this);
    showControlsAction->setShortcut(QKeySequence(Qt::ShiftModifier + Qt::ControlModifier + Qt::AltModifier + Qt::Key_C));

    QMenu *window_menu = menu_bar->addMenu("Window");
    window_menu->addAction(showControlsAction);

    QMenu *debug_menu = menu_bar->addMenu("Debug");
    debug_menu->addAction(run_action);
    debug_menu->addAction(step_action);
    debug_menu->addAction(continue_action);
    debug_menu->addAction(next_action);
    debug_menu->addAction(stop_action);
    debug_menu->addAction(reset_action);

    // -- Connections

    connect(run_action, SIGNAL(triggered()), d->composer, SLOT(run()));
    connect(step_action, SIGNAL(triggered()), d->composer, SLOT(step()));
    connect(continue_action, SIGNAL(triggered()), d->composer, SLOT(cont()));
    connect(next_action, SIGNAL(triggered()), d->composer, SLOT(next()));
    connect(stop_action, SIGNAL(triggered()), d->composer, SLOT(stop()));
    connect(reset_action, SIGNAL(triggered()), d->composer, SLOT(reset()));

    connect(switchToCompoAction, SIGNAL(triggered()), this, SLOT(switchToCompo()));
    connect(switchToDstrbAction, SIGNAL(triggered()), this, SLOT(switchToDstrb()));
    connect(switchToDebugAction, SIGNAL(triggered()), this, SLOT(switchToDebug()));
    connect(switchToViewAction, SIGNAL(triggered()), this, SLOT(switchToView()));

    connect(showControlsAction, SIGNAL(triggered()), this, SLOT(showControls()));

    connect(d->view_manager, SIGNAL(focused(dtkAbstractView *)), this, SLOT(onViewFocused(dtkAbstractView *)));

    connect(d->compo_button, SIGNAL(pressed()), this, SLOT(switchToCompo()));
    connect(d->distr_button, SIGNAL(pressed()), this, SLOT(switchToDstrb()));
    connect(d->debug_button, SIGNAL(pressed()), this, SLOT(switchToDebug()));
    connect(d->view_button, SIGNAL(pressed()), this, SLOT(switchToView()));

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(close()));

    connect(d->composer, SIGNAL(modified(bool)), d, SLOT(setModified(bool)));

    connect(d->composition_open_action, SIGNAL(triggered()), this, SLOT(compositionOpen()));
    connect(d->composition_save_action, SIGNAL(triggered()), this, SLOT(compositionSave()));
    connect(d->composition_saveas_action, SIGNAL(triggered()), this, SLOT(compositionSaveAs()));
    connect(d->composition_insert_action, SIGNAL(triggered()), this, SLOT(compositionInsert()));
    connect(d->composition_quit_action, SIGNAL(triggered()), qApp, SLOT(quit()));

    connect(d->recent_compositions_menu, SIGNAL(recentFileTriggered(const QString&)), this, SLOT(compositionOpen(const QString&)));

    // -- Layout

    dtkSplitter *left = new dtkSplitter(this);
    left->setOrientation(Qt::Vertical);
    left->addWidget(d->nodes);
    left->addWidget(d->distributor);
    left->addWidget(d->view_manager);

    dtkSplitter *right = new dtkSplitter(this);
    right->setOrientation(Qt::Vertical);
    right->addWidget(d->scene);
    right->addWidget(d->editor);
    right->addWidget(d->stack);
    right->addWidget(d->composer->compass());
    right->setSizes(QList<int>()
                    << this->size().height()/4
                    << this->size().height()/4
                    << this->size().height()/4
                    << this->size().height()/4);

    d->inner = new dtkSplitter(this);
    d->inner->setOrientation(Qt::Horizontal);
    d->inner->addWidget(left);
    d->inner->addWidget(d->graph);
    d->inner->addWidget(d->composer);
    d->inner->addWidget(right);
    d->inner->setSizes(QList<int>()
                    << 300
                    << 0
                    << this->size().width()-300-300
                    << 300);

    QHBoxLayout *b_layout = new QHBoxLayout;
    b_layout->setContentsMargins(0, 0, 0, 0);
    b_layout->setSpacing(0);
    b_layout->addWidget(d->log_view);

    QWidget *bottom = new QWidget(this);
    bottom->setLayout(b_layout);

    dtkSplitter *central = new dtkSplitter(this);
    central->setOrientation(Qt::Vertical);
    central->addWidget(d->inner);
    central->addWidget(bottom);

    this->setCentralWidget(central);
    this->setStyleSheet(dtkReadFile(":dtkCreator/dtkCreator.qss"));
    this->setUnifiedTitleAndToolBarOnMac(true);

#if defined(Q_WS_MAC) && (MAC_OS_X_VERSION_MAX_ALLOWED > MAC_OS_X_VERSION_10_6)
    d->enableFullScreenSupport();
#endif

    d->setCurrentFile("");

    dtkNotify(QString("Discovered %1 plugins").arg(dtkPluginManager::instance()->plugins().count()), 5000);
}

dtkCreatorMainWindow::~dtkCreatorMainWindow(void)
{
    delete d;
}

void dtkCreatorMainWindow::readSettings(void)
{
    QSettings settings("inria", "dtk");
    settings.beginGroup("creator");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(600, 400)).toSize();
    move(pos);
    resize(size);
    settings.endGroup();
}

void dtkCreatorMainWindow::writeSettings(void)
{
    QSettings settings("inria", "dtk");
    settings.beginGroup("creator");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.endGroup();
}

bool dtkCreatorMainWindow::compositionOpen(void)
{
    if(!d->maySave())
        return true;

    QSettings settings("inria", "dtk");
    settings.beginGroup("creator");
    QString path = settings.value("last_open_dir", QDir::homePath()).toString();
    settings.endGroup();

    QFileDialog *dialog = new QFileDialog(this, tr("Open composition"), path, QString("dtk composition (*.dtk)"));
    dialog->setStyleSheet("background-color: none ; color: none;");
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->open(this, SLOT(compositionOpen(const QString&)));

    return true;
}

bool dtkCreatorMainWindow::compositionOpen(const QString& file)
{
    if(sender() == d->recent_compositions_menu && !d->maySave())
        return true;

    bool status = d->composer->open(file);

    if(status) {
        d->recent_compositions_menu->addRecentFile(file);
        d->setCurrentFile(file);
    }

    QFileInfo info(file);

    QSettings settings("inria", "dtk");
    settings.beginGroup("creator");
    settings.setValue("last_open_dir", info.absolutePath());
    settings.endGroup();

    if(status)
        dtkNotify(QString("<div style=\"color: #006600\">Opened %1</div>").arg(info.baseName()), 3000);

    return status;
}

bool dtkCreatorMainWindow::compositionSave(void)
{
    bool status;

    if(d->current_composition.isEmpty() || d->current_composition == "untitled.dtk")
        status = this->compositionSaveAs();
    else
        status = d->composer->save();

    if(status)
        this->setWindowModified(false);

    if(status)
        dtkNotify(QString("<div style=\"color: #006600\">Saved %1</div>").arg(d->current_composition), 3000);

    return status;
}

bool dtkCreatorMainWindow::compositionSaveAs(void)
{
    bool status = false;

    QSettings settings("inria", "dtk");
    settings.beginGroup("creator");
    QString path = settings.value("last_open_dir", QDir::homePath()).toString();
    settings.endGroup();

    QStringList nameFilters;
    nameFilters <<  "Ascii composition (*.dtk)";
    nameFilters << "Binary composition (*.dtk)";

    QFileDialog dialog(this, "Save composition as ...", path, QString("dtk composition (*.dtk)"));
    dialog.setStyleSheet("background-color: none ; color: none;");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setConfirmOverwrite(true);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilters(nameFilters);
    dialog.setDefaultSuffix("dtk");

    if(dialog.exec()) {

        if(dialog.selectedNameFilter() == nameFilters.at(0))
            status = this->compositionSaveAs(dialog.selectedFiles().first(), dtkComposerWriter::Ascii);
        else
            status = this->compositionSaveAs(dialog.selectedFiles().first(), dtkComposerWriter::Binary);
    }

    return status;
}

bool dtkCreatorMainWindow::compositionSaveAs(const QString& file, dtkComposerWriter::Type type)
{
    bool status = false;

    if(file.isEmpty())
        return status;

    status = d->composer->save(file, type);

    if(status) {
        d->setCurrentFile(file);
        this->setWindowModified(false);
    }

    QFileInfo info(file);

    QSettings settings("inria", "dtk");
    settings.beginGroup("creator");
    settings.setValue("last_open_dir", info.absolutePath());
    settings.endGroup();

    if(status)
        dtkNotify(QString("<div style=\"color: #006600\">Saved as %1</div>").arg(info.baseName()), 3000);

    return status;
}

bool dtkCreatorMainWindow::compositionInsert(void)
{
    QSettings settings("inria", "dtk");
    settings.beginGroup("creator");
    QString path = settings.value("last_open_dir", QDir::homePath()).toString();
    settings.endGroup();

    QFileDialog *dialog = new QFileDialog(this, tr("Insert composition"), path, QString("dtk composition (*.dtk)"));
    dialog->setStyleSheet("background-color: none ; color: none;");
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->open(this, SLOT(compositionInsert(const QString&)));

    return true;
}

bool dtkCreatorMainWindow::compositionInsert(const QString& file)
{
    bool status = d->composer->insert(file);

    if(status)
        this->setWindowModified(true);

    QFileInfo info(file);

    QSettings settings("inria", "dtk");
    settings.beginGroup("creator");
    settings.setValue("last_open_dir", info.absolutePath());
    settings.endGroup();

    return status;
}

void dtkCreatorMainWindow::switchToCompo(void)
{
    dtkNotify("Composition workspace", 2000);

    d->compo_button->blockSignals(true);
    d->compo_button->setChecked(true);
    d->compo_button->blockSignals(false);

    if(!d->wl && !d->wr) {
        d->wl = d->nodes->size().width();
        d->wr = d->stack->size().width();
    }

    d->composer->setVisible(true);
    d->composer->compass()->setVisible(true);
    d->nodes->setVisible(true);
    d->scene->setVisible(true);
    d->editor->setVisible(true);
    d->stack->setVisible(true);
    d->distributor->setVisible(false);
    d->view_manager->setVisible(false);

    d->graph->setVisible(false);
    d->log_view->setVisible(false);

    d->inner->setSizes(QList<int>() << d->wl << 0 << this->size().width() - d->wl - d->wr << d->wr);
}

void dtkCreatorMainWindow::switchToDstrb(void)
{
    dtkNotify("Distribution workspace", 2000);

    d->distr_button->blockSignals(true);
    d->distr_button->setChecked(true);
    d->distr_button->blockSignals(false);

    if(!d->wl && !d->wr) {
        d->wl = d->nodes->size().width();
        d->wr = d->stack->size().width();
    }

    d->composer->setVisible(true);
    d->composer->compass()->setVisible(true);
    d->nodes->setVisible(false);
    d->scene->setVisible(true);
    d->editor->setVisible(true);
    d->stack->setVisible(true);
    d->distributor->setVisible(true);
    d->view_manager->setVisible(false);

    d->graph->setVisible(false);
    d->log_view->setVisible(false);

    d->inner->setSizes(QList<int>() << d->wl << 0 << this->size().width() - d->wl - d->wr << d->wr);
}

void dtkCreatorMainWindow::switchToDebug(void)
{
    dtkNotify("Debug workspace", 2000);

    d->debug_button->blockSignals(true);
    d->debug_button->setChecked(true);
    d->debug_button->blockSignals(false);

    if(!d->wl && !d->wr) {
        d->wl = d->nodes->size().width();
        d->wr = d->stack->size().width();
    }

    d->composer->setVisible(true);
    d->composer->compass()->setVisible(true);
    d->nodes->setVisible(false);
    d->scene->setVisible(false);
    d->editor->setVisible(false);
    d->stack->setVisible(false);
    d->distributor->setVisible(false);
    d->view_manager->setVisible(false);

    d->graph->setVisible(true);
    d->log_view->setVisible(true);

    int w = this->size().width() - d->wl - d->wr;

    d->inner->setSizes(QList<int>() << d->wl << w/2 << w/2 << d->wr);
}

void dtkCreatorMainWindow::switchToView(void)
{
    dtkNotify("View workspace", 2000);

    d->view_button->blockSignals(true);
    d->view_button->setChecked(true);
    d->view_button->blockSignals(false);

    if(!d->wl && !d->wr) {
        d->wl = d->nodes->size().width();
        d->wr = d->stack->size().width();
    }

    d->composer->setVisible(false);
    d->composer->compass()->setVisible(false);
    d->nodes->setVisible(false);
    d->scene->setVisible(false);
    d->editor->setVisible(false);
    d->stack->setVisible(false);
    d->distributor->setVisible(false);
    d->view_manager->setVisible(true);

    d->graph->setVisible(false);
    d->log_view->setVisible(false);
}

void dtkCreatorMainWindow::showControls(void)
{
    if(!d->controls) {
        d->controls = new dtkComposerControls(this);
        d->controls->setScene(d->composer->scene());
        d->controls->setWindowFlags(Qt::Dialog);
        d->controls->setWindowTitle("Composer Controls");

        if(!this->isFullScreen()) {
            d->controls->resize(d->controls->size().width(), this->size().height());
            d->controls->move(this->rect().topRight() + QPoint(10, 0));
        }
    }

    d->controls->show();
}

void dtkCreatorMainWindow::closeEvent(QCloseEvent *event)
{
    if (d->maySave()) {
         writeSettings();
         d->closing = true;
         event->accept();
     } else {
         event->ignore();
     }
}

void dtkCreatorMainWindow::onViewFocused(dtkAbstractView *view)
{
    if (!view)
        return;

#if defined(DTK_BUILD_PLOT) && defined(DTK_HAVE_PLOT)
    if(dtkPlotView *v = dynamic_cast<dtkPlotView *>(view)) {
        d->plot_view_settings->setView(v);
        d->view_manager->setCurrentWidget(d->plot_view_settings);
    }
#endif
}
