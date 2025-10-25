#ifndef ANALYSISRESULTSWIDGET_H
#define ANALYSISRESULTSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "../backend/HydraulicCalculator.h"

class AnalysisResultsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnalysisResultsWidget(QWidget* parent = nullptr);
    ~AnalysisResultsWidget();

    bool is_complete() const;

    void update_results(const CalculationResults& results, bool useUsCustomary);

signals:
    void data_changed();

private:
    void setup_ui();
    void apply_styling();

    QLabel* placeholderLabel_;
    QLabel* normalDepthLabel_;
    QLabel* velocityLabel_;
    QLabel* froudeNumberLabel_;
    QLabel* flowRegimeLabel_;
    QLabel* errorLabel_;
};

#endif // ANALYSISRESULTSWIDGET_H
