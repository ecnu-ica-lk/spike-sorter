/*
------------------------------------------------------------------

This file is part of the Open Ephys GUI
Copyright (C) 2013 Open Ephys

------------------------------------------------------------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef __WAVEFORMSTATS_H
#define __WAVEFORMSTATS_H

#include <ProcessorHeaders.h>

#include "Containers.h"

#include <algorithm>    // std::sort
#include <list>
#include <queue>
#include <atomic>


class WaveformStats
{
public:
    WaveformStats();
    ~WaveformStats();
    void resizeWaveform(int newlength);
    void reset();
    std::vector<double> getMean(int index);
    std::vector<double> getStandardDeviation(int index);
    void update(SorterSpikePtr so);
    bool queryNewData();

    double LastSpikeTime;
    bool newData;
    std::vector<std::vector<double> > WaveFormMean,WaveFormSk,WaveFormMk;
    double numSamples;
};


#endif // __WAVEFORMSTATS_H
