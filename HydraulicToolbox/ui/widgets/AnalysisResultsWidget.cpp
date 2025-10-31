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
    , errorLabel_{nullptr}
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

    errorLabel_ = new QLabel();
    errorLabel_->setWordWrap(true);
    errorLabel_->setAlignment(Qt::AlignCenter);
    errorLabel_->setStyleSheet(
        "QLabel { "
        "  color: #ff6b6b; "
        "  font-size: 13px; "
        "  font-weight: bold; "
        "  padding: 10px; "
        "  background-color: #4a2020; "
        "  border: 1px solid #ff6b6b; "
        "  border-radius: 3px; "
        "}"
        );
    errorLabel_->setVisible(false);
    mainLayout->addWidget(errorLabel_);

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

void AnalysisResultsWidget::update_results(const CalculationResults& results, bool useUsCustomary)
{
    if(!results.isValid)
    {
        errorLabel_->setText(results.errorMessage);
        errorLabel_->setVisible(true);
        placeholderLabel_->setVisible(true);

        normalDepthLabel_->setText("--");
        velocityLabel_->setText("--");
        froudeNumberLabel_->setText("--");
        flowRegimeLabel_->setText("--");
        return;
    }

    errorLabel_->setVisible(false);
    placeholderLabel_->setVisible(false);

    QString depthUnit = useUsCustomary ? "ft" : "m";
    QString velocityUnit = useUsCustomary ? "ft/s" : "m/s";

    normalDepthLabel_->setText(QString::number(results.normalDepth, 'f', 3) + " " + depthUnit);
    velocityLabel_->setText(QString::number(results.velocity, 'f', 3) + " " + velocityUnit);
    froudeNumberLabel_->setText(QString::number(results.froudeNumber, 'f', 3));
    flowRegimeLabel_->setText(results.flowRegime);
}
