#ifndef WORKFLOWTABBAR_H
#define WORKFLOWTABBAR_H

#include <QWidget>
#include <QPushButton>
#include <array>
#include "WorkflowController.h"

class WorkflowTabBar : public QWidget
{
    Q_OBJECT

public:
    explicit WorkflowTabBar(WorkflowController* controller, QWidget* parent = nullptr);
    ~WorkflowTabBar();

    void update_tab_states();

signals:
    void tab_clicked(WorkflowStage stage);

private slots:
    void on_tab_button_clicked();
    void on_stage_changed(WorkflowStage newStage);
    void on_completion_changed(WorkflowStage stage, bool complete);

private:
    void setup_ui();
    void create_tab_button(WorkflowStage stage, const QString& label);
    void update_button_style(QPushButton* button, WorkflowStage stage);
    QString get_button_stylesheet(bool isActive, bool isComplete, bool isLocked) const;

    WorkflowController* controller_;
    std::array<QPushButton*, 5> tabButtons_;
};

#endif // WORKFLOWTABBAR_H
