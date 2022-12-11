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
    
    void setRandNum()
     {
         randNum = randomizer.nextInt(4);
     }
     
    void setSample(const void* sourceData, size_t sourceDataSize, int randNum)
    {
        formatmanager.registerBasicFormats();
        juce::BigInteger allNotes;
        allNotes.setRange(0, 128, true);
        switch (randNum)
        {
            case 0 :{
                auto* reader0 = formatmanager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::ForestNight1_wav, BinaryData::ForestNight1_wavSize, false));
                addSound( new juce::SamplerSound("default0", *reader0, allNotes, 60, 0, 0.1, 10.0f ) );
                break;
            }
            case 1 :{
                auto* reader1 = formatmanager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::ForestNight2_wav, BinaryData::ForestNight2_wavSize, false));
                addSound( new juce::SamplerSound("default1", *reader1, allNotes, 60, 0, 0.1, 10.0f ) );
                break;
            }
            case 2 :{
                auto* reader2 = formatmanager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::ForestNight3_wav, BinaryData::ForestNight3_wavSize, false));
                addSound( new juce::SamplerSound("default2", *reader2, allNotes, 60, 0, 0.1, 10.0f ) );
                break;
            }
            case 3 :{
                auto* reader3 = formatmanager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::ForestNight4_wav, BinaryData::ForestNight4_wavSize, false));
                addSound( new juce::SamplerSound("default3", *reader3, allNotes, 60, 0, 0.1, 10.0f ) );
                break;
            }
            case 4 :{
                auto* reader4 = formatmanager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::ForestNight5_wav, BinaryData::ForestNight5_wavSize, false));
                addSound( new juce::SamplerSound("default4", *reader4, allNotes, 60, 0, 0.1, 10.0f ) );
                break;
            }
            default:
                break;
        }
        
        
//        auto* reader = formatmanager.createReaderFor(std::make_unique<juce::MemoryInputStream>(BinaryData::ForestNight1_wav, BinaryData::ForestNight1_wavSize, false));
//        juce::BigInteger allNotes;
//        allNotes.setRange(0, 128, true);
//        addSound( new TongSamplerSound("default", *reader, allNotes, 60, 0, 0.1, 10.0f ) ); //name, format reader, range of notes, centerkey, attack, release, maximumduration
        
        //I cant use my TongSamplerSound here it cause jassert error
        
    }
//    ~TongSampler()
//    {
//        delete ;
//    }
    int getRandNum()
    {
        return randNum;
    }
private:
    juce::AudioFormatManager formatmanager;
    
    int randNum;
    juce::Random randomizer;
};

// try put the randomizer here
