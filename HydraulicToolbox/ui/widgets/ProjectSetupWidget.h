#ifndef PROJECTSETUPWIDGET_H
#define PROJECTSETUPWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QRadioButton>
#include <QButtonGroup>
#include <QString>

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

signals:
    void data_changed();

private:
    void setup_ui();
    void apply_styling();

    QLineEdit* projectNameEdit_;
    QLineEdit* locationEdit_;
    QRadioButton* usCustomaryRadio_;
    QRadioButton* siMetricRadio_;
    QButtonGroup* unitSystemGroup_;
};

#endif // PROJECTSETUPWIDGET_H
