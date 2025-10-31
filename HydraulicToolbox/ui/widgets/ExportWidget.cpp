#include "ExportWidget.h"
#include <QVBoxLayout>
#include <QGroupBox>

ExportWidget::ExportWidget(QWidget* parent)
    : QWidget(parent)
    , saveProjectButton_{nullptr}
    , exportDataButton_{nullptr}
    , captureScreenshotButton_{nullptr}
    , generateReportButton_{nullptr}
    , placeholderLabel_{nullptr}
{
    setup_ui();
    apply_styling();

    connect(saveProjectButton_, &QPushButton::clicked, this, &ExportWidget::data_changed);
    connect(exportDataButton_, &QPushButton::clicked, this, &ExportWidget::data_changed);
    connect(captureScreenshotButton_, &QPushButton::clicked, this, &ExportWidget::data_changed);
    connect(generateReportButton_, &QPushButton::clicked, this, &ExportWidget::data_changed);
}

ExportWidget::~ExportWidget()
{
}

bool ExportWidget::is_complete() const
{
    return true;
}

void ExportWidget::setup_ui()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    placeholderLabel_ = new QLabel("Export and save your analysis results.");
    placeholderLabel_->setWordWrap(true);
    placeholderLabel_->setAlignment(Qt::AlignCenter);
    placeholderLabel_->setStyleSheet(
        "QLabel { "
        "  color: #999999; "
        "  font-size: 14px; "
        "  font-style: italic; "
        "  padding: 20px; "
        "}"
        );
    mainLayout->addWidget(placeholderLabel_);

    QGroupBox* exportGroup = new QGroupBox("Export Options");
    QVBoxLayout* exportLayout = new QVBoxLayout();
    exportLayout->setSpacing(15);

    saveProjectButton_ = new QPushButton("Save Project File");
    saveProjectButton_->setMinimumHeight(40);
    saveProjectButton_->setCursor(Qt::PointingHandCursor);
    exportLayout->addWidget(saveProjectButton_);

    exportDataButton_ = new QPushButton("Export Data to CSV");
    exportDataButton_->setMinimumHeight(40);
    exportDataButton_->setCursor(Qt::PointingHandCursor);
    exportLayout->addWidget(exportDataButton_);

    captureScreenshotButton_ = new QPushButton("Capture Visualization Screenshot");
    captureScreenshotButton_->setMinimumHeight(40);
    captureScreenshotButton_->setCursor(Qt::PointingHandCursor);
    exportLayout->addWidget(captureScreenshotButton_);

    generateReportButton_ = new QPushButton("Generate Analysis Report");
    generateReportButton_->setMinimumHeight(40);
    generateReportButton_->setCursor(Qt::PointingHandCursor);
    exportLayout->addWidget(generateReportButton_);

    exportGroup->setLayout(exportLayout);
    mainLayout->addWidget(exportGroup);

    mainLayout->addStretch();
}

void ExportWidget::apply_styling()
{
    setStyleSheet(
        "QLabel { color: #c0c0c0; font-size: 13px; }"
        "QPushButton { "
        "  background-color: #4a4a4a; "
        "  color: #ffffff; "
        "  border: 1px solid #5a5a5a; "
        "  border-radius: 3px; "
        "  padding: 10px; "
        "  font-size: 13px; "
        "  text-align: left; "
        "}"
        "QPushButton:hover { "
        "  background-color: #5a5a5a; "
        "  border: 1px solid #0078d4; "
        "}"
        "QPushButton:pressed { "
        "  background-color: #3a3a3a; "
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
