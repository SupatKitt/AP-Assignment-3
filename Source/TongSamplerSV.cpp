/*
  ==============================================================================

    TongSamplerSV.cpp
    Created: 10 Dec 2022 11:58:37pm
    Author:  Supat Kittawornrat

  ==============================================================================
*/

#include "TongSamplerSV.h"


        TongSamplerSound::TongSamplerSound (const String& soundName,
                          AudioFormatReader& source,
                          const BigInteger& notes,
                          int midiNoteForNormalPitch,
                          double attackTimeSecs,
                          double releaseTimeSecs,
                          double maxSampleLengthSeconds)
        : tname (soundName),
        tsourceSampleRate (source.sampleRate),
        tmidiNotes (notes),
        tmidiRootNote (midiNoteForNormalPitch)
        {
            if (tsourceSampleRate > 0 && source.lengthInSamples > 0)
            {
                tlength = jmin ((int) source.lengthInSamples,
                                (int) (maxSampleLengthSeconds * tsourceSampleRate));
                
                tdata.reset (new AudioBuffer<float> (jmin (2, (int) source.numChannels), tlength + 4));
                
                source.read (tdata.get(), 0, tlength + 4, 0, true, true);
                
                tparams.attack  = static_cast<float> (attackTimeSecs);
                tparams.release = static_cast<float> (releaseTimeSecs);
            }
        }
        
        TongSamplerSound::~TongSamplerSound()
        {
        }
        
        bool TongSamplerSound::appliesToNote (int midiNoteNumber)
        {
            return tmidiNotes[midiNoteNumber]; // can enter return true instead
        }
        
        bool TongSamplerSound::appliesToChannel (int /*midiChannel*/)
        {
            return true;
        }
        
//========================================================================//
        
    TongSamplerVoice::TongSamplerVoice()//constructor
        {
            
        }
         
    TongSamplerVoice::~TongSamplerVoice() //destructor
        {
            
        }

        
        bool TongSamplerVoice::canPlaySound (SynthesiserSound* sound)
        {
            return dynamic_cast<const TongSamplerSound*> (sound) != nullptr;
        }

void TongSamplerVoice::connectEnvelopeParameters(std::atomic<float>* _sGain
                                                 ,std::atomic<float>* _sAttackParam
                                                 ,std::atomic<float>* _sDecayParam
                                                 ,std::atomic<float>* _sSustainParam
                                                 ,std::atomic<float>* _sReleaseParam)
    {
        masterGain = _sGain;
        attackParam = _sAttackParam;
        decayParam = _sDecayParam;
        sustainParam = _sSustainParam;
        releaseParam = _sReleaseParam;
    }
        
void TongSamplerVoice::startNote (int midiNoteNumber, float velocity, SynthesiserSound* s, int /*currentPitchWheelPosition*/) 
        {
            if (auto* sound = dynamic_cast<const TongSamplerSound*> (s))
            {
                pitchRatio = std::pow (2.0, (midiNoteNumber - sound->tmidiRootNote) / 12.0)
                * sound->tsourceSampleRate / getSampleRate();
                
                adsr.setSampleRate(getSampleRate());
                juce::ADSR::Parameters envParam;
                
                envParam.attack = *attackParam;
                envParam.decay = *decayParam;
                envParam.sustain = *sustainParam;
                envParam.release = *releaseParam;

                adsr.setParameters(envParam);
                
                sourceSamplePosition = 0.0;
                lgain = velocity;
                rgain = velocity;
                //DBG(lgain);
                
                adsr.noteOn();
            }
            else
            {
                jassertfalse; // this object can only play SamplerSounds!
            }
        }
        
        void TongSamplerVoice::stopNote (float /*velocity*/, bool allowTailOff)
        {
            
            if (allowTailOff)
            {
                adsr.noteOff();
            }
            else
            {
                clearCurrentNote();
                adsr.reset();
            }
        }
        
        
        void TongSamplerVoice::pitchWheelMoved (int /*newValue*/) {}

        void TongSamplerVoice::controllerMoved (int /*controllerNumber*/, int /*newValue*/) {}




        //==============================================================================
        void TongSamplerVoice::renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
        {
            if (auto* playingSound = static_cast<TongSamplerSound*> (getCurrentlyPlayingSound().get())) // get playing sound from constructor
            {
                // sound goes in here
                auto& data = *playingSound->tdata;
                const float* const inL = data.getReadPointer (0);
                const float* const inR = data.getNumChannels() > 1 ? data.getReadPointer (1) : nullptr;
                //setting channel
                float* outL = outputBuffer.getWritePointer (0, startSample);
                float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer (1, startSample) : nullptr;
                
                //setting up array for storing sample
                while (--numSamples >= 0) //actual processor slower than actual playhead
                {
                    auto pos = (int) sourceSamplePosition;
                    auto alpha = (float) (sourceSamplePosition - pos);
                    auto invAlpha = 1.0f - alpha;
                    
                    // just using a very simple linear interpolation here..
                    float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
                    float r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha)
                    : l;
                    
                    auto envelopeValue = adsr.getNextSample();
                    
                    l *= lgain * envelopeValue;
                    r *= rgain * envelopeValue;
                    
                    
                    if (outR != nullptr)
                    {
                        *outL++ += l * *masterGain;
                        *outR++ += r * *masterGain;
                    }
                    else
                    {
                        *outL++ += (l + r) * 0.5f;
                    }
                    
                    sourceSamplePosition += pitchRatio;
                    
                    if (sourceSamplePosition > playingSound->tlength)
                    {
                        sourceSamplePosition = 0;
                        //stopNote (0.0f, false);
                        break;
                    }
                    
                }
            }
        }
        

