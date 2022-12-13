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

/**inherited from SynthesiserSound;  This allows more than one SynthesiserVoice to play the same sound at the same time.*/
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
    
    void connectEnvelopeParameters(std::atomic<float>* _sGain
                                   ,std::atomic<float>* _sAttackParam
                                   ,std::atomic<float>* _sDecayParam
                                   ,std::atomic<float>* _sSustainParam
                                   ,std::atomic<float>* _sReleaseParam
                                   ,std::atomic<float>* _sDryLevel
                                   ,std::atomic<float>* _sWetLevel
                                   ,std::atomic<float>* _sWidth
                                   ,std::atomic<float>* _sRoomSize
                                   ,std::atomic<float>*  _slocalSamplerLowpassFreq
                                   ,std::atomic<float>*  _slocalSamplerLowpassQ
                                   ,std::atomic<float>*  _slocalSamplerHighpassFreq
                                   ,std::atomic<float>*  _slocalSamplerHighpassQ);
    
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
    std::atomic<float>* masterGain;
    ADSR adsr;
    
    std::atomic<float>* attackParam;
    std::atomic<float>* decayParam;
    std::atomic<float>* sustainParam;
    std::atomic<float>* releaseParam;
    
    //Reverb
    juce::Reverb sReverb;
    std::atomic<float>* dryLevel;
    std::atomic<float>* wetLevel;
    std::atomic<float>* roomSize;
    std::atomic<float>* width;
    
    // Lowpass
    juce::IIRFilter samplerLowpassFilter;
    std::atomic<float>*  localSamplerLowpasscutoffFreq;
    std::atomic<float>*  localSamplerLowpassQ;
    
    // Highpass
    juce::IIRFilter samplerHighpassFilter;
    std::atomic<float>*  localSamplerHighpassFreq;
    std::atomic<float>*  localSamplerHighpassQ;
    
};

