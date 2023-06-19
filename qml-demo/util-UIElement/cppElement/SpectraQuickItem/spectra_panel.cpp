#include "spectra_panel.h"

#include <QDebug>

SpectraPanel::SpectraPanel(QWidget *parent)
    : QWidget{parent}
{
    setupUI();
}

void SpectraPanel::setupUI()
{
    timeChart_ = new TimeSpectraChart(this);
    legendBar_ = new FrequencyLegendBar(this);
    instantChart_ = new InstantSpectrumChart(this);

    QHBoxLayout* mainLayout { new QHBoxLayout(this) };
    mainLayout->setContentsMargins(QMargins());
    mainLayout->setSpacing(0);

    mainLayout->addWidget(timeChart_, 3);
    mainLayout->addWidget(legendBar_, 0);
    mainLayout->addWidget(instantChart_, 1);
}
