#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mainSplitter_{nullptr}
    , visualizationArea_{nullptr}
    , parameterPanel_{nullptr}
    , unitSystemIndicator_{nullptr}
    , fileMenu_{nullptr}
    , newProjectAction_{nullptr}
    , openProjectAction_{nullptr}
    , saveAction_{nullptr}
    , saveAsAction_{nullptr}
    , exitAction_{nullptr}
    , workflowController_{nullptr}
    , workflowTabBar_{nullptr}
    , stageStackedWidget_{nullptr}
    , projectSetupWidget_{nullptr}
    , geometryDefinitionWidget_{nullptr}
    , hydraulicParametersWidget_{nullptr}
    , analysisResultsWidget_{nullptr}
    , exportWidget_{nullptr}
    , vtkWidget_{nullptr}
    , visualizationRendered_{false}

{
    ui->setupUi(this);

    workflowController_ = new WorkflowController(this);

    setup_ui();

    connect(projectSetupWidget_, &ProjectSetupWidget::data_changed,
            this, &MainWindow::on_project_setup_data_changed);
    connect(geometryDefinitionWidget_, &GeometryDefinitionWidget::data_changed,
            this, &MainWindow::on_geometry_data_changed);
    connect(hydraulicParametersWidget_, &HydraulicParametersWidget::data_changed,
            this, &MainWindow::on_hydraulic_parameters_data_changed);
    connect(workflowController_, &WorkflowController::current_stage_changed,
            this, &MainWindow::on_current_stage_changed);
    connect(workflowController_, &WorkflowController::calculation_completed,
            this, &MainWindow::on_calculation_completed);

    workflowController_->set_current_stage(WorkflowStage::ProjectSetup);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setup_ui()
{
    setup_menu_bar();
    setup_layout();
    apply_dark_theme();

    setWindowTitle("Hydraulic Toolbox");
    resize(1200, 800);
}

void MainWindow::setup_menu_bar()
{
    fileMenu_ = menuBar()->addMenu("File");

    newProjectAction_ = new QAction("New Project", this);
    openProjectAction_ = new QAction("Open Project", this);
    saveAction_ = new QAction("Save", this);
    saveAsAction_ = new QAction("Save As...", this);
    exitAction_ = new QAction("Exit", this);

    fileMenu_->addAction(newProjectAction_);
    fileMenu_->addAction(openProjectAction_);
    fileMenu_->addSeparator();
    fileMenu_->addAction(saveAction_);
    fileMenu_->addAction(saveAsAction_);
    fileMenu_->addSeparator();
    fileMenu_->addAction(exitAction_);

    connect(exitAction_, &QAction::triggered, this, &QMainWindow::close);

    unitSystemIndicator_ = new QLabel("US Customary", this);
    unitSystemIndicator_->setStyleSheet(
        "QLabel { "
        "  background-color: #4a4a4a; "
        "  color: #ffffff; "
        "  padding: 4px 12px; "
        "  border-radius: 3px; "
        "  font-size: 11px; "
        "}"
        );
    menuBar()->setCornerWidget(unitSystemIndicator_, Qt::TopRightCorner);
}

void MainWindow::setup_layout()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainSplitter_ = new QSplitter(Qt::Vertical, centralWidget);

    visualizationArea_ = new QWidget();
    visualizationArea_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    visualizationArea_->setMinimumHeight(600);

    QVBoxLayout* visualizationLayout = new QVBoxLayout(visualizationArea_);
    visualizationLayout->setContentsMargins(0, 0, 0, 0);
    visualizationLayout->setSpacing(0);

    vtkWidget_ = new VtkWidget();
    visualizationLayout->addWidget(vtkWidget_);

    parameterPanel_ = new QWidget();
    parameterPanel_->setStyleSheet("QWidget { background-color: #3c3c3c; }");
    parameterPanel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    QVBoxLayout* parameterLayout = new QVBoxLayout(parameterPanel_);
    parameterLayout->setContentsMargins(0, 0, 0, 0);
    parameterLayout->setSpacing(0);

    stageStackedWidget_ = new QStackedWidget(parameterPanel_);

    projectSetupWidget_ = new ProjectSetupWidget();
    geometryDefinitionWidget_ = new GeometryDefinitionWidget();
    hydraulicParametersWidget_ = new HydraulicParametersWidget();
    analysisResultsWidget_ = new AnalysisResultsWidget();
    exportWidget_ = new ExportWidget();

    stageStackedWidget_->addWidget(projectSetupWidget_);
    stageStackedWidget_->addWidget(geometryDefinitionWidget_);
    stageStackedWidget_->addWidget(hydraulicParametersWidget_);
    stageStackedWidget_->addWidget(analysisResultsWidget_);
    stageStackedWidget_->addWidget(exportWidget_);

    parameterLayout->addWidget(stageStackedWidget_);

    workflowTabBar_ = new WorkflowTabBar(workflowController_, parameterPanel_);
    parameterLayout->addWidget(workflowTabBar_);

    connect(workflowTabBar_, &WorkflowTabBar::tab_clicked,
            this, &MainWindow::on_tab_clicked);

    mainSplitter_->addWidget(visualizationArea_);
    mainSplitter_->addWidget(parameterPanel_);

    mainSplitter_->setStretchFactor(0, 2);
    mainSplitter_->setStretchFactor(1, 1);

    mainLayout->addWidget(mainSplitter_);
}

void MainWindow::apply_dark_theme()
{
    setStyleSheet(
        "QMainWindow { background-color: #2b2b2b; }"
        "QMenuBar { background-color: #3c3c3c; color: #ffffff; }"
        "QMenuBar::item { background-color: #3c3c3c; color: #ffffff; padding: 4px 12px; }"
        "QMenuBar::item:selected { background-color: #4a4a4a; }"
        "QMenu { background-color: #3c3c3c; color: #ffffff; }"
        "QMenu::item:selected { background-color: #4a4a4a; }"
        );
}

void MainWindow::on_tab_clicked(WorkflowStage stage)
{
    workflowController_->set_current_stage(stage);
}

void MainWindow::on_current_stage_changed(WorkflowStage newStage)
{
    int index = static_cast<int>(newStage);

    if(index < stageStackedWidget_->count())
    {
        stageStackedWidget_->setCurrentIndex(index);
    }

    if(newStage == WorkflowStage::AnalysisResults)
    {
        workflowController_->perform_calculation();
        workflowController_->mark_stage_complete(WorkflowStage::AnalysisResults, true);
    }
}

void MainWindow::on_project_setup_data_changed()
{
    ProjectData& data = workflowController_->get_project_data();

    data.projectName = projectSetupWidget_->get_project_name();
    data.location = projectSetupWidget_->get_location();
    data.useUsCustomary = projectSetupWidget_->is_us_customary();

    bool isComplete = projectSetupWidget_->is_complete();
    workflowController_->mark_stage_complete(WorkflowStage::ProjectSetup, isComplete);

    update_unit_system_indicator();
}

void MainWindow::on_geometry_data_changed()
{
    GeometryData& data = workflowController_->get_geometry_data();

    data.channelType = geometryDefinitionWidget_->get_channel_type();
    data.bottomWidth = geometryDefinitionWidget_->get_bottom_width();
    data.sideSlope = geometryDefinitionWidget_->get_side_slope();
    data.bedSlope = geometryDefinitionWidget_->get_bed_slope();

    bool isComplete = geometryDefinitionWidget_->is_complete();
    workflowController_->mark_stage_complete(WorkflowStage::GeometryDefinition, isComplete);
}

void MainWindow::on_hydraulic_parameters_data_changed()
{
    HydraulicData& data = workflowController_->get_hydraulic_data();

    data.discharge = hydraulicParametersWidget_->get_discharge();
    data.manningN = hydraulicParametersWidget_->get_mannings_n();

    bool isComplete = hydraulicParametersWidget_->is_complete();
    workflowController_->mark_stage_complete(WorkflowStage::HydraulicParameters, isComplete);
}

void MainWindow::update_unit_system_indicator()
{
    ProjectData& data = workflowController_->get_project_data();

    if(data.useUsCustomary)
        unitSystemIndicator_->setText("US Customary");
    else
        unitSystemIndicator_->setText("SI Metric");
}

void MainWindow::on_calculation_completed(const CalculationResults& results)
{
    ProjectData& projectData = workflowController_->get_project_data();
    analysisResultsWidget_->update_results(results, projectData.useUsCustomary);

    if(results.isValid)
    {
        GeometryData& geometryData = workflowController_->get_geometry_data();
        vtkWidget_->render_channel(geometryData, results);
        visualizationRendered_ = true;
    }
}
