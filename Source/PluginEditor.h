/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AudioProgramming_AMB_SynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AudioProgramming_AMB_SynthAudioProcessorEditor (AudioProgramming_AMB_SynthAudioProcessor&);
    ~AudioProgramming_AMB_SynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioProgramming_AMB_SynthAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioProgramming_AMB_SynthAudioProcessorEditor)
};
