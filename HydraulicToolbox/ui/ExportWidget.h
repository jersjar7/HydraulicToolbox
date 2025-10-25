#ifndef EXPORTWIDGET_H
#define EXPORTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class ExportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExportWidget(QWidget* parent = nullptr);
    ~ExportWidget();

    bool is_complete() const;

signals:
    void data_changed();

private:
    void setup_ui();
    void apply_styling();

    QPushButton* saveProjectButton_;
    QPushButton* exportDataButton_;
    QPushButton* captureScreenshotButton_;
    QPushButton* generateReportButton_;
    QLabel* placeholderLabel_;
};

#endif // EXPORTWIDGET_H
