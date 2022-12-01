/*
  ==============================================================================

    Tong_Sampler.h
    Created: 27 Nov 2022 3:34:30pm
    Author:  Supat Kittawornrat

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class TongSampler : public juce::Synthesiser
{
public:
    
    void init()
    {
        // use to initialize file format and point to the file location
        formatmanager.registerBasicFormats();
//        juce::File* file = new juce::File("/Users/supatkittawornrat/Desktop/TongLibrary/rawRecording/H6/Car pass by _100122_Edinburgh/ZOOM0001_LR.WAV"); //enter file directory in arguement
//
//        // this use to read the file that are pointed to
//        std::unique_ptr<juce::AudioFormatReader> reader; // wrap audio format reader class with smart pointer to prevent memory leak
//        //reader->sampleRate;
//        reader.reset( formatmanager.createReaderFor(*file) ); //reset is a function for unique pointer
//
//        juce::BigInteger allNotes;
//        allNotes.setRange(0, 128, true);
//
//        addSound( new juce::SamplerSound("default", *reader, allNotes, 60, 0, 0.1, 10.0f ) ); //name, format reader, range of notes, centerkey, attack, release, maximumduration
    }
    
    void setSample(const void* sourceData, size_t sourceDataSize)
    {
        formatmanager.registerBasicFormats();
        auto* reader = formatmanager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::ForestNight1_wav, BinaryData::ForestNight1_wavSize, false));
        
        juce::BigInteger allNotes;
        allNotes.setRange(0, 128, true);
        
        addSound( new juce::SamplerSound("default", *reader, allNotes, 60, 0, 0.1, 10.0f ) ); //name, format reader, range of notes, centerkey, attack, release, maximumduration
        
    }
//    ~TongSampler()
//    {
//        delete ;
//    }

private:
    juce::AudioFormatManager formatmanager;
    
};
