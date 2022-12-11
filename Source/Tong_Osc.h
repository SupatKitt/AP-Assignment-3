//
//  Tong_Osc.h
//  Oscillator_Class
//
//  Created by ** on 10/14/22.
//

#ifndef Tong_Osc_h
#define Tong_Osc_h
#include<cmath>
class Phasor
{
public: // make all of this visible to all scope until see another keyword
    
    virtual ~Phasor(){}
    //Setter
    ///must be called called / initialize first in order to gain sample rate information to calculate in setFrequency()
    void setSampleRate (float samplerate)
    {
        sampleRate = samplerate;
    }
    
    /// for set frequency use only within class
    void setFrequency(float newFrequency)
    {
        frequency  = newFrequency;
       phaseDelta  = frequency/sampleRate;
        //in short phaseDelta=newFrequency/sampleRate;
    }
    /// apply to for loop for phasing and create sine tone from applying 2*pi to phase value
     float process ()
    {
        phase += phaseDelta;
        
        if ( phase > 1.0 )
        {
            phase -= 1.0 ;
        }
        return output(phase); //return the value itself || this initialize virtual function and send float value to output() (child class contain all of this )
    }
    //recieve single element of phase|| this function is use for sending phase data to child class (inheritence)
    virtual float output(float p)
    {
        return p;
    }
    
private: // for keeping variable safe and work only with function
     float sampleRate;
     float phase {0};
     float frequency;
     float phaseDelta;
};
///Sinusodial Oscillator
class SineOsc : public Phasor
{
public:
    
    virtual float output(float p) override
    {
        return sin( p * 2 * pi );
    }
    virtual ~SineOsc(){}
private:
   const float pi = 3.1415926535897932;
};

class SquareOsc : public Phasor
{
public:
        virtual float output(float p) override
    {
        float outVal = 0.5f;

        // the reason that we use if condition p < 0.5 is because phase delta range from 0 - 1 so 0.5 would be half of it
        if(p > pulseWidth)
            outVal = -0.5 ;
//        else
//            outVal = 0.5;

        //this can also achieve the same result by using Sinusodial wave instead of working on the phase itself
//        outVal = sin ( p * 2 * M_PI );
//        if (outVal > 0)
//            outVal = 0.9;
//        else
//            outVal =-0.9;
        
        return outVal ;
    }
    /// set pulse width before using SquareOsc default at 0.5
    void setPulseWidth(float pw) //this allow us to set/modulate the pulse width
    {
        pulseWidth = pw;
    }
    
    virtual ~SquareOsc(){}
    
private:
    
    float pulseWidth = 0.5f ;
    
};

class TriangleOsc : public Phasor
{
public:
    virtual float output(float p) override
{
    // took from github rchoscillators
   // float outVal = ((p < 0.5) * (4.0 * p - 1.0)) + ((p >= 0.5) * (1.0 - 4.0 * (p - 0.5)));
    
    float outVal ;
    if (p < 0.5)
    {
        outVal = p * 4.0 - 1.0;
    }
    else
    {
        outVal = (1.0 - p) * 4.0 - 1.0;
    }
            
    return outVal ;
}
    
    virtual ~TriangleOsc(){}
    
};


#endif /* Tong_Osc_h */
