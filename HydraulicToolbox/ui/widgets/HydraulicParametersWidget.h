#ifndef HYDRAULICPARAMETERSWIDGET_H
#define HYDRAULICPARAMETERSWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>

class HydraulicParametersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HydraulicParametersWidget(QWidget* parent = nullptr);
    ~HydraulicParametersWidget();

    double get_discharge() const;
    double get_mannings_n() const;
    bool is_complete() const;

signals:
    void data_changed();

private:
    void setup_ui();
    void apply_styling();
    void populate_mannings_materials();
    void on_material_selected(int index);

    QLineEdit* dischargeEdit_;
    QComboBox* manningsMaterialCombo_;
    QLineEdit* manningsNEdit_;
    QFormLayout* formLayout_;
};

#endif // HYDRAULICPARAMETERSWIDGET_H
