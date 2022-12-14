/*
  ==============================================================================

    Tong_Detune.h
    Created: 13 Dec 2022 9:33:36pm
    Author:  Supat Kittawornrat

  ==============================================================================
*/

#pragma once
#include "Tong_Osc.h"

///This class create 2 detune wave from the main input frequency
class Detuner 
{
public:
    Detuner() {}
    
    ~Detuner() {}
    
    ///Set sampler rate for all osc
    void setDetunerSampleRate (float sampleRate)
    {
        sawDetune1.setSampleRate (sampleRate);
        sawDetune2.setSampleRate (sampleRate);
        
        sineDetune1.setSampleRate (sampleRate);
        sineDetune2.setSampleRate (sampleRate);
        
        triDetune1.setSampleRate (sampleRate);
        triDetune2.setSampleRate (sampleRate);
        
        sqDetune1.setSampleRate (sampleRate);
        sqDetune2.setSampleRate (sampleRate);
    }
    
    ///Set frequency and detune rate then return float (saw)
    float sawDetuner()
    {
        float positive = mainFreq + detunerate;
        float negative = mainFreq - detunerate;
        sawDetune1.setFrequency (positive);
        sawDetune2.setFrequency (negative);
        float finalPos1 = sawDetune1.process();
        float finalNeg2 = sawDetune2.process();
        return (finalPos1 + finalNeg2)/2;
    }
    
    ///Set frequency and detune rate then return float (sine)
    float sineDetuner()
    {
        float positive = mainFreq + detunerate;
        float negative = mainFreq - detunerate;
        sineDetune1.setFrequency (positive);
        sineDetune2.setFrequency (negative);
        float finalPos1 = sineDetune1.process();
        float finalNeg2 = sineDetune2.process();
        return (finalPos1 + finalNeg2)/2;
    }
    
    ///Set frequency and detune rate then return float (saw)
    float TriangleDetuner()
    {
        float positive = mainFreq + detunerate;
        float negative = mainFreq - detunerate;
        triDetune1.setFrequency (positive);
        triDetune2.setFrequency (negative);
        float finalPos1 = triDetune1.process();
        float finalNeg2 = triDetune2.process();
        return (finalPos1 + finalNeg2)/2;
    }
    ///Set frequency and detune rate then return float (square)
    float squareDetuner()
    {
        float positive = mainFreq + detunerate;
        float negative = mainFreq - detunerate;
        sqDetune1.setFrequency (positive);
        sqDetune2.setFrequency (negative);
        float finalPos1 = sqDetune1.process();
        float finalNeg2 = sqDetune2.process();
        return (finalPos1 + finalNeg2)/2;
    }
    
    ///use this to set detune Parameter
    void setDetuneParam(float _mainFreq, float _detunerate)
    {
        mainFreq = _mainFreq;
        detunerate = _detunerate;
    }
    
    
private:
    Phasor sawDetune1;
    Phasor sawDetune2;
    
    SineOsc sineDetune1;
    SineOsc sineDetune2;
    
    TriangleOsc triDetune1;
    TriangleOsc triDetune2;
    
    SquareOsc sqDetune1;
    SquareOsc sqDetune2;
    
    float mainFreq;
    float detunerate;
    
};

