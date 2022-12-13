/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MySynth.h"
#include "Tong_Osc.h"
#include "Tong_Sampler.h"
#include "TongSamplerSV.h"
#include <ctime>
//==============================================================================
/**
*/
class AudioProgramming_AMB_SynthAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    AudioProgramming_AMB_SynthAudioProcessor();
    ~AudioProgramming_AMB_SynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    
private:
    
    float noteStore;
    
    float sampleStore;
    
    juce::AudioProcessorValueTreeState apvts;
    
    
    juce::Synthesiser synth;
    
    int voiceCount = 6;
    
    TongSampler sampler;
    
    
    juce::Random randomer;
    
    juce::Reverb reverb;
    std::atomic<float>* dryLevel;
    std::atomic<float>* wetLevel;
    std::atomic<float>* roomSize;
    std::atomic<float>* width;
    
    int myRandomInt;
    int samplecount = 0;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProgramming_AMB_SynthAudioProcessor)
};
