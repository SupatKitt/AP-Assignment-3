/*
  ==============================================================================

    TongSamplerVoice.h
    Created: 9 Dec 2022 9:47:22pm
    Author:  Supat Kittawornrat

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

using namespace juce;

class TongSamplerSound : public juce::SynthesiserSound
{
public:
    
    //constructor
    TongSamplerSound (const String& soundName,
                      AudioFormatReader& source,
                      const BigInteger& notes,
                      int midiNoteForNormalPitch,
                      double attackTimeSecs,
                      double releaseTimeSecs,
                      double maxSampleLengthSeconds);
    //destructor
    ~TongSamplerSound()override;
    
    bool appliesToNote (int midiNoteNumber) override;
    
    bool appliesToChannel (int /*midiChannel*/) override;
    
    
    
private:
    
    String tname;
    std::unique_ptr<AudioBuffer<float>> tdata;
    double tsourceSampleRate;
    BigInteger tmidiNotes;
    int tlength = 0, tmidiRootNote = 0;
    
    ADSR::Parameters tparams;
    friend class TongSamplerVoice;
};

class TongSamplerVoice : public juce::SynthesiserVoice
{
public:
    ///constructor
    TongSamplerVoice();
    ///destructor
    ~TongSamplerVoice() override;
    
    void connectEnvelopeParameters(std::atomic<float>* _sattackParam
                                   ,std::atomic<float>* _sdecayParam
                                   ,std::atomic<float>* _ssustainParam
                                   ,std::atomic<float>* _sreleaseParam);
    
    bool canPlaySound (SynthesiserSound* sound) override ;
    
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound* s, int /*currentPitchWheelPosition*/) override ;
    
    void stopNote (float /*velocity*/, bool allowTailOff) override ;
    
    void pitchWheelMoved (int /*newValue*/) override ;
    
    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override ;
    
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override ;
    
private:
    
    double pitchRatio = 0;
    double sourceSamplePosition = 0;
    float lgain = 0, rgain = 0;

    ADSR adsr;
    
//    std::atomic<float>* attackParam;
//    std::atomic<float>* decayParam;
//    std::atomic<float>* sustainParam;
//    std::atomic<float>* releaseParam;
    
};

