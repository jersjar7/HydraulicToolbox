#include "InputSummaryWidget.h"
#include "UnitSystemConstants.h"
#include <QSettings>
#include <QFormLayout>
#include <QResizeEvent>
#include <QTimer>
#include <QPainter>

// ============================================================================
// InputSummaryWidget Implementation
// ============================================================================

InputSummaryWidget::InputSummaryWidget(WorkflowController* controller, QWidget* parent)
    : QWidget(parent)
    , controller_{controller}
    , isMinimized_{false}
    , minimizedButton_{nullptr}
    , expandedContainer_{nullptr}
    , scrollArea_{nullptr}
    , scrollContent_{nullptr}
    , mainContentLayout_{nullptr}
    , emptyStateLabel_{nullptr}
    , projectSection_{nullptr}
    , geometrySection_{nullptr}
    , hydraulicSection_{nullptr}
    , toggleAnimation_{nullptr}
{
    setup_ui();
    apply_styling();
    load_state();
}

InputSummaryWidget::~InputSummaryWidget()
{
    save_state();
}

void InputSummaryWidget::setup_ui()
{
    setFixedWidth(EXPANDED_WIDTH);

    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);

    setup_minimized_button();
    setup_expanded_view();

    toggleAnimation_ = new QPropertyAnimation(this, "maximumWidth");
    toggleAnimation_->setDuration(250);
    toggleAnimation_->setEasingCurve(QEasingCurve::InOutQuad);

    show_expanded_view();
}

void InputSummaryWidget::setup_minimized_button()
{
    minimizedButton_ = new QPushButton(this);
    minimizedButton_->setFixedSize(MINIMIZED_BUTTON_SIZE, MINIMIZED_BUTTON_SIZE);
    minimizedButton_->setText("≡");
    minimizedButton_->setToolTip("Show Input Summary");
    minimizedButton_->setCursor(Qt::PointingHandCursor);
    minimizedButton_->hide();

    connect(minimizedButton_, &QPushButton::clicked, this, &InputSummaryWidget::toggle_minimized);
}

void InputSummaryWidget::setup_expanded_view()
{
    expandedContainer_ = new QWidget(this);
    expandedContainer_->setStyleSheet("background-color: transparent;");
    QVBoxLayout* containerLayout = new QVBoxLayout(expandedContainer_);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->setSpacing(0);

    // Header with title and minimize button
    QWidget* header = new QWidget();
    header->setStyleSheet("background-color: transparent;");
    QHBoxLayout* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(10, 10, 10, 10);
    headerLayout->setSpacing(5);

    QLabel* titleLabel = new QLabel("Input Summary");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #ffffff;");
    headerLayout->addWidget(titleLabel);

    headerLayout->addStretch();

    QPushButton* minimizeButton = new QPushButton("−");
    minimizeButton->setFixedSize(24, 24);
    minimizeButton->setToolTip("Minimize");
    minimizeButton->setCursor(Qt::PointingHandCursor);
    minimizeButton->setStyleSheet(
        "QPushButton { "
        "  background-color: #4a4a4a; "
        "  border: 1px solid #5a5a5a; "
        "  border-radius: 3px; "
        "  color: #ffffff; "
        "  font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #5a5a5a; }"
        );
    headerLayout->addWidget(minimizeButton);

    connect(minimizeButton, &QPushButton::clicked, this, &InputSummaryWidget::toggle_minimized);

    containerLayout->addWidget(header);

    // Scroll area for content
    scrollArea_ = new QScrollArea();
    scrollArea_->setWidgetResizable(true);
    scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea_->setFrameShape(QFrame::NoFrame);

    scrollContent_ = new QWidget();
    scrollContent_->setStyleSheet("background-color: transparent;");
    mainContentLayout_ = new QVBoxLayout(scrollContent_);
    mainContentLayout_->setContentsMargins(10, 10, 10, 10);
    mainContentLayout_->setSpacing(15);
    mainContentLayout_->setAlignment(Qt::AlignTop);

    // Empty state label
    emptyStateLabel_ = new QLabel("Enter project parameters to see summary");
    emptyStateLabel_->setWordWrap(true);
    emptyStateLabel_->setAlignment(Qt::AlignCenter);
    emptyStateLabel_->setStyleSheet(
        "QLabel { "
        "  color: #ffffff; "
        "  font-size: 13px; "
        "  font-style: italic; "
        "  padding: 40px 20px; "
        "}"
        );
    mainContentLayout_->addWidget(emptyStateLabel_);

    // Create collapsible sections
    projectSection_ = new CollapsibleSection("PROJECT SETUP");
    geometrySection_ = new CollapsibleSection("GEOMETRY DEFINITION");
    hydraulicSection_ = new CollapsibleSection("HYDRAULIC PARAMETERS");

    projectSection_->hide();
    geometrySection_->hide();
    hydraulicSection_->hide();

    mainContentLayout_->addWidget(projectSection_);
    mainContentLayout_->addWidget(geometrySection_);
    mainContentLayout_->addWidget(hydraulicSection_);

    mainContentLayout_->addStretch();

    scrollArea_->setWidget(scrollContent_);
    containerLayout->addWidget(scrollArea_);

    expandedContainer_->setLayout(containerLayout);
}

void InputSummaryWidget::apply_styling()
{
    setStyleSheet(
        "InputSummaryWidget { "
        "  border-right: 1px solid #5a5a5a; "
        "  border-radius: 5px; "
        "}"
        );

    minimizedButton_->setStyleSheet(
        "QPushButton { "
        "  background-color: rgba(60, 60, 60, 230); "
        "  border: 1px solid #5a5a5a; "
        "  border-radius: 5px; "
        "  color: #ffffff; "
        "  font-size: 18px; "
        "  font-weight: bold; "
        "}"
        "QPushButton:hover { "
        "  background-color: rgba(74, 74, 74, 230); "
        "  border: 1px solid #0078d4; "
        "}"
        );

    scrollArea_->setStyleSheet(
        "QScrollArea { "
        "  background-color: transparent; "
        "  border: none; "
        "}"
        "QScrollBar:vertical { "
        "  background-color: #3c3c3c; "
        "  width: 10px; "
        "  border-radius: 5px; "
        "}"
        "QScrollBar::handle:vertical { "
        "  background-color: #5a5a5a; "
        "  border-radius: 5px; "
        "}"
        "QScrollBar::handle:vertical:hover { "
        "  background-color: #6a6a6a; "
        "}"
        );
}

void InputSummaryWidget::update_project_data(const ProjectData& data)
{
    update_project_section(data);

    if(has_any_data())
    {
        emptyStateLabel_->hide();
        projectSection_->show();
    }
}

void InputSummaryWidget::update_geometry_data(const GeometryData& data)
{
    update_geometry_section(data);

    if(has_any_data())
    {
        emptyStateLabel_->hide();
        geometrySection_->show();
        scroll_to_section(WorkflowStage::GeometryDefinition);
    }
}

void InputSummaryWidget::update_hydraulic_data(const HydraulicData& data)
{
    update_hydraulic_section(data);

    if(has_any_data())
    {
        emptyStateLabel_->hide();
        hydraulicSection_->show();
        scroll_to_section(WorkflowStage::HydraulicParameters);
    }
}

void InputSummaryWidget::update_project_section(const ProjectData& data)
{
    QWidget* content = new QWidget();
    content->setStyleSheet("background-color: transparent;");
    QVBoxLayout* layout = new QVBoxLayout(content);
    layout->setContentsMargins(10, 5, 10, 5);
    layout->setSpacing(8);

    auto add_field = [&](const QString& label, const QString& value) {
        if(!value.isEmpty())
        {
            QHBoxLayout* fieldLayout = new QHBoxLayout();
            fieldLayout->setSpacing(5);

            QLabel* labelWidget = new QLabel(label + ":");
            labelWidget->setStyleSheet("background-color: transparent; color: #ffffff; font-size: 12px;");
            fieldLayout->addWidget(labelWidget);

            fieldLayout->addStretch();

            QLabel* valueWidget = new QLabel(value);
            valueWidget->setStyleSheet("background-color: transparent; color: #ffffff; font-size: 12px; font-weight: 500;");
            valueWidget->setAlignment(Qt::AlignRight);
            fieldLayout->addWidget(valueWidget);

            layout->addLayout(fieldLayout);
        }
    };

    add_field("Project Name", data.projectName);
    add_field("Location", data.location);
    add_field("Unit System", data.useUsCustomary ?
                                 UnitSystemConstants::SYSTEM_NAME_US :
                                 UnitSystemConstants::SYSTEM_NAME_SI);

    projectSection_->set_content_widget(content);
}

void InputSummaryWidget::update_geometry_section(const GeometryData& data)
{
    QWidget* content = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(content);
    layout->setContentsMargins(10, 5, 10, 5);
    layout->setSpacing(8);

    bool useUsCustomary = controller_->get_project_data().useUsCustomary;
    QString lengthUnit = useUsCustomary ?
                             UnitSystemConstants::LABEL_LENGTH_US :
                             UnitSystemConstants::LABEL_LENGTH_SI;

    auto add_field = [&](const QString& label, const QString& value) {
        if(!value.isEmpty())
        {
            QHBoxLayout* fieldLayout = new QHBoxLayout();
            fieldLayout->setSpacing(5);

            QLabel* labelWidget = new QLabel(label + ":");
            labelWidget->setStyleSheet("color: #ffffff; font-size: 12px;");
            fieldLayout->addWidget(labelWidget);

            fieldLayout->addStretch();

            QLabel* valueWidget = new QLabel(value);
            valueWidget->setStyleSheet("color: #ffffff; font-size: 12px; font-weight: 500;");
            valueWidget->setAlignment(Qt::AlignRight);
            fieldLayout->addWidget(valueWidget);

            layout->addLayout(fieldLayout);
        }
    };

    add_field("Channel Type", data.channelType);

    if(data.channelType == "Rectangular" || data.channelType == "Trapezoidal")
    {
        if(data.bottomWidth > 0.0)
            add_field("Bottom Width", format_with_units(data.bottomWidth, lengthUnit));
    }

    if(data.channelType == "Trapezoidal" || data.channelType == "Triangular")
    {
        if(data.sideSlope > 0.0)
            add_field("Side Slope (H:V)", QString::number(data.sideSlope, 'g'));
    }

    if(data.bedSlope > 0.0)
        add_field("Bed Slope", QString::number(data.bedSlope, 'g'));

    geometrySection_->set_content_widget(content);
}

void InputSummaryWidget::update_hydraulic_section(const HydraulicData& data)
{
    QWidget* content = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(content);
    layout->setContentsMargins(10, 5, 10, 5);
    layout->setSpacing(8);

    bool useUsCustomary = controller_->get_project_data().useUsCustomary;
    QString dischargeUnit = useUsCustomary ?
                                UnitSystemConstants::LABEL_DISCHARGE_US :
                                UnitSystemConstants::LABEL_DISCHARGE_SI;

    auto add_field = [&](const QString& label, const QString& value) {
        if(!value.isEmpty())
        {
            QHBoxLayout* fieldLayout = new QHBoxLayout();
            fieldLayout->setSpacing(5);

            QLabel* labelWidget = new QLabel(label + ":");
            labelWidget->setStyleSheet("color: #ffffff; font-size: 12px;");
            fieldLayout->addWidget(labelWidget);

            fieldLayout->addStretch();

            QLabel* valueWidget = new QLabel(value);
            valueWidget->setStyleSheet("color: #ffffff; font-size: 12px; font-weight: 500;");
            valueWidget->setAlignment(Qt::AlignRight);
            fieldLayout->addWidget(valueWidget);

            layout->addLayout(fieldLayout);
        }
    };

    if(data.discharge > 0.0)
        add_field("Discharge", format_with_units(data.discharge, dischargeUnit));

    if(data.manningN > 0.0)
        add_field("Manning's n", QString::number(data.manningN, 'g'));

    hydraulicSection_->set_content_widget(content);
}

QString InputSummaryWidget::format_with_units(double value, const QString& unit) const
{
    QString numStr = QString::number(value, 'f', 6);  // More precision

    // Remove trailing zeros
    while(numStr.contains('.') && (numStr.endsWith('0') || numStr.endsWith('.')))
    {
        numStr.chop(1);
        if(numStr.endsWith('.'))
        {
            numStr.chop(1);
            break;
        }
    }

    return numStr + " " + unit;
}

bool InputSummaryWidget::has_any_data() const
{
    const ProjectData& projectData = controller_->get_project_data();
    const GeometryData& geometryData = controller_->get_geometry_data();
    const HydraulicData& hydraulicData = controller_->get_hydraulic_data();

    return !projectData.projectName.isEmpty() ||
           !geometryData.channelType.isEmpty() ||
           hydraulicData.discharge > 0.0;
}

void InputSummaryWidget::scroll_to_section(WorkflowStage stage)
{
    QWidget* targetSection = nullptr;

    switch(stage)
    {
    case WorkflowStage::ProjectSetup:
        targetSection = projectSection_;
        break;
    case WorkflowStage::GeometryDefinition:
        targetSection = geometrySection_;
        break;
    case WorkflowStage::HydraulicParameters:
        targetSection = hydraulicSection_;
        break;
    default:
        return;
    }

    if(targetSection && targetSection->isVisible())
    {
        scrollArea_->ensureWidgetVisible(targetSection, 0, 0);
    }
}

bool InputSummaryWidget::is_minimized() const
{
    return isMinimized_;
}

void InputSummaryWidget::toggle_minimized()
{
    isMinimized_ = !isMinimized_;
    animate_toggle();
    save_state();
    emit minimized_state_changed(isMinimized_);
}

void InputSummaryWidget::animate_toggle()
{
    if(isMinimized_)
    {
        toggleAnimation_->setStartValue(EXPANDED_WIDTH);
        toggleAnimation_->setEndValue(0);
        toggleAnimation_->start();

        QTimer::singleShot(250, this, [this]() {
            show_minimized_button();
        });
    }
    else
    {
        minimizedButton_->hide();

        toggleAnimation_->setStartValue(0);
        toggleAnimation_->setEndValue(EXPANDED_WIDTH);
        toggleAnimation_->start();

        QTimer::singleShot(100, this, [this]() {
            show_expanded_view();
        });
    }
}

void InputSummaryWidget::show_expanded_view()
{
    expandedContainer_->show();
    setFixedWidth(EXPANDED_WIDTH);
}

void InputSummaryWidget::show_minimized_button()
{
    expandedContainer_->hide();
    setFixedWidth(MINIMIZED_BUTTON_SIZE + 20);
    minimizedButton_->move(10, height() - MINIMIZED_BUTTON_SIZE - 10);
    minimizedButton_->show();
    minimizedButton_->raise();
}

void InputSummaryWidget::check_auto_minimize(int windowWidth)
{
    if(windowWidth < AUTO_MINIMIZE_THRESHOLD && !isMinimized_)
    {
        toggle_minimized();
    }
}

void InputSummaryWidget::paintEvent(QPaintEvent* event)
{
    // Only paint background when expanded
    if(!isMinimized_)
    {
        QPainter painter(this);

        // Create horizontal gradient from left to right
        QLinearGradient gradient(0, 0, width(), 0);

        // Left side: semi-transparent dark grey (60% opacity)
        QColor leftColor(60, 60, 60, 153);
        gradient.setColorAt(0.0, leftColor);

        // Right side: same grey but fully transparent
        QColor rightColor(60, 60, 60, 0);
        gradient.setColorAt(1.0, rightColor);

        // Paint the gradient
        painter.fillRect(rect(), gradient);
    }
}

void InputSummaryWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    if(expandedContainer_)
    {
        expandedContainer_->setGeometry(0, 0, width(), height());
    }

    if(isMinimized_ && minimizedButton_)
    {
        minimizedButton_->move(10, height() - MINIMIZED_BUTTON_SIZE - 10);
    }
}

void InputSummaryWidget::save_state()
{
    QSettings settings("HydraulicToolbox", "InputSummary");
    settings.setValue("minimized", isMinimized_);
    settings.setValue("projectExpanded", projectSection_->is_expanded());
    settings.setValue("geometryExpanded", geometrySection_->is_expanded());
    settings.setValue("hydraulicExpanded", hydraulicSection_->is_expanded());
}

void InputSummaryWidget::load_state()
{
    QSettings settings("HydraulicToolbox", "InputSummary");

    bool minimized = settings.value("minimized", false).toBool();
    if(minimized)
    {
        isMinimized_ = true;
        show_minimized_button();
    }

    projectSection_->set_expanded(settings.value("projectExpanded", true).toBool());
    geometrySection_->set_expanded(settings.value("geometryExpanded", true).toBool());
    hydraulicSection_->set_expanded(settings.value("hydraulicExpanded", true).toBool());
}

// ============================================================================
// CollapsibleSection Implementation
// ============================================================================

CollapsibleSection::CollapsibleSection(const QString& title, QWidget* parent)
    : QWidget(parent)
    , title_{title}
    , isExpanded_{true}
    , toggleButton_{nullptr}
    , contentWidget_{nullptr}
    , mainLayout_{nullptr}
    , expandAnimation_{nullptr}
{
    setup_ui();
    apply_styling();
}

CollapsibleSection::~CollapsibleSection()
{
}

void CollapsibleSection::setup_ui()
{
    mainLayout_ = new QVBoxLayout(this);

    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAutoFillBackground(false);

    mainLayout_->setContentsMargins(0, 0, 0, 0);
    mainLayout_->setSpacing(0);

    toggleButton_ = new QPushButton();
    toggleButton_->setText("▼ " + title_);
    toggleButton_->setFlat(true);
    toggleButton_->setCursor(Qt::PointingHandCursor);
    toggleButton_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    toggleButton_->setFixedHeight(32);

    connect(toggleButton_, &QPushButton::clicked, this, &CollapsibleSection::toggle_expansion);

    mainLayout_->addWidget(toggleButton_);

    expandAnimation_ = new QPropertyAnimation(this, "maximumHeight");
    expandAnimation_->setDuration(200);
    expandAnimation_->setEasingCurve(QEasingCurve::InOutQuad);
}

void CollapsibleSection::apply_styling()
{
    setStyleSheet(
        "CollapsibleSection { "
        "  background-color: transparent; "
        "  border: 1px solid #5a5a5a; "
        "  border-radius: 3px; "
        "}"
        );

    toggleButton_->setStyleSheet(
        "QPushButton { "
        "  background-color: rgba(74, 74, 74, 153); "
        "  color: #ffffff; "
        "  font-size: 12px; "
        "  font-weight: bold; "
        "  text-align: left; "
        "  padding-left: 8px; "
        "  border: none; "
        "  border-bottom: 1px solid #5a5a5a; "
        "  border-radius: 3px; "
        "}"
        "QPushButton:hover { "
        "  background-color: rgba(90, 90, 90, 153); "
        "}"
        );
}

void CollapsibleSection::set_content_widget(QWidget* widget)
{
    if(contentWidget_)
    {
        mainLayout_->removeWidget(contentWidget_);
        contentWidget_->deleteLater();
    }

    contentWidget_ = widget;

    if(contentWidget_)
    {
        contentWidget_->setStyleSheet("background-color: transparent;");
        mainLayout_->addWidget(contentWidget_);
        contentWidget_->setVisible(isExpanded_);
    }
}

void CollapsibleSection::clear_content()
{
    if(contentWidget_)
    {
        mainLayout_->removeWidget(contentWidget_);
        contentWidget_->deleteLater();
        contentWidget_ = nullptr;
    }
}

void CollapsibleSection::set_expanded(bool expanded)
{
    if(isExpanded_ != expanded)
    {
        isExpanded_ = expanded;
        update_toggle_icon();

        if(contentWidget_)
        {
            contentWidget_->setVisible(isExpanded_);
        }

        emit expansion_changed(isExpanded_);
    }
}

bool CollapsibleSection::is_expanded() const
{
    return isExpanded_;
}

void CollapsibleSection::toggle_expansion()
{
    set_expanded(!isExpanded_);
}

void CollapsibleSection::update_toggle_icon()
{
    QString icon = isExpanded_ ? "▼" : "▶";
    toggleButton_->setText(icon + " " + title_);
}
