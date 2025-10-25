#include "AnalysisResultsWidget.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFormLayout>

AnalysisResultsWidget::AnalysisResultsWidget(QWidget* parent)
    : QWidget(parent)
    , placeholderLabel_{nullptr}
    , normalDepthLabel_{nullptr}
    , velocityLabel_{nullptr}
    , froudeNumberLabel_{nullptr}
    , flowRegimeLabel_{nullptr}
{
    setup_ui();
    apply_styling();
}

AnalysisResultsWidget::~AnalysisResultsWidget()
{
}

bool AnalysisResultsWidget::is_complete() const
{
    return true;
}

void AnalysisResultsWidget::setup_ui()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    placeholderLabel_ = new QLabel("Calculation results will appear here after completing all input stages.");
    placeholderLabel_->setWordWrap(true);
    placeholderLabel_->setAlignment(Qt::AlignCenter);
    placeholderLabel_->setStyleSheet(
        "QLabel { "
        "  color: #999999; "
        "  font-size: 14px; "
        "  font-style: italic; "
        "  padding: 40px; "
        "}"
        );
    mainLayout->addWidget(placeholderLabel_);

    QGroupBox* resultsGroup = new QGroupBox("Analysis Results");
    QFormLayout* formLayout = new QFormLayout();
    formLayout->setSpacing(15);
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    normalDepthLabel_ = new QLabel("--");
    normalDepthLabel_->setMinimumWidth(300);
    formLayout->addRow("Normal Depth:", normalDepthLabel_);

    velocityLabel_ = new QLabel("--");
    velocityLabel_->setMinimumWidth(300);
    formLayout->addRow("Average Velocity:", velocityLabel_);

    froudeNumberLabel_ = new QLabel("--");
    froudeNumberLabel_->setMinimumWidth(300);
    formLayout->addRow("Froude Number:", froudeNumberLabel_);

    flowRegimeLabel_ = new QLabel("--");
    flowRegimeLabel_->setMinimumWidth(300);
    formLayout->addRow("Flow Regime:", flowRegimeLabel_);

    resultsGroup->setLayout(formLayout);
    mainLayout->addWidget(resultsGroup);

    mainLayout->addStretch();
}

void AnalysisResultsWidget::apply_styling()
{
    setStyleSheet(
        "QLabel { color: #c0c0c0; font-size: 13px; }"
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
