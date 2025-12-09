#ifndef INPUTSUMMARYWIDGET_H
#define INPUTSUMMARYWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QMap>
#include "ProjectDataStructures.h"
#include "WorkflowController.h"

class CollapsibleSection;

class InputSummaryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InputSummaryWidget(WorkflowController* controller, QWidget* parent = nullptr);
    ~InputSummaryWidget();

    void update_project_data(const ProjectData& data);
    void update_geometry_data(const GeometryData& data);
    void update_hydraulic_data(const HydraulicData& data);

    bool is_minimized() const;

public slots:
    void toggle_minimized();
    void check_auto_minimize(int windowWidth);

signals:
    void minimized_state_changed(bool minimized);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void setup_ui();
    void setup_minimized_button();
    void setup_expanded_view();
    void apply_styling();

    void save_state();
    void load_state();

    void animate_toggle();
    void show_expanded_view();
    void show_minimized_button();

    void update_project_section(const ProjectData& data);
    void update_geometry_section(const GeometryData& data);
    void update_hydraulic_section(const HydraulicData& data);

    void scroll_to_section(WorkflowStage stage);

    bool has_any_data() const;
    QString format_with_units(double value, const QString& unit) const;

    WorkflowController* controller_;

    bool isMinimized_;

    QPushButton* minimizedButton_;
    QWidget* expandedContainer_;
    QScrollArea* scrollArea_;
    QWidget* scrollContent_;
    QVBoxLayout* mainContentLayout_;

    QLabel* emptyStateLabel_;

    CollapsibleSection* projectSection_;
    CollapsibleSection* geometrySection_;
    CollapsibleSection* hydraulicSection_;

    QPropertyAnimation* toggleAnimation_;

    static constexpr int EXPANDED_WIDTH = 280;
    static constexpr int MINIMIZED_BUTTON_SIZE = 40;
    static constexpr int AUTO_MINIMIZE_THRESHOLD = 800;
};

class CollapsibleSection : public QWidget
{
    Q_OBJECT

public:
    explicit CollapsibleSection(const QString& title, QWidget* parent = nullptr);
    ~CollapsibleSection();

    void set_content_widget(QWidget* widget);
    void set_expanded(bool expanded);
    bool is_expanded() const;

    void clear_content();

signals:
    void expansion_changed(bool expanded);

private slots:
    void toggle_expansion();

private:
    void setup_ui();
    void apply_styling();
    void update_toggle_icon();

    QString title_;
    bool isExpanded_;

    QPushButton* toggleButton_;
    QWidget* contentWidget_;
    QVBoxLayout* mainLayout_;

    QPropertyAnimation* expandAnimation_;
};

#endif // INPUTSUMMARYWIDGET_H
