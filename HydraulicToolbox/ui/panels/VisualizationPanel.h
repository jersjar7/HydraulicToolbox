#ifndef VISUALIZATIONPANEL_H
#define VISUALIZATIONPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
#include "../visualization/VtkWidget.h"
#include "InputSummaryWidget.h"
#include "ProjectDataStructures.h"
#include "WorkflowController.h"

class VisualizationPanel : public QWidget
{
    Q_OBJECT

public:
    explicit VisualizationPanel(WorkflowController* controller, QWidget* parent = nullptr);
    ~VisualizationPanel();

    void render_channel(const GeometryData& geometry, const CalculationResults& results);
    VtkWidget* get_vtk_widget();
    InputSummaryWidget* get_input_summary_widget();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void setup_ui();
    void setup_view_controls();
    void apply_styling();
    void position_view_controls();
    void position_input_summary();
    void on_input_summary_minimized(bool minimized);

    WorkflowController* controller_;

    VtkWidget* vtkWidget_;
    InputSummaryWidget* inputSummaryWidget_;
    QWidget* viewControlsContainer_;

    QPushButton* viewTopButton_;
    QPushButton* viewFrontButton_;
    QPushButton* viewRightButton_;
    QPushButton* viewIsoButton_;
    QPushButton* viewResetButton_;
};

#endif // VISUALIZATIONPANEL_H
