#include "ProjectSetupWidget.h"
#include "WorkflowController.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>

ProjectSetupWidget::ProjectSetupWidget(QWidget* parent)
    : QWidget(parent)
    , projectNameEdit_{nullptr}
    , locationEdit_{nullptr}
    , usCustomaryRadio_{nullptr}
    , siMetricRadio_{nullptr}
    , unitSystemGroup_{nullptr}
    , workflowController_{nullptr}
    , suppressUnitChangeWarning_{false}
{
    setup_ui();
    apply_styling();

    connect(projectNameEdit_, &QLineEdit::textChanged, this, &ProjectSetupWidget::data_changed);
    connect(locationEdit_, &QLineEdit::textChanged, this, &ProjectSetupWidget::data_changed);
    connect(unitSystemGroup_, &QButtonGroup::buttonClicked, this, &ProjectSetupWidget::on_unit_system_changed);
}

ProjectSetupWidget::~ProjectSetupWidget()
{
}

QString ProjectSetupWidget::get_project_name() const
{
    return projectNameEdit_->text();
}

QString ProjectSetupWidget::get_location() const
{
    return locationEdit_->text();
}

bool ProjectSetupWidget::is_us_customary() const
{
    return usCustomaryRadio_->isChecked();
}

bool ProjectSetupWidget::is_complete() const
{
    return !projectNameEdit_->text().isEmpty();
}

void ProjectSetupWidget::set_workflow_controller(WorkflowController* controller)
{
    workflowController_ = controller;
}

void ProjectSetupWidget::on_unit_system_changed()
{
    if(suppressUnitChangeWarning_)
    {
        emit data_changed();
        return;
    }

    // Check if there's data in downstream stages
    if(has_downstream_data())
    {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Change Unit System");
        msgBox.setText("Changing the unit system will clear all entered data.");
        msgBox.setInformativeText("All geometry, hydraulic parameters, and results will be erased. Do you want to continue?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        int response = msgBox.exec();

        if(response == QMessageBox::Yes)
        {
            // User confirmed - emit signal to clear data
            emit unit_system_changed_with_data_clear();
            emit data_changed();
        }
        else
        {
            // User cancelled - revert unit system selection
            suppressUnitChangeWarning_ = true;
            bool shouldBeUsCustomary = !is_us_customary();
            if(shouldBeUsCustomary)
                usCustomaryRadio_->setChecked(true);
            else
                siMetricRadio_->setChecked(true);
            suppressUnitChangeWarning_ = false;
        }
    }
    else
    {
        // No downstream data, just emit change
        emit data_changed();
    }
}

bool ProjectSetupWidget::has_downstream_data() const
{
    if(!workflowController_)
        return false;

    return workflowController_->has_any_data_entered();
}

void ProjectSetupWidget::setup_ui()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->setSpacing(15);
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    projectNameEdit_ = new QLineEdit();
    projectNameEdit_->setPlaceholderText("Enter project name");
    projectNameEdit_->setMinimumWidth(300);
    formLayout->addRow("Project Name:", projectNameEdit_);

    locationEdit_ = new QLineEdit();
    locationEdit_->setPlaceholderText("Enter location (optional)");
    locationEdit_->setMinimumWidth(300);
    formLayout->addRow("Location:", locationEdit_);

    mainLayout->addLayout(formLayout);

    QGroupBox* unitSystemGroup = new QGroupBox("Unit System");
    QVBoxLayout* radioLayout = new QVBoxLayout();
    radioLayout->setSpacing(15);

    usCustomaryRadio_ = new QRadioButton("US Customary (ft, cfs)");
    siMetricRadio_ = new QRadioButton("SI Metric (m, m³/s)");

    usCustomaryRadio_->setChecked(true);

    unitSystemGroup_ = new QButtonGroup(this);
    unitSystemGroup_->addButton(usCustomaryRadio_, 0);
    unitSystemGroup_->addButton(siMetricRadio_, 1);

    radioLayout->addWidget(usCustomaryRadio_);
    radioLayout->addWidget(siMetricRadio_);
    radioLayout->addStretch();

    unitSystemGroup->setLayout(radioLayout);
    mainLayout->addWidget(unitSystemGroup);

    mainLayout->addStretch();
}

void ProjectSetupWidget::apply_styling()
{
    setStyleSheet(
        "QLabel { color: #c0c0c0; font-size: 13px; }"
        "QLineEdit { "
        "  background-color: #4a4a4a; "
        "  color: #ffffff; "
        "  border: 1px solid #5a5a5a; "
        "  border-radius: 3px; "
        "  padding: 6px; "
        "  font-size: 13px; "
        "}"
        "QLineEdit:focus { border: 1px solid #0078d4; }"
        "QGroupBox { "
        "  color: #c0c0c0; "
        "  font-size: 13px; "
        "  font-weight: bold; "
        "  border: 1px solid #5a5a5a; "
        "  border-radius: 5px; "
        "  margin-top: 10px; "
        "  padding-top: 10px; "
        "}"
        "QGroupBox::title { "
        "  subcontrol-origin: margin; "
        "  subcontrol-position: top left; "
        "  padding: 0 5px; "
        "  left: 10px; "
        "}"
        "QRadioButton { color: #c0c0c0; font-size: 13px; }"
        "QRadioButton::indicator { width: 16px; height: 16px; }"
        );
}
