#include "VisualizationPanel.h"

VisualizationPanel::VisualizationPanel(QWidget* parent)
    : QWidget(parent)
    , vtkWidget_{nullptr}
    , viewControlsContainer_{nullptr}
    , viewTopButton_{nullptr}
    , viewFrontButton_{nullptr}
    , viewRightButton_{nullptr}
    , viewIsoButton_{nullptr}
    , viewResetButton_{nullptr}
{
    setup_ui();
    setup_view_controls();
    apply_styling();
}

VisualizationPanel::~VisualizationPanel()
{
}

void VisualizationPanel::setup_ui()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    vtkWidget_ = new VtkWidget(this);
    mainLayout->addWidget(vtkWidget_);

    setMinimumHeight(400);
}

void VisualizationPanel::setup_view_controls()
{
    viewControlsContainer_ = new QWidget(this);

    QHBoxLayout* controlsLayout = new QHBoxLayout(viewControlsContainer_);
    controlsLayout->setContentsMargins(6, 6, 6, 6);
    controlsLayout->setSpacing(4);

    viewIsoButton_ = new QPushButton("Isometric", viewControlsContainer_);
    viewIsoButton_->setToolTip("Isometric view");
    controlsLayout->addWidget(viewIsoButton_);

    viewTopButton_ = new QPushButton("Top", viewControlsContainer_);
    viewTopButton_->setToolTip("View from top");
    controlsLayout->addWidget(viewTopButton_);

    viewFrontButton_ = new QPushButton("Front", viewControlsContainer_);
    viewFrontButton_->setToolTip("View from front");
    controlsLayout->addWidget(viewFrontButton_);

    viewRightButton_ = new QPushButton("Right", viewControlsContainer_);
    viewRightButton_->setToolTip("View from right");
    controlsLayout->addWidget(viewRightButton_);

    viewResetButton_ = new QPushButton("Reset", viewControlsContainer_);
    viewResetButton_->setToolTip("Reset to default view");
    controlsLayout->addWidget(viewResetButton_);

    viewControlsContainer_->setFixedSize(310, 35);
    viewControlsContainer_->raise();
    viewControlsContainer_->show();

    connect(viewIsoButton_, &QPushButton::clicked, vtkWidget_, &VtkWidget::set_view_isometric);
    connect(viewTopButton_, &QPushButton::clicked, vtkWidget_, &VtkWidget::set_view_top);
    connect(viewFrontButton_, &QPushButton::clicked, vtkWidget_, &VtkWidget::set_view_front);
    connect(viewRightButton_, &QPushButton::clicked, vtkWidget_, &VtkWidget::set_view_right);
    connect(viewResetButton_, &QPushButton::clicked, vtkWidget_, &VtkWidget::reset_view);
}

void VisualizationPanel::apply_styling()
{
    viewControlsContainer_->setStyleSheet(
        "QWidget { "
        "  background-color: rgba(60, 60, 60, 200); "
        "  border-radius: 5px; "
        "}"
        );

    QString buttonStyle =
        "QPushButton { "
        "  background-color: #4a4a4a; "
        "  color: #ffffff; "
        "  border: 1px solid #5a5a5a; "
        "  border-radius: 3px; "
        "  padding: 5px 10px; "
        "  font-size: 10px; "
        "}"
        "QPushButton:hover { "
        "  background-color: #5a5a5a; "
        "  border: 1px solid #0078d4; "
        "}"
        "QPushButton:pressed { "
        "  background-color: #3a3a3a; "
        "}";

    viewIsoButton_->setStyleSheet(buttonStyle);
    viewTopButton_->setStyleSheet(buttonStyle);
    viewFrontButton_->setStyleSheet(buttonStyle);
    viewRightButton_->setStyleSheet(buttonStyle);
    viewResetButton_->setStyleSheet(buttonStyle);
}

void VisualizationPanel::position_view_controls()
{
    if(viewControlsContainer_)
    {
        int x = width() - viewControlsContainer_->width() - 20;
        int y = 10;

        viewControlsContainer_->move(x, y);
        viewControlsContainer_->raise();
    }
}

void VisualizationPanel::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    position_view_controls();
}

void VisualizationPanel::render_channel(const GeometryData& geometry, const CalculationResults& results)
{
    if(vtkWidget_)
    {
        vtkWidget_->render_channel(geometry, results);
    }
}

VtkWidget* VisualizationPanel::get_vtk_widget()
{
    return vtkWidget_;
}
