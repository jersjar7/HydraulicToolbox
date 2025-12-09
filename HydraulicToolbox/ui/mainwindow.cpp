#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mainSplitter_{nullptr}
    , visualizationPanel_{nullptr}
    , parameterPanel_{nullptr}
    , unitSystemIndicator_{nullptr}
    , fileMenu_{nullptr}
    , newProjectAction_{nullptr}
    , openProjectAction_{nullptr}
    , saveAction_{nullptr}
    , saveAsAction_{nullptr}
    , exitAction_{nullptr}
    , workflowController_{nullptr}
{
    ui->setupUi(this);

    workflowController_ = new WorkflowController(this);

    setup_ui();

    connect(parameterPanel_->get_project_setup_widget(), &ProjectSetupWidget::data_changed,
            this, &MainWindow::on_project_setup_data_changed);
    connect(parameterPanel_->get_geometry_definition_widget(), &GeometryDefinitionWidget::data_changed,
            this, &MainWindow::on_geometry_data_changed);
    connect(parameterPanel_->get_hydraulic_parameters_widget(), &HydraulicParametersWidget::data_changed,
            this, &MainWindow::on_hydraulic_parameters_data_changed);
    connect(workflowController_, &WorkflowController::current_stage_changed,
            this, &MainWindow::on_current_stage_changed);
    connect(workflowController_, &WorkflowController::calculation_completed,
            this, &MainWindow::on_calculation_completed);
    connect(parameterPanel_, &ParameterPanel::tab_clicked,
            this, &MainWindow::on_tab_clicked);

    parameterPanel_->get_project_setup_widget()->set_workflow_controller(workflowController_);

    connect(parameterPanel_->get_project_setup_widget(), &ProjectSetupWidget::unit_system_changed_with_data_clear,
            this, &MainWindow::on_unit_system_changed_with_data_clear);

    connect_input_summary_updates();
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
    resize(1400, 800);
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

    visualizationPanel_ = new VisualizationPanel(workflowController_);
    parameterPanel_ = new ParameterPanel(workflowController_);

    parameterPanel_->set_current_stage(WorkflowStage::ProjectSetup);

    mainSplitter_->addWidget(visualizationPanel_);
    mainSplitter_->addWidget(parameterPanel_);

    mainSplitter_->setStretchFactor(0, 3);
    mainSplitter_->setStretchFactor(1, 1);

    QList<int> sizes;
    sizes << 600 << 200;
    mainSplitter_->setSizes(sizes);

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

void MainWindow::connect_input_summary_updates()
{
    InputSummaryWidget* summaryWidget = visualizationPanel_->get_input_summary_widget();

    if(!summaryWidget)
        return;

    // Connect to project setup widget field focus changes
    auto projectWidget = parameterPanel_->get_project_setup_widget();
    connect(projectWidget, &ProjectSetupWidget::data_changed,
            this, &MainWindow::update_input_summary);

    // Connect to geometry definition widget field focus changes
    auto geometryWidget = parameterPanel_->get_geometry_definition_widget();
    connect(geometryWidget, &GeometryDefinitionWidget::data_changed,
            this, &MainWindow::update_input_summary);

    // Connect to hydraulic parameters widget field focus changes
    auto hydraulicWidget = parameterPanel_->get_hydraulic_parameters_widget();
    connect(hydraulicWidget, &HydraulicParametersWidget::data_changed,
            this, &MainWindow::update_input_summary);

    // Connect to stage changes to update summary
    connect(workflowController_, &WorkflowController::current_stage_changed,
            this, &MainWindow::update_input_summary);
}

void MainWindow::update_input_summary()
{
    InputSummaryWidget* summaryWidget = visualizationPanel_->get_input_summary_widget();

    if(!summaryWidget)
        return;

    summaryWidget->update_project_data(workflowController_->get_project_data());
    summaryWidget->update_geometry_data(workflowController_->get_geometry_data());
    summaryWidget->update_hydraulic_data(workflowController_->get_hydraulic_data());
}

void MainWindow::on_tab_clicked(WorkflowStage stage)
{
    workflowController_->set_current_stage(stage);
}

void MainWindow::on_current_stage_changed(WorkflowStage newStage)
{
    parameterPanel_->set_current_stage(newStage);

    if(newStage == WorkflowStage::AnalysisResults)
    {
        workflowController_->perform_calculation();
        workflowController_->mark_stage_complete(WorkflowStage::AnalysisResults, true);
    }

    update_input_summary();
}

void MainWindow::on_project_setup_data_changed()
{
    ProjectData& data = workflowController_->get_project_data();

    ProjectSetupWidget* widget = parameterPanel_->get_project_setup_widget();
    data.projectName = widget->get_project_name();
    data.location = widget->get_location();
    data.useUsCustomary = widget->is_us_customary();

    bool isComplete = widget->is_complete();
    workflowController_->mark_stage_complete(WorkflowStage::ProjectSetup, isComplete);

    update_unit_system_indicator();

    parameterPanel_->get_hydraulic_parameters_widget()->update_placeholders(data.useUsCustomary);
}

void MainWindow::on_geometry_data_changed()
{
    GeometryData& data = workflowController_->get_geometry_data();

    GeometryDefinitionWidget* widget = parameterPanel_->get_geometry_definition_widget();
    data.channelType = widget->get_channel_type();
    data.bottomWidth = widget->get_bottom_width();
    data.sideSlope = widget->get_side_slope();
    data.bedSlope = widget->get_bed_slope();

    bool isComplete = widget->is_complete();
    workflowController_->mark_stage_complete(WorkflowStage::GeometryDefinition, isComplete);
}

void MainWindow::on_hydraulic_parameters_data_changed()
{
    HydraulicData& data = workflowController_->get_hydraulic_data();

    HydraulicParametersWidget* widget = parameterPanel_->get_hydraulic_parameters_widget();
    data.discharge = widget->get_discharge();
    data.manningN = widget->get_mannings_n();

    bool isComplete = widget->is_complete();
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
    parameterPanel_->get_analysis_results_widget()->update_results(results, projectData.useUsCustomary);

    if(results.isValid)
    {
        GeometryData& geometryData = workflowController_->get_geometry_data();
        visualizationPanel_->render_channel(geometryData, results);
    }
}

void MainWindow::on_unit_system_changed_with_data_clear()
{
    workflowController_->clear_all_data();

    parameterPanel_->get_geometry_definition_widget()->clear_fields();
    parameterPanel_->get_hydraulic_parameters_widget()->clear_fields();

    CalculationResults emptyResults;
    emptyResults.isValid = false;
    parameterPanel_->get_analysis_results_widget()->update_results(
        emptyResults,
        workflowController_->get_project_data().useUsCustomary);

    update_unit_system_indicator();

    parameterPanel_->get_hydraulic_parameters_widget()->update_placeholders(
        workflowController_->get_project_data().useUsCustomary);

    // Clear and update input summary
    update_input_summary();
}
