#ifndef PROJECTSETUPWIDGET_H
#define PROJECTSETUPWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QRadioButton>
#include <QButtonGroup>
#include <QString>

class WorkflowController;

class ProjectSetupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectSetupWidget(QWidget* parent = nullptr);
    ~ProjectSetupWidget();

    QString get_project_name() const;
    QString get_location() const;
    bool is_us_customary() const;
    bool is_complete() const;

    void set_workflow_controller(WorkflowController* controller);

signals:
    void data_changed();
    void unit_system_changed_with_data_clear();

private slots:
    void on_unit_system_changed();

private:
    void setup_ui();
    void apply_styling();
    bool has_downstream_data() const;

    QLineEdit* projectNameEdit_;
    QLineEdit* locationEdit_;
    QRadioButton* usCustomaryRadio_;
    QRadioButton* siMetricRadio_;
    QButtonGroup* unitSystemGroup_;

    WorkflowController* workflowController_;
    bool suppressUnitChangeWarning_{false};
};

#endif // PROJECTSETUPWIDGET_H
