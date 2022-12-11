/*
  ==============================================================================

    Chorus.h
    Created: 4 Nov 2022 10:48:35pm
    Author:  **

  ==============================================================================
*/

#pragma once
#include"Feedback.h"
#include"Tong_Osc.h"
#include <cmath>

class Chorus
{
public:
    
    /** Set Feedback tail time between 0-1!!*/
    void setFeedbackLength(float _feedbackLength)
    {
        Del1.setFeedbackLength(_feedbackLength);
        Del2.setFeedbackLength(_feedbackLength);
        Del3.setFeedbackLength(_feedbackLength);
    }
    
    /** set the maximum size of the delay line which is buffer size*/
    void setDelaySize (int _size)
    {
        Del1.setFeedbackSize(_size);
        Del2.setFeedbackSize(_size);
        Del3.setFeedbackSize(_size);
    }
    
    /**set delaytime around 15-20 ms (48 sample per ms for 48khz samplerate)*/
    void setLowestHighestDelaytimeSamples(float LowestDelayTime, float HighestDelayTime)
    {
        highestDelayTime = HighestDelayTime;
        lowestDelaytime = HighestDelayTime + LowestDelayTime;
        
        Del1.setFeedbacktimeInSamples(delaytime1);
        Del2.setFeedbacktimeInSamples(delaytime2);
        Del3.setFeedbacktimeInSamples(delaytime3);
    }
    
    /** set lfo value.
        don't set lfo value to the same number so that it creates variation
     */
    void setLfoVal (float sr, float _freqLfo1, float _freqLfo2, float _freqLfo3)
    {
        samplerate = sr;
        lfo1.setSampleRate(samplerate);
        lfo2.setSampleRate(samplerate);
        lfo3.setSampleRate(samplerate);
        lfo1.setFrequency(_freqLfo1);
        lfo2.setFrequency(_freqLfo2);
        lfo3.setFrequency(_freqLfo3);

    }
    
    /** chorus process Enter sample Data
        basically the Lfo modulate the delay time of other sample so that it varies from the entered sample**/
 float process(float sampleData) //process lfo modulate the delay time
    {
        
        float lfo1Val = lfo1.process();
        float lfo2Val = lfo2.process();
        float lfo3Val = lfo3.process();
        
          delaytime1 = (lfo1Val * highestDelayTime) + lowestDelaytime;
          Del1.setFeedbacktimeInSamples(delaytime1);
        
          delaytime2 = (lfo2Val * highestDelayTime) + lowestDelaytime;
          Del2.setFeedbacktimeInSamples(delaytime2);
        
          delaytime3 = (lfo3Val * highestDelayTime) + lowestDelaytime;
          Del3.setFeedbacktimeInSamples(delaytime3);
        
        float del1Val = Del1.process(sampleData);
        float del2Val = Del2.process(sampleData);
        float del3Val = Del3.process(sampleData);
        
        float mix = (del1Val + del2Val + del3Val);
        return mix;
    }
    
private:
    
    Feedback Del1;
    Feedback Del2;
    Feedback Del3;
    
    float delaytime1;
    float delaytime2;
    float delaytime3;
    
    float lowestDelaytime;
    float highestDelayTime;
    
    SineOsc lfo1;
    SineOsc lfo2;
    SineOsc lfo3;
    float samplerate;
    
};
