/*
  ==============================================================================

    Tong_Sampler.h
    Created: 27 Nov 2022 3:34:30pm
    Author:  Supat Kittawornrat

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include"TongSamplerSV.h"

class TongSampler : public juce::Synthesiser
{
public:
    
    /// Register basic format to format manager .wav
    void init()
    {
        formatmanager.registerBasicFormats();
    }
    
    /// Receive sample call from PluginProcessor
    void setSample (const void* sourceData, size_t sourceDataSize)
    {
        // flag false in case you want to play same sound at the same time
        if (checker == false)
        {
            formatmanager.registerBasicFormats();
            checker = true;
        }
        
        auto* reader = formatmanager.createReaderFor (std::make_unique<juce::MemoryInputStream> (sourceData, sourceDataSize, false));
        juce::BigInteger allNotes; // create BigInteger object to store range of available key to play sample
        allNotes.setRange (0, 128, true); //all key
        addSound ( new TongSamplerSound("default", *reader, allNotes, 60, 0, 0.1, 15.0f ) );
        //name, format reader, range of notes, centerkey, attack, release, maximumduration
        
        
    }
 
private:
            
    juce::AudioFormatManager formatmanager;
    bool checker = false ;

};


