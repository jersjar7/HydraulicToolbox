#include "HydraulicParametersWidget.h"
#include "UnitSystemConstants.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QListView>
#include <QStyledItemDelegate>

HydraulicParametersWidget::HydraulicParametersWidget(QWidget* parent)
    : QWidget(parent)
    , dischargeEdit_{nullptr}
    , manningsMaterialCombo_{nullptr}
    , manningsNEdit_{nullptr}
    , formLayout_{nullptr}
{
    setup_ui();
    apply_styling();
    populate_mannings_materials();

    connect(dischargeEdit_, &QLineEdit::textChanged, this, &HydraulicParametersWidget::data_changed);
    connect(manningsNEdit_, &QLineEdit::textChanged, this, &HydraulicParametersWidget::data_changed);
    connect(manningsMaterialCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &HydraulicParametersWidget::on_material_selected);
}

HydraulicParametersWidget::~HydraulicParametersWidget()
{
}

double HydraulicParametersWidget::get_discharge() const
{
    return dischargeEdit_->text().toDouble();
}

double HydraulicParametersWidget::get_mannings_n() const
{
    return manningsNEdit_->text().toDouble();
}

bool HydraulicParametersWidget::is_complete() const
{
    return !dischargeEdit_->text().isEmpty() && !manningsNEdit_->text().isEmpty();
}

void HydraulicParametersWidget::clear_fields()
{
    dischargeEdit_->clear();
    manningsMaterialCombo_->setCurrentIndex(0);
    manningsNEdit_->clear();
}

void HydraulicParametersWidget::update_placeholders(bool useUsCustomary)
{
    QString dischargePlaceholder = QString("Enter discharge (%1)")
    .arg(useUsCustomary ? UnitSystemConstants::LABEL_DISCHARGE_US : UnitSystemConstants::LABEL_DISCHARGE_SI);

    dischargeEdit_->setPlaceholderText(dischargePlaceholder);
}

void HydraulicParametersWidget::setup_ui()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    QGroupBox* hydraulicGroup = new QGroupBox("Hydraulic Parameters");
    formLayout_ = new QFormLayout();
    formLayout_->setSpacing(15);
    formLayout_->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    dischargeEdit_ = new QLineEdit();
    dischargeEdit_->setPlaceholderText("Enter discharge");
    dischargeEdit_->setMinimumWidth(300);
    formLayout_->addRow("Discharge (Q):", dischargeEdit_);

    QGroupBox* manningsGroup = new QGroupBox("Manning's Roughness Coefficient");
    QVBoxLayout* manningsLayout = new QVBoxLayout();
    manningsLayout->setSpacing(15);

    QLabel* materialLabel = new QLabel("Channel Material:");
    manningsMaterialCombo_ = new QComboBox();
    manningsMaterialCombo_->setMinimumWidth(300);

    QListView* comboListView = new QListView(manningsMaterialCombo_);
    comboListView->setSpacing(0);
    comboListView->setFrameShape(QFrame::NoFrame);
    manningsMaterialCombo_->setView(comboListView);
    manningsMaterialCombo_->setItemDelegate(new QStyledItemDelegate(manningsMaterialCombo_));

    comboListView->setStyleSheet(
        "QListView { "
        "  background-color: #4a4a4a; "
        "  border: none; "
        "  outline: none; "
        "}"
        );

    if(manningsMaterialCombo_->view()->parentWidget())
    {
        manningsMaterialCombo_->view()->parentWidget()->setStyleSheet(
            "QWidget { "
            "  background-color: #4a4a4a; "
            "  border: none; "
            "  padding: 0px; "
            "  margin: 0px; "
            "}"
            );
    }

    manningsLayout->addWidget(materialLabel);
    manningsLayout->addWidget(manningsMaterialCombo_);

    QLabel* manningsNLabel = new QLabel("Manning's n:");
    manningsNEdit_ = new QLineEdit();
    manningsNEdit_->setPlaceholderText("Enter or select from materials");
    manningsNEdit_->setMinimumWidth(300);
    manningsLayout->addWidget(manningsNLabel);
    manningsLayout->addWidget(manningsNEdit_);

    manningsGroup->setLayout(manningsLayout);

    hydraulicGroup->setLayout(formLayout_);
    mainLayout->addWidget(hydraulicGroup);
    mainLayout->addWidget(manningsGroup);
    mainLayout->addStretch();
}

void HydraulicParametersWidget::apply_styling()
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

        // Combobox styling
        "QComboBox { "
        "  background-color: #4a4a4a; "
        "  color: #ffffff; "
        "  border: 1px solid #5a5a5a; "
        "  border-radius: 3px; "
        "  padding: 6px 20px 6px 6px; "
        "  font-size: 13px; "
        "}"
        "QComboBox:focus { border: 1px solid #0078d4; }"

        // Drop-down button
        "QComboBox::drop-down { "
        "  background-color: #4a4a4a; "
        "  border: none; "
        "  width: 20px; "
        "  subcontrol-origin: padding; "
        "  subcontrol-position: center right; "
        "}"

        // Down arrow
        "QComboBox::down-arrow { "
        "  image: none; "
        "  border-style: solid; "
        "  border-width: 6px 4px 0 4px; "
        "  border-color: #c0c0c0 transparent transparent transparent; "
        "  width: 0; "
        "  height: 0; "
        "}"

        // Hover state
        "QComboBox::down-arrow:hover { "
        "  border-color: #ffffff transparent transparent transparent; "
        "}"

        // Item view styling
        "QComboBox QAbstractItemView { "
        "  background-color: #4a4a4a; "
        "  color: #ffffff; "
        "  selection-background-color: #0078d4; "
        "  border: 1px solid #5a5a5a; "
        "  padding: 0px; "
        "  margin: 0px; "
        "  outline: none; "
        "}"

        // Item styling with proper padding
        "QComboBox QAbstractItemView::item { "
        "  padding: 6px; "
        "  min-height: 20px; "
        "}"

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
        );
}

void HydraulicParametersWidget::populate_mannings_materials()
{
    manningsMaterialCombo_->addItem("-- Select Material --", 0.0);
    manningsMaterialCombo_->addItem("PVC / Plastic Pipe (n=0.009)", 0.009);
    manningsMaterialCombo_->addItem("Concrete - Finished (n=0.012)", 0.012);
    manningsMaterialCombo_->addItem("Concrete - Unfinished (n=0.014)", 0.014);
    manningsMaterialCombo_->addItem("Concrete - Gunite (n=0.019)", 0.019);
    manningsMaterialCombo_->addItem("Earth - Clean, Straight (n=0.022)", 0.022);
    manningsMaterialCombo_->addItem("Corrugated Metal (n=0.024)", 0.024);
    manningsMaterialCombo_->addItem("Earth - Gravelly (n=0.025)", 0.025);
    manningsMaterialCombo_->addItem("Earth - Weedy (n=0.030)", 0.030);
    manningsMaterialCombo_->addItem("Natural Channel - Clean (n=0.030)", 0.030);
    manningsMaterialCombo_->addItem("Grass-Lined - Short 2-6 in (n=0.030)", 0.030);
    manningsMaterialCombo_->addItem("Earth - Stony, Cobbles (n=0.035)", 0.035);
    manningsMaterialCombo_->addItem("Grass-Lined - Medium 6-12 in (n=0.035)", 0.035);
    manningsMaterialCombo_->addItem("Natural Channel - Winding (n=0.040)", 0.040);
    manningsMaterialCombo_->addItem("Grass-Lined - Long 12+ in (n=0.050)", 0.050);
    manningsMaterialCombo_->addItem("Natural Channel - Heavy Brush (n=0.075)", 0.075);
}

void HydraulicParametersWidget::on_material_selected(int index)
{
    if(index > 0)
    {
        double nValue = manningsMaterialCombo_->currentData().toDouble();
        manningsNEdit_->setText(QString::number(nValue, 'f', 3));
    }
}
