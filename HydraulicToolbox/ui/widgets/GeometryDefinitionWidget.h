#ifndef GEOMETRYDEFINITIONWIDGET_H
#define GEOMETRYDEFINITIONWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>
#include <QString>

class GeometryDefinitionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GeometryDefinitionWidget(QWidget* parent = nullptr);
    ~GeometryDefinitionWidget();

    QString get_channel_type() const;
    double get_bottom_width() const;
    double get_side_slope() const;
    double get_bed_slope() const;
    bool is_complete() const;

    void clear_fields();

signals:
    void data_changed();

private slots:
    void on_channel_type_changed(int index);

private:
    void setup_ui();
    void apply_styling();
    void update_geometry_inputs();

    QComboBox* channelTypeCombo_;
    QLineEdit* bottomWidthEdit_;
    QLineEdit* sideSlopeEdit_;
    QLineEdit* bedSlopeEdit_;
    QLabel* bottomWidthLabel_;
    QLabel* sideSlopeLabel_;
    QFormLayout* formLayout_;
};

#endif // GEOMETRYDEFINITIONWIDGET_H
