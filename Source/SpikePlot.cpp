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

#include "SpikePlot.h"

#include "SpikeSorter.h"
#include "PCAProjectionAxes.h"
#include "WaveformAxes.h"

SpikePlot::SpikePlot(Electrode* electrode_) :
    electrode(electrode_),
    limitsChanged(true)

{

    font = Font("Default", 15, Font::plain);

    switch (electrode->numChannels)
    {
    case 1:
        nWaveAx = 1;
        nProjAx = 1;
        minWidth = 600;
        aspectRatio = 0.5f;
        break;
    case 2:
        nWaveAx = 2;
        nProjAx = 1;
        minWidth = 300;
        aspectRatio = 0.5f;
        break;
    case 4:
        nWaveAx = 4;
        nProjAx = 1;
        minWidth = 400;
        aspectRatio = 0.5f;
        break;
    default: // unsupported number of axes provided
        std::cout << "SpikePlot as UNKNOWN, defaulting to SINGLE_PLOT" << std::endl;
        nWaveAx = 1;
        nProjAx = 0;
    }

    std::vector<float> scales = { 250, 250, 250, 250 }; // processor->getElectrodeVoltageScales(electrodeID);
    //initAxes(scales);

    for (int i = 0; i < electrode->numChannels; i++)
    {
        UtilityButton* rangeButton = new UtilityButton(String(scales[i], 0), Font("Small Text", 10, Font::plain));
        rangeButton->setRadius(3.0f);
        rangeButton->addListener(this);
        addAndMakeVisible(rangeButton);

        rangeButtons.add(rangeButton);
    }

}

SpikePlot::~SpikePlot()
{
    pAxes.clear();
    wAxes.clear();

}


void SpikePlot::setSelectedUnitAndBox(int unitID, int boxID)
{
    const ScopedLock myScopedLock(mut);
    electrode->sorter->setSelectedUnitAndBox(unitID, boxID);
}

void SpikePlot::getSelectedUnitAndBox(int& unitID, int& boxID)
{
    const ScopedLock myScopedLock(mut);
    electrode->sorter->getSelectedUnitAndBox(unitID, boxID);
}


void SpikePlot::paint(Graphics& g)
{

    g.setColour(Colours::white);
    g.drawRect(0, 0, getWidth(), getHeight());

    g.setFont(font);

    g.drawText(name, 10, 0, 200, 20, Justification::left, false);

}


void SpikePlot::setPolygonDrawingMode(bool on)
{
    const ScopedLock myScopedLock(mut);
    pAxes[0]->setPolygonDrawingMode(on);
}

void SpikePlot::updateUnits()
{
    const ScopedLock myScopedLock(mut);
    boxUnits = electrode->sorter->getBoxUnits();
    pcaUnits = electrode->sorter->getPCAUnits();

    if (nWaveAx > 0)
    {
        wAxes[0]->updateUnits(boxUnits);
    }
    pAxes[0]->updateUnits(pcaUnits);

    int selectedUnitID, selectedBoxID;
    electrode->sorter->getSelectedUnitAndBox(selectedUnitID, selectedBoxID);

}

void SpikePlot::setPCARange(float p1min, float p2min, float p1max, float p2max)
{
    const ScopedLock myScopedLock(mut);
    pAxes[0]->setPCARange(p1min, p2min, p1max, p2max);
}

void SpikePlot::processSpikeObject(SorterSpikePtr s)
{
    const ScopedLock myScopedLock(mut);

    if (nWaveAx > 0)
    {
        for (int i = 0; i < nWaveAx; i++)
        {
            wAxes[i]->updateSpikeData(s);
        }

        pAxes[0]->updateSpikeData(s);


    }
}


void SpikePlot::initAxes(std::vector<float> scales)
{
    //const ScopedLock myScopedLock(mut);
    initLimits();

    for (int i = 0; i < nWaveAx; i++)
    {
        WaveformAxes* wAx = new WaveformAxes(electrode, i);
        //wAx->setDetectorThreshold(electrode->thresholds[i]);
        wAxes.add(wAx);
        addAndMakeVisible(wAx);
        ranges.add(scales[i]);
    }

    PCAProjectionAxes* pAx = new PCAProjectionAxes(electrode);
    float p1min, p2min, p1max, p2max;
    electrode->sorter->getPCArange(p1min, p2min, p1max, p2max);
    pAx->setPCARange(p1min, p2min, p1max, p2max);

    pAxes.add(pAx);
    addAndMakeVisible(pAx);

    setLimitsOnAxes(); // initialize the ranges
}

void SpikePlot::resized()
{
    const ScopedLock myScopedLock(mut);

    float width = (float)getWidth() - 10;
    float height = (float)getHeight() - 25;

    float axesWidth = 0;
    float axesHeight = 0;

    // to compute the axes positions we need to know how many columns of proj and wave axes should exist
    // using these two values we can calculate the positions of all of the sub axes
    int nProjCols = 0;
    int nWaveCols = 0;

    switch (electrode->numChannels)
    {
    case 1:
        nProjCols = 1;
        nWaveCols = 1;
        axesWidth = width / 2;
        axesHeight = height;
        break;

    case 2:
        nProjCols = 1;
        nWaveCols = 2;
        axesWidth = width / 2;
        axesHeight = height;
        break;
    case 4:
        nProjCols = 1;
        nWaveCols = 2;
        axesWidth = width / 2;
        axesHeight = height / 2;
        break;
    }

    for (int i = 0; i < nWaveAx; i++)
    {
        wAxes[i]->setBounds(5 + (i % nWaveCols) * axesWidth / nWaveCols, 20 + (i / nWaveCols) * axesHeight, axesWidth / nWaveCols, axesHeight);
        rangeButtons[i]->setBounds(8 + (i % nWaveCols) * axesWidth / nWaveCols,
            20 + (i / nWaveCols) * axesHeight + axesHeight - 18,
            35, 15);
    }
    pAxes[0]->setBounds(5 + axesWidth, 20 + 0, width / 2, height);


}



void SpikePlot::modifyRange(std::vector<float> values)
{
    const int NUM_RANGE = 7;
    float range_array[NUM_RANGE] = { 100,250,500,750,1000,1250,1500 };
    String label;
    int newIndex = 0;

    for (int index = 0; index < electrode->numChannels; index++)
    {
        for (int k = 0; k < NUM_RANGE; k++)
        {
            if (std::abs(values[index] - range_array[k]) < 0.1)
            {
                newIndex = k;
                break;
            }
        }

        ranges.set(index, range_array[newIndex]);
        String label = String(range_array[newIndex], 0);
        rangeButtons[index]->setLabel(label);
    }

    setLimitsOnAxes();
}


void SpikePlot::modifyRange(int index, bool up)
{
    const int NUM_RANGE = 7;
    float range_array[NUM_RANGE] = { 100,250,500,750,1000,1250,1500 };
    String label;
    for (int k = 0; k < NUM_RANGE; k++)
    {
        if (std::abs(ranges[index] - range_array[k]) < 0.1)
        {
            int newIndex;
            if (up)
                newIndex = (k + 1) % NUM_RANGE;
            else
            {
                newIndex = (k - 1);
                if (newIndex < 0)
                    newIndex = NUM_RANGE - 1;
            }
            ranges.set(index, range_array[newIndex]);
            String label = String(range_array[newIndex], 0);
            rangeButtons[index]->setLabel(label);
            setLimitsOnAxes();

            //processor->setElectrodeVoltageScale(electrodeID, index, range_array[newIndex]);
            return;
        }

    }
    // we shoudl never reach here.
    jassert(false);
    return;

}

void SpikePlot::buttonClicked(Button* button)
{
    UtilityButton* buttonThatWasClicked = (UtilityButton*)button;

    int index = rangeButtons.indexOf(buttonThatWasClicked);
    modifyRange(index, true);

}

void SpikePlot::setLimitsOnAxes()
{
    const ScopedLock myScopedLock(mut);
    for (int i = 0; i < nWaveAx; i++)
        wAxes[i]->setRange(ranges[i]);
}

void SpikePlot::initLimits()
{
    for (int i = 0; i < electrode->numChannels; i++)
    {
        limits[i][0] = 1209;//-1*pow(2,11);
        limits[i][1] = 11059;//pow(2,14)*1.6;
    }

}

void SpikePlot::getBestDimensions(int* w, int* h)
{
    switch (electrode->numChannels)
    {
    case 4:
        *w = 4;
        *h = 2;
        break;
    case 2:
        *w = 2;
        *h = 1;
        break;
    case 1:
        *w = 1;
        *h = 1;
        break;
    default:
        *w = 1;
        *h = 1;
        break;
    }
}

void SpikePlot::clear()
{
    const ScopedLock myScopedLock(mut);
    std::cout << "SpikePlot::clear()" << std::endl;

    for (int i = 0; i < nWaveAx; i++)
        wAxes[i]->clear();
    for (int i = 0; i < nProjAx; i++)
        pAxes[i]->clear();


}


void SpikePlot::setDisplayThresholdForChannel(int i, float f)
{
    const ScopedLock myScopedLock(mut);
    if (i < wAxes.size())
        wAxes[i]->setDetectorThreshold(f);

    return;
}


float SpikePlot::getDisplayThresholdForChannel(int i)
{
    const ScopedLock myScopedLock(mut);
    float f = wAxes[i]->getDisplayThreshold();

    return f;
}

/********************************/
