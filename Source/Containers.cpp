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

#include "Containers.h"

PointD::PointD()
{
    X = Y = 0;
}

PointD::PointD(float x, float y)
{
    X = x;
    Y = y;
}

PointD::PointD(const PointD& P)
{
    X = P.X;
    Y = P.Y;
}

PointD& PointD::operator+=(const PointD& rhs)
{
    X += rhs.X;
    Y += rhs.Y;
    return *this;
}

PointD& PointD::operator-=(const PointD& rhs)
{
    X -= rhs.X;
    Y -= rhs.Y;
    return *this;
}

const PointD PointD::operator+(const PointD& other) const
{
    PointD result = *this;
    result += other;
    return result;
}

const PointD PointD::operator-(const PointD& other) const
{
    PointD result = *this;
    result -= other;
    return result;
}

const PointD PointD::operator*(const PointD& other) const
{
    PointD result = *this;
    result.X *= other.X;
    result.Y *= other.Y;
    return result;

}

float PointD::cross(PointD c) const
{
    return X * c.Y - Y * c.X;
}


SorterSpikeContainer::SorterSpikeContainer(const SpikeChannel* channel, uint16 sortedId_, int64 timestamp_, const float* waveform)
    : chan(channel),
      sortedId(sortedId_),
      timestamp(timestamp_)
{
    color[0] = color[1] = color[2] = 127;
    pcProj[0] = pcProj[1] = pcProj[2] = 0;

    int nSamples = chan->getNumChannels() * chan->getTotalSamples();

    data.malloc(nSamples);
    memcpy(data.getData(), waveform, nSamples*sizeof(float));

}

const float* SorterSpikeContainer::getData() const
{
    return data.getData();
}

const SpikeChannel* SorterSpikeContainer::getChannel() const
{
    return chan;
}

int64 SorterSpikeContainer::getTimestamp() const
{
    return timestamp;
}

float SorterSpikeContainer::getMinimum(int channelIndex)
{
    int offset = channelIndex * chan->getTotalSamples() + chan->getPrePeakSamples() + 1;

    return data[offset];
}

float SorterSpikeContainer::getMaximum(int channelIndex)
{
    int offset = channelIndex * chan->getTotalSamples();

    float maximum = -99999.9f;

    for (int i = offset; i < offset + chan->getTotalSamples(); i++)
    {
        if (data[i] > maximum)
            maximum = data[i];
    }

    return maximum;
}

bool SorterSpikeContainer::checkThresholds(Array<float> thresholds)
{

    bool belowThresh = true;

    for (int i = 0; i < thresholds.size(); i++)
    {
        belowThresh &= getMinimum(i) < thresholds[i];
    }

    return belowThresh;
}