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

SorterSpikeContainer::SorterSpikeContainer(const SpikeChannel* channel, SpikePtr spike)
{
    color[0] = color[1] = color[2] = 127;
    pcProj[0] = pcProj[1] = 0;
    sortedId = 0;

    chan = channel;
    int nSamples = chan->getNumChannels() * chan->getTotalSamples();

    //data.malloc(nSamples);
    //memcpy(data.getData(), spikedata.getRawPointer(), nSamples*sizeof(float));

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
