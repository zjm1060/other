#ifndef QPERFSTAT_H
#define QPERFSTAT_H

#include <QMap>

#include "qstatperfprocessor.h"

#ifdef Q_OS_AIX
void getStatPerfProcessor(PerfProcessor& processor, QMap<int, QStatPerfProcessor::CpuUsage>& cpus);

void getStatSystemMemory(SystemMemory& memory, quint64& peak);
#endif

#endif // QPERFSTAT_H
