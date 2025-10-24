#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QWidget>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "ProjectSetupWidget.h"
#include "WorkflowController.h"
#include "WorkflowTabBar.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tab_clicked(WorkflowStage stage);
    void on_project_setup_data_changed();

private:
    void setup_ui();
    void setup_menu_bar();
    void setup_layout();
    void apply_dark_theme();
    void update_unit_system_indicator();

    Ui::MainWindow* ui;

    // Layout components
    QSplitter* mainSplitter_;
    QWidget* visualizationArea_;
    QWidget* parameterPanel_;
    QLabel* unitSystemIndicator_;

    // Menu components
    QMenu* fileMenu_;
    QAction* newProjectAction_;
    QAction* openProjectAction_;
    QAction* saveAction_;
    QAction* saveAsAction_;
    QAction* exitAction_;

    // Workflow components
    WorkflowController* workflowController_;
    WorkflowTabBar* workflowTabBar_;

    // Workflow widgets
    ProjectSetupWidget* projectSetupWidget_;
};

#endif // MAINWINDOW_H
