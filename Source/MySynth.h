/*
  ==============================================================================

    MySynth.h
    Created: 16 Nov 2022 11:27:14am
    Author:  Supat Kittawornrat

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Tong_Osc.h" //use here instead synth object here
#include "Chorus.h"
#include <vector>

///subclass intherited from synthesiser sound
class TongSynthSound : public juce::SynthesiserSound
{
public:
    // trigger synth when get midi note 
    bool appliesToNote      (int) override      { return true; }
    //--------------------------------------------------------------------------
    bool appliesToChannel   (int) override      { return true; }
};
// =================================
// =================================
// Synthesiser Voice - your synth code goes in here

/*!
 @class YourSynthVoice
 @abstract struct defining the DSP associated with a specific voice.
 @discussion multiple YourSynthVoice objects will be created by the Synthesiser so that it can be played polyphicially
 
 @namespace none
 @updated 2019-06-18
 */
///subclass intherited from synthesiser voice
class TongSynthVoice : public juce::SynthesiserVoice
{
public:
    //constructor
    TongSynthVoice() {}
    //--------------------------------------------------------------------------
    /**
     What should be done when a note starts

     @param midiNoteNumber
     @param velocity
     @param SynthesiserSound unused variable
     @param / unused variable
     */
    
    /// connect linkable parameter with constructor
    void connectEnvelopeParameters(std::atomic<float>* _waveShape
                                   ,std::atomic<float>* _gain
                                   ,std::atomic<float>* _attackParam
                                   ,std::atomic<float>* _decayParam
                                   ,std::atomic<float>* _sustainParam
                                   ,std::atomic<float>* _releaseParam
                                   ,std::atomic<float>* _noteLowpasscutoffFreqL
                                   ,std::atomic<float>* _noteLowpassQL)
    {
        //dropDown waveshape
        waveShape = _waveShape;
        
        // amplitude parameter
        gain = _gain;
        attackParam = _attackParam;
        decayParam = _decayParam;
        sustainParam = _sustainParam;
        releaseParam = _releaseParam;
        
        // filter parameter
        localNoteLowpasscutoffFreqL = _noteLowpasscutoffFreqL;
        localNoteLowpassQL = _noteLowpassQL;

    }
    
    /// very much like prepareToPlay in PluginProcessor
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        
        // set sampleRate for oscillator
        sineOsc.setSampleRate(getSampleRate());
        sawOsc.setSampleRate(getSampleRate());
        triOsc.setSampleRate(getSampleRate());
        sqosc.setSampleRate(getSampleRate());
        
        // can set adsr parameter in constructor because it is public
        env.setSampleRate(getSampleRate());
        
        //set filter parameter
        filterL.setCoefficients(juce::IIRCoefficients::makeLowPass(getSampleRate(), *localNoteLowpasscutoffFreqL, *localNoteLowpassQL));

        
        // create parameter object
        juce::ADSR::Parameters envParam; //(1.0, 0.3, 0.3, 1)you can put an arguement for ADSR after creating parameter object
        // assign parameter via method in ADSR parameters class
        envParam.attack = *attackParam; //in seconds // dereference and use variable stored in private that connect to parameter
        envParam.decay = *decayParam; //in seconds
        envParam.sustain = *sustainParam;// in levels
        envParam.release = *releaseParam;// in seconds
        env.setParameters(envParam);
        
        env.noteOn();
        
        localVelocity = velocity;
        
        float freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        sineOsc.setFrequency(freq);
        sawOsc.setFrequency(freq);
        triOsc.setFrequency(freq);
        sqosc.setFrequency(freq);
        
        playing = true;
        
        //DBG("StartNote");
        
    }
    //--------------------------------------------------------------------------
    /// Called when a MIDI noteOff message is received
    /**
     What should be done when a note stops

     @param / unused variable
     @param allowTailOff bool to decie if the should be any volume decay
     */
    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        env.noteOff(); // release start when release note(stopNote)
    }
    
    //--------------------------------------------------------------------------
    /**
     The Main DSP Block: Put your DSP code in here
          
          If the sound that the voice is playing finishes during the course of this rendered block, it must call clearCurrentNote(), to tell the synthesiser that it has finished

          @param outputBuffer pointer to output
          @param startSample position of first sample in buffer
          @param numSamples number of samples in output buffer
          */
         void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
         {

             if (playing)
             {
                 float* left = outputBuffer.getWritePointer(0);
                 float* right = outputBuffer.getWritePointer(1);
                 
                 //use starSample becuase the buffer might not start at 0 but startsomewhere in between so we start there to sync with the buffer
                 for (int i = startSample; i < numSamples+startSample ; i++ )
                 {
                     float envVal = env.getNextSample();
                     
                     float signalVal;
                     
                     //switch case recieve from dropdown menu to change waveshape
                     int choice = *waveShape;
                     switch(choice)
                     {
                         case 0 :{ signalVal = sineOsc.process() * envVal * localVelocity; break;}
                         case 1 :{ signalVal = sawOsc.process() * envVal * localVelocity; break;}
                         case 2 :{ signalVal = triOsc.process() * envVal * localVelocity; break;}
                         case 3 :{signalVal = sqosc.process() * envVal * localVelocity; break;}
                         default:{signalVal = sineOsc.process() * envVal * localVelocity; break;}
                     }
                     
                     // += for creating polyphony if = only buffer will stop the note before and start next note if use += next note will be add together with previous note
                     float filteredSamp = filterL.processSingleSampleRaw(signalVal);
                     
                     left[i] += (filteredSamp *  *gain);
                     right[i] += (filteredSamp *  *gain);
                     
                     if(! env.isActive()) //move stop note to here (! means if envelope is not active)
                     {
                         clearCurrentNote();
                         playing = false; // when putted here the note will stop when there are no more samples to run
                     }
                 }
             }
         }
         //--------------------------------------------------------------------------
         void pitchWheelMoved(int) override {}
         //--------------------------------------------------------------------------
         void controllerMoved(int, int) override {}
         //--------------------------------------------------------------------------
         /**
          Can this voice play a sound. I wouldn't worry about this for the time being

          @param sound a juce::SynthesiserSound* base class pointer
          @return sound cast as a pointer to an instance of YourSynthSound
          */
            //use for validate midi input -- inherited from synth sound to MySynthSound
         bool canPlaySound (juce::SynthesiserSound* sound) override
         {
             return dynamic_cast<TongSynthSound*> (sound) != nullptr;
         }
         //--------------------------------------------------------------------------
     private:
         //--------------------------------------------------------------------------
         // Set up any necessary variables here
         /// Should the voice be playing?
         bool playing = false;

         /// a random object for use in our test noise function
         //juce::Random random;
            
        SineOsc sineOsc;
        TriangleOsc triOsc;
        Phasor sawOsc;
        SquareOsc sqosc;
        juce::ADSR env;
    
        float localVelocity;
    
        std::atomic<float>* waveShape;
        std::atomic<float>* gain;
    
        std::atomic<float>* attackParam;
        std::atomic<float>* decayParam;
        std::atomic<float>* sustainParam;
        std::atomic<float>* releaseParam;
    
        juce::IIRFilter filterL;
    
    
        std::atomic<float>*  localNoteLowpasscutoffFreqL;
        std::atomic<float>*  localNoteLowpassQL;

    
};


