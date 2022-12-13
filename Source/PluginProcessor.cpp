/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioProgramming_AMB_SynthAudioProcessor::AudioProgramming_AMB_SynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
//put parameter ID, name, lowest , highest, default setting here
apvts(*this, nullptr, "ParameterTreeState", {
    std::make_unique<juce::AudioParameterChoice>("waveShape", "Wave Shape", StringArray({"Sine", "Saw", "Triangle", "Square" }), 0),
    std::make_unique<juce::AudioParameterFloat>("masterGain", "Gain Output", 0, 5.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>("gain", "Note Gain", 0.001f, 0.9f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>("attack", "Note Attack time", 0.001f, 5.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>("decay", "Note Decay time", 0.001f, 5.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>("sustain", "Note Sustain level", 0.1, 0.9f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>("release", "Note Release time", 0.001f, 10.0f, 3.0f),
    std::make_unique<juce::AudioParameterFloat>("sampleGain", "Nature Gain", 0.001f, 0.9f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>("sampleAttack", "Nature attack time", 0.1f, 20.0f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>("sampleDecay", "Nature decay time", 0.1f, 20.0f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>("sampleSustain", "Nature sustain value", 0.1f, 0.9f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>("sampleRelease", "Nature release time", 0.1f, 20.0f, 2.0f),
    std::make_unique<juce::AudioParameterFloat>("noteCutoffFreqL", "Note cutoff Frequency", 1, 20000, 20000),
    std::make_unique<juce::AudioParameterFloat>("filterQL", "Note Filter Q L", 0.01, 10.0f, 0.01),
    std::make_unique<juce::AudioParameterFloat>("sampleWetLevel", "Nature Wet Level", 0, 1.0f, 0),
    std::make_unique<juce::AudioParameterFloat>("sampleDryLevel", "Nature Dry Level", 0, 1.0f, 1),
    std::make_unique<juce::AudioParameterFloat>("width", "Width", 0, 1.0f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>("sampleRoomSize", "Space Size", 0, 1.0f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>("sampleLowpassFilter", "Nature Lowpass", 1, 20000, 20000),
    std::make_unique<juce::AudioParameterFloat>("sampleLowpassQ", "Nature Lowpass Q", 0.01, 10.0f, 0.01),
    std::make_unique<juce::AudioParameterFloat>("sampleHighpassFilter", "Nature Highpass", 1, 20000, 1),
    std::make_unique<juce::AudioParameterFloat>("sampleHighpassQ", "Nature Highpass Q", 0.01, 10.0f, 0.01),
})
//constructor
{
    synth.clearVoices();
    for (int i = 0; i < voiceCount; i++)
    {
        synth.addVoice(new TongSynthVoice() );
        sampler.addVoice(new TongSamplerVoice());
    }
    synth.clearSounds();
    
    // add "sound" to synth use to control parts of keyboard
    synth.addSound(new TongSynthSound() );
    //link parametervalue with the UI, with parameter ID
    masterGain = apvts.getRawParameterValue("masterGain");
    wetLevel = apvts.getRawParameterValue("sampleWetLevel");
    dryLevel = apvts.getRawParameterValue("sampleDryLevel");
    width = apvts.getRawParameterValue("width");
    roomSize =  apvts.getRawParameterValue("sampleRoomSize");
    
    // for linking with Synth and Sampler class
    for (int voiceNum = 0; voiceNum < voiceCount; voiceNum++)
    {
        TongSynthVoice* voicePtr = dynamic_cast<TongSynthVoice*>(synth.getVoice(voiceNum));
        voicePtr->connectEnvelopeParameters(apvts.getRawParameterValue("waveShape"),
                                            apvts.getRawParameterValue("gain"),
                                            apvts.getRawParameterValue("attack"),
                                            apvts.getRawParameterValue("decay"),
                                            apvts.getRawParameterValue("sustain"),
                                            apvts.getRawParameterValue("release"),
                                            apvts.getRawParameterValue("noteCutoffFreqL"),
                                            apvts.getRawParameterValue("filterQL")
                                            ); //link with synth
        
        TongSamplerVoice* samVoicePtr = dynamic_cast<TongSamplerVoice*>(sampler.getVoice(voiceNum));
        samVoicePtr->connectEnvelopeParameters(apvts.getRawParameterValue("sampleGain"),
                                               apvts.getRawParameterValue("sampleAttack"),
                                               apvts.getRawParameterValue("sampleDecay"),
                                               apvts.getRawParameterValue("sampleSustain"),
                                               apvts.getRawParameterValue("sampleRelease"),
                                               apvts.getRawParameterValue("sampleLowpassFilter"),
                                               apvts.getRawParameterValue("sampleLowpassQ"),
                                               apvts.getRawParameterValue("sampleHighpassFilter"),
                                               apvts.getRawParameterValue("sampleHighpassQ")
                                               ); //link with sampler
        myRandomInt = randomer.nextInt(4);
        switch (myRandomInt)
        {
            case 0 :{ sampler.setSample(BinaryData::ForestNight1_wav, BinaryData::ForestNight1_wavSize); break ;}
            case 1 :{ sampler.setSample(BinaryData::ForestNight2_wav, BinaryData::ForestNight2_wavSize); break ;}
            case 2 :{ sampler.setSample(BinaryData::ForestNight3_wav, BinaryData::ForestNight3_wavSize); break ;}
            case 3 :{ sampler.setSample(BinaryData::ForestNight4_wav, BinaryData::ForestNight4_wavSize); break ;}
            case 4 :{ sampler.setSample(BinaryData::ForestNight5_wav, BinaryData::ForestNight5_wavSize); break ;}
            default : {sampler.setSample(BinaryData::ForestNight1_wav, BinaryData::ForestNight1_wavSize); break;}
        }
    }
}

AudioProgramming_AMB_SynthAudioProcessor::~AudioProgramming_AMB_SynthAudioProcessor()
{
}

//==============================================================================
const juce::String AudioProgramming_AMB_SynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioProgramming_AMB_SynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioProgramming_AMB_SynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioProgramming_AMB_SynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioProgramming_AMB_SynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioProgramming_AMB_SynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioProgramming_AMB_SynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioProgramming_AMB_SynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AudioProgramming_AMB_SynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioProgramming_AMB_SynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AudioProgramming_AMB_SynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // set sample rate for synth, sampler, and reverb.
    synth.setCurrentPlaybackSampleRate(sampleRate);
    sampler.setCurrentPlaybackSampleRate(sampleRate);
    reverb.setSampleRate(sampleRate);
    
}

void AudioProgramming_AMB_SynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioProgramming_AMB_SynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AudioProgramming_AMB_SynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    // sound working fine from here with this loop might be some other issue that does not making the loop working
    
//    if (samplecount == 0)
//    {
//        myRandomInt = randomer.nextInt(4);
//        switch (myRandomInt)
//        {
//            case 0 :{ sampler.setSample(BinaryData::ForestNight1_wav, BinaryData::ForestNight1_wavSize); break ;}
//            case 1 :{ sampler.setSample(BinaryData::ForestNight2_wav, BinaryData::ForestNight2_wavSize); break ;}
//            case 2 :{ sampler.setSample(BinaryData::ForestNight3_wav, BinaryData::ForestNight3_wavSize); break ;}
//            case 3 :{ sampler.setSample(BinaryData::ForestNight4_wav, BinaryData::ForestNight4_wavSize); break ;}
//            case 4 :{ sampler.setSample(BinaryData::ForestNight5_wav, BinaryData::ForestNight5_wavSize); break ;}
//            default : {sampler.setSample(BinaryData::ForestNight1_wav, BinaryData::ForestNight1_wavSize); break;}
//        }
//    }
//    samplecount++;
//    if (samplecount == 1000)
//    {
//        samplecount = 0;
//    }

    juce::Reverb::Parameters reverbParam;
    reverbParam.wetLevel = *wetLevel;
    reverbParam.dryLevel = *dryLevel;
    reverbParam.width = *width;
    reverbParam.roomSize = *roomSize;
    reverb.setParameters(reverbParam);
    
    buffer.clear();
    
    //create pointer to buffer array to apply reverb
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);
    
    //output sample data to buffer
    sampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    //apply gain to buffer with only sample
    buffer.applyGain(4);
    
    //process reverb
    reverb.processStereo(leftChannel, rightChannel, buffer.getNumSamples());
    
    //output synth data to buffer
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    //set master gain
    buffer.applyGain(*masterGain);
   
}

//==============================================================================
bool AudioProgramming_AMB_SynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioProgramming_AMB_SynthAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void AudioProgramming_AMB_SynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    //for presets
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void AudioProgramming_AMB_SynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    //for presets
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName (apvts.state.getType()))
        {
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioProgramming_AMB_SynthAudioProcessor();
}
