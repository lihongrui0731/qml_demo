#ifndef SPECTRA_PANEL_H
#define SPECTRA_PANEL_H

#include <QWidget>

#include "TimeSpectraChart.h"
#include "FrequencyLegendBar.h"
#include "InstantSpectrumChart.h"

class SpectraPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SpectraPanel(QWidget *parent = nullptr);

    TimeSpectraChart* timeSpectraChart() const
    {
        return timeChart_;
    }

    FrequencyLegendBar* legendBar() const
    {
        return legendBar_;
    }

    InstantSpectrumChart* instantSpectrumChart() const
    {
        return instantChart_;
    }

private:

    TimeSpectraChart* timeChart_{};
    FrequencyLegendBar* legendBar_{};
    InstantSpectrumChart* instantChart_{};

    void setupUI();

};

#endif // SPECTRA_PANEL_H
