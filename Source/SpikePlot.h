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

#ifndef SPIKEPLOT_H_
#define SPIKEPLOT_H_

#include <VisualizerWindowHeaders.h>

#include "SpikeSorterCanvas.h"
#include "Containers.h"
#include "BoxUnit.h"
#include "PCAUnit.h"

#include <vector>

class SpikeSorter;
class SpikeSorterCanvas;
class Electrode;
class PCAProjectionAxes;
class WaveformAxes;

#define MAX_N_CHAN 4

class SpikePlot : public Component, 
                  public Button::Listener
{
public:

    /** Constructor */
    SpikePlot(Electrode*);

    /** Destructor */
    virtual ~SpikePlot();

    /** Draws outline and electrode name*/
    void paint(Graphics& g);

    /** Sets bounds of sub-plots*/
    void resized();

    /** Gets the currently available units from the electrode*/
    void updateUnits();

    /** Turns PCAProjectionAxes polygon mode on or off*/
    void setPolygonDrawingMode(bool on);

    /** Sets the range of the PCAProjetionAxes*/
    void setPCARange(float p1min, float p2min, float p1max, float p2max);

    /** Sets axes limits*/
    void modifyRange(int index ,bool up);

    /** Sets axes limits*/
    void modifyRange(std::vector<float> values);
    
    /** Handle an incoming spike object*/
	void processSpikeObject(SorterSpikePtr s);

    Electrode* electrode;

    void getSelectedUnitAndBox(int& unitID, int& boxID);
    void setSelectedUnitAndBox(int unitID, int boxID);

    void initAxes(std::vector<float> scales);
    void getBestDimensions(int*, int*);

    void clear();

    float minWidth;
    float aspectRatio;

    void buttonClicked(Button* button);

    float getDisplayThresholdForChannel(int);
    void setDisplayThresholdForChannel(int channelNum, float thres);

private:

    int nWaveAx;
    int nProjAx;

    bool limitsChanged;

    double limits[MAX_N_CHAN][2];

    std::vector<BoxUnit> boxUnits;
    std::vector<PCAUnit> pcaUnits;

    OwnedArray<PCAProjectionAxes> pAxes;
    OwnedArray<WaveformAxes> wAxes;
    OwnedArray<UtilityButton> rangeButtons;
    Array<float> ranges;

    void initLimits();
    void setLimitsOnAxes();
    void updateAxesPositions();

    String name;
    CriticalSection mut;
    Font font;

};

#endif  // SPIKEPLOT_H_
