#include "GeometryDefinitionWidget.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QListView>
#include <QStyledItemDelegate>

GeometryDefinitionWidget::GeometryDefinitionWidget(QWidget* parent)
    : QWidget(parent)
    , channelTypeCombo_{nullptr}
    , bottomWidthEdit_{nullptr}
    , sideSlopeEdit_{nullptr}
    , bedSlopeEdit_{nullptr}
    , bottomWidthLabel_{nullptr}
    , sideSlopeLabel_{nullptr}
    , formLayout_{nullptr}
{
    setup_ui();
    apply_styling();

    connect(channelTypeCombo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GeometryDefinitionWidget::on_channel_type_changed);
    connect(bottomWidthEdit_, &QLineEdit::textChanged, this, &GeometryDefinitionWidget::data_changed);
    connect(sideSlopeEdit_, &QLineEdit::textChanged, this, &GeometryDefinitionWidget::data_changed);
    connect(bedSlopeEdit_, &QLineEdit::textChanged, this, &GeometryDefinitionWidget::data_changed);
}

GeometryDefinitionWidget::~GeometryDefinitionWidget()
{
}

QString GeometryDefinitionWidget::get_channel_type() const
{
    return channelTypeCombo_->currentText();
}

double GeometryDefinitionWidget::get_bottom_width() const
{
    return bottomWidthEdit_->text().toDouble();
}

double GeometryDefinitionWidget::get_side_slope() const
{
    return sideSlopeEdit_->text().toDouble();
}

double GeometryDefinitionWidget::get_bed_slope() const
{
    return bedSlopeEdit_->text().toDouble();
}

bool GeometryDefinitionWidget::is_complete() const
{
    QString channelType = channelTypeCombo_->currentText();

    if(bedSlopeEdit_->text().isEmpty()) return false;

    if(channelType == "Rectangular")
    {
        return !bottomWidthEdit_->text().isEmpty();
    }
    else if(channelType == "Trapezoidal")
    {
        return !bottomWidthEdit_->text().isEmpty() && !sideSlopeEdit_->text().isEmpty();
    }
    else if(channelType == "Triangular")
    {
        return !sideSlopeEdit_->text().isEmpty();
    }

    return false;
}

void GeometryDefinitionWidget::clear_fields()
{
    channelTypeCombo_->setCurrentIndex(0);
    bottomWidthEdit_->clear();
    sideSlopeEdit_->clear();
    bedSlopeEdit_->clear();
}

void GeometryDefinitionWidget::setup_ui()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    QGroupBox* channelTypeGroup = new QGroupBox("Channel Cross-Section");
    QVBoxLayout* typeLayout = new QVBoxLayout();

    channelTypeCombo_ = new QComboBox();
    channelTypeCombo_->addItem("Rectangular");
    channelTypeCombo_->addItem("Trapezoidal");
    channelTypeCombo_->addItem("Triangular");
    channelTypeCombo_->setMinimumWidth(300);

    QListView* comboListView = new QListView(channelTypeCombo_);
    comboListView->setSpacing(0);
    comboListView->setFrameShape(QFrame::NoFrame);
    channelTypeCombo_->setView(comboListView);
    channelTypeCombo_->setItemDelegate(new QStyledItemDelegate(channelTypeCombo_));

    comboListView->setStyleSheet(
        "QListView { "
        "  background-color: #4a4a4a; "
        "  border: none; "
        "  outline: none; "
        "}"
        );

    if(channelTypeCombo_->view()->parentWidget())
    {
        channelTypeCombo_->view()->parentWidget()->setStyleSheet(
            "QWidget { "
            "  background-color: #4a4a4a; "
            "  border: none; "
            "  padding: 0px; "
            "  margin: 0px; "
            "}"
            );
    }

    typeLayout->addWidget(channelTypeCombo_);
    channelTypeGroup->setLayout(typeLayout);
    mainLayout->addWidget(channelTypeGroup);

    QGroupBox* dimensionsGroup = new QGroupBox("Channel Dimensions");
    formLayout_ = new QFormLayout();
    formLayout_->setSpacing(15);
    formLayout_->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    bottomWidthLabel_ = new QLabel("Bottom Width:");
    bottomWidthEdit_ = new QLineEdit();
    bottomWidthEdit_->setPlaceholderText("Enter bottom width");
    bottomWidthEdit_->setMinimumWidth(300);
    formLayout_->addRow(bottomWidthLabel_, bottomWidthEdit_);

    sideSlopeLabel_ = new QLabel("Side Slope (H:V):");
    sideSlopeEdit_ = new QLineEdit();
    sideSlopeEdit_->setPlaceholderText("Enter side slope ratio");
    sideSlopeEdit_->setMinimumWidth(300);
    formLayout_->addRow(sideSlopeLabel_, sideSlopeEdit_);

    bedSlopeEdit_ = new QLineEdit();
    bedSlopeEdit_->setPlaceholderText("Enter bed slope");
    bedSlopeEdit_->setMinimumWidth(300);
    formLayout_->addRow("Bed Slope:", bedSlopeEdit_);

    dimensionsGroup->setLayout(formLayout_);
    mainLayout->addWidget(dimensionsGroup);

    mainLayout->addStretch();

    update_geometry_inputs();
}

void GeometryDefinitionWidget::apply_styling()
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

        // FIXED: Combobox styling
        "QComboBox { "
        "  background-color: #4a4a4a; "
        "  color: #ffffff; "
        "  border: 1px solid #5a5a5a; "
        "  border-radius: 3px; "
        "  padding: 6px 20px 6px 6px; "  // Extra right padding for arrow
        "  font-size: 13px; "
        "}"
        "QComboBox:focus { border: 1px solid #0078d4; }"

        // FIXED: Drop-down button - make completely transparent
        "QComboBox::drop-down { "
        "  background-color: transparent; "  // Changed from #4a4a4a
        "  border: none; "
        "  width: 20px; "
        "  subcontrol-origin: padding; "
        "  subcontrol-position: center right; "
        "}"

        // Arrow styling
        "QComboBox::down-arrow { "
        "  image: none; "
        "  border-left: 4px solid transparent; "
        "  border-right: 4px solid transparent; "
        "  border-top: 6px solid #c0c0c0; "
        "  width: 0; "
        "  height: 0; "
        "  margin-right: 6px; "
        "}"

        // FIXED: Item view styling - remove padding and margin
        "QComboBox QAbstractItemView { "
        "  background-color: #4a4a4a; "
        "  color: #ffffff; "
        "  selection-background-color: #0078d4; "
        "  border: 1px solid #5a5a5a; "
        "  padding: 0px; "     // Remove padding
        "  margin: 0px; "      // Remove margin
        "  outline: none; "    // Remove focus outline
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

void GeometryDefinitionWidget::on_channel_type_changed(int index)
{
    update_geometry_inputs();
    emit data_changed();
}

void GeometryDefinitionWidget::update_geometry_inputs()
{
    QString channelType = channelTypeCombo_->currentText();

    if(channelType == "Rectangular")
    {
        bottomWidthLabel_->setVisible(true);
        bottomWidthEdit_->setVisible(true);
        sideSlopeLabel_->setVisible(false);
        sideSlopeEdit_->setVisible(false);
    }
    else if(channelType == "Trapezoidal")
    {
        bottomWidthLabel_->setVisible(true);
        bottomWidthEdit_->setVisible(true);
        sideSlopeLabel_->setVisible(true);
        sideSlopeEdit_->setVisible(true);
    }
    else if(channelType == "Triangular")
    {
        bottomWidthLabel_->setVisible(false);
        bottomWidthEdit_->setVisible(false);
        sideSlopeLabel_->setVisible(true);
        sideSlopeEdit_->setVisible(true);
    }
}
