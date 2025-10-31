#include "WorkflowTabBar.h"
#include <QHBoxLayout>

WorkflowTabBar::WorkflowTabBar(WorkflowController* controller, QWidget* parent)
    : QWidget(parent)
    , controller_{controller}
    , tabButtons_{nullptr, nullptr, nullptr, nullptr, nullptr}
{
    setup_ui();

    connect(controller_, &WorkflowController::current_stage_changed,
            this, &WorkflowTabBar::on_stage_changed);
    connect(controller_, &WorkflowController::stage_completion_changed,
            this, &WorkflowTabBar::on_completion_changed);

    update_tab_states();
}

WorkflowTabBar::~WorkflowTabBar()
{
}

void WorkflowTabBar::setup_ui()
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    create_tab_button(WorkflowStage::ProjectSetup, "Project Setup");
    create_tab_button(WorkflowStage::GeometryDefinition, "Geometry Definition");
    create_tab_button(WorkflowStage::HydraulicParameters, "Hydraulic Parameters");
    create_tab_button(WorkflowStage::AnalysisResults, "Analysis and Results");
    create_tab_button(WorkflowStage::Export, "Export");

    for(auto* button : tabButtons_)
        layout->addWidget(button);
}

void WorkflowTabBar::create_tab_button(WorkflowStage stage, const QString& label)
{
    int index = static_cast<int>(stage);

    QPushButton* button = new QPushButton(label, this);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    button->setMinimumHeight(45);
    button->setCursor(Qt::PointingHandCursor);
    button->setProperty("stage", index);

    connect(button, &QPushButton::clicked, this, &WorkflowTabBar::on_tab_button_clicked);

    tabButtons_[index] = button;
}

void WorkflowTabBar::on_tab_button_clicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());

    if(!button)
        return;

    int stageIndex = button->property("stage").toInt();
    WorkflowStage targetStage = static_cast<WorkflowStage>(stageIndex);

    if(controller_->can_advance_to_stage(targetStage))
    {
        emit tab_clicked(targetStage);
    }
}

void WorkflowTabBar::on_stage_changed(WorkflowStage newStage)
{
    update_tab_states();
}

void WorkflowTabBar::on_completion_changed(WorkflowStage stage, bool complete)
{
    update_tab_states();
}

void WorkflowTabBar::update_tab_states()
{
    WorkflowStage currentStage = controller_->get_current_stage();

    for(size_t i = 0; i < tabButtons_.size(); ++i)
    {
        WorkflowStage stage = static_cast<WorkflowStage>(i);
        QPushButton* button = tabButtons_[i];

        update_button_style(button, stage);

        bool isActive = (stage == currentStage);
        bool isComplete = controller_->is_stage_complete(stage);
        bool canAccess = controller_->can_advance_to_stage(stage);

        button->setEnabled(canAccess);

        QString text = button->text();
        if(text.startsWith("✓ "))
            text = text.mid(2);

        if(isComplete && !isActive)
            button->setText("✓ " + text);
        else
            button->setText(text);
    }
}

void WorkflowTabBar::update_button_style(QPushButton* button, WorkflowStage stage)
{
    bool isActive = (stage == controller_->get_current_stage());
    bool isComplete = controller_->is_stage_complete(stage);
    bool isLocked = !controller_->can_advance_to_stage(stage);

    button->setStyleSheet(get_button_stylesheet(isActive, isComplete, isLocked));
}

QString WorkflowTabBar::get_button_stylesheet(bool isActive, bool isComplete, bool isLocked) const
{
    QString baseStyle =
        "QPushButton { "
        "  border: none; "
        "  border-top: 3px solid %1; "
        "  background-color: %2; "
        "  color: %3; "
        "  font-size: 12px; "
        "  font-weight: %4; "
        "  padding: 8px; "
        "  text-align: center; "
        "} "
        "QPushButton:hover:enabled { "
        "  background-color: %5; "
        "}";

    QString borderColor;
    QString backgroundColor;
    QString textColor;
    QString fontWeight;
    QString hoverColor;

    if(isActive)
    {
        borderColor = "#0078d4";
        backgroundColor = "#4a4a4a";
        textColor = "#ffffff";
        fontWeight = "bold";
        hoverColor = "#4a4a4a";
    }
    else if(isComplete)
    {
        borderColor = "#2d7d2d";
        backgroundColor = "#3c3c3c";
        textColor = "#c0c0c0";
        fontWeight = "normal";
        hoverColor = "#454545";
    }
    else if(isLocked)
    {
        borderColor = "#3c3c3c";
        backgroundColor = "#2d2d2d";
        textColor = "#707070";
        fontWeight = "normal";
        hoverColor = "#2d2d2d";
    }
    else
    {
        borderColor = "#3c3c3c";
        backgroundColor = "#3c3c3c";
        textColor = "#c0c0c0";
        fontWeight = "normal";
        hoverColor = "#454545";
    }

    return baseStyle.arg(borderColor, backgroundColor, textColor, fontWeight, hoverColor);
}
