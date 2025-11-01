#ifndef ANALYZER_H
#define ANALYZER_H

class Channel;
class Flow;

enum class FlowRegime
{
    Subcritical,
    Critical,
    Supercritical
};

struct AnalysisResult
{
    double normalDepth{0.0};
    double velocity{0.0};
    double froudeNumber{0.0};
    FlowRegime flowRegime{FlowRegime::Subcritical};
    bool isValid{false};
};

class Analyzer
{
public:
    Analyzer() = default;

    AnalysisResult solve_for_depth(Channel& channel, const Flow& flow, double slope, double manningsCoefficient, double gravity) const;

private:
};

#endif // ANALYZER_H
