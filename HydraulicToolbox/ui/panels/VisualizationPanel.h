#ifndef VISUALIZATIONPANEL_H
#define VISUALIZATIONPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
#include "VtkWidget.h"
#include "ProjectDataStructures.h"

class VisualizationPanel : public QWidget
{
    Q_OBJECT

public:
    explicit VisualizationPanel(QWidget* parent = nullptr);
    ~VisualizationPanel();

    void render_channel(const GeometryData& geometry, const CalculationResults& results);
    VtkWidget* get_vtk_widget();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void setup_ui();
    void setup_view_controls();
    void apply_styling();
    void position_view_controls();

    VtkWidget* vtkWidget_;
    QWidget* viewControlsContainer_;

    QPushButton* viewTopButton_;
    QPushButton* viewFrontButton_;
    QPushButton* viewRightButton_;
    QPushButton* viewIsoButton_;
    QPushButton* viewResetButton_;
};

#endif // VISUALIZATIONPANEL_H
