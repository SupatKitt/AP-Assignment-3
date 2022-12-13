/*
  ==============================================================================

    Feedback.h
    Created: 4 Nov 2022 4:36:51pm
    Author:  **

  ==============================================================================
*/

#pragma once

/** Delay class this class will get a sample value at realtime process(read) and output(write) it later as user inputted time*/
class Feedback
{
public:
    
    /// This clean up the dynamiically allocated arrray when object is destroyed
    ~Feedback()
    {
        if (data != nullptr)
            delete[] data;
    }
    
    /** Set Feedback tail time between 0-1!! default at 0.5*/
    void setFeedbackLength(float _feedbackLength)
    {
        feedback = _feedbackLength;
        
        if(feedback < 0)
            feedback = 0;
        else if(feedback > 1)
            feedback = 1;
    }
    
    /** set the maximum size of the delay line which is buffer size*/
    void setFeedbackSize(int _size)
    {
        maxDelaySize = _size;     //user store size
        
        if (data != nullptr)
            delete[] data;        //clean up a address where pointer point to
        
        data = new float[maxDelaySize];     //initialize array to user inputted size
        
        for (int i = 0; i < maxDelaySize; i++)
        {
            data[i] = 0.0;
        }
    }
    
    /**set delaytime around 15-20 ms (48 sample per ms for 48khz samplerate)*/
    void setFeedbacktimeInSamples (float newDelayTime)
    {
        delayTime = newDelayTime;
        
        readIndex = writeIndex - delayTime; //write index = any value but - delaytime to make sure that read index follow write index
        
        if(readIndex < 0)
            readIndex = readIndex + maxDelaySize;
        
        if(readIndex > maxDelaySize)
            readIndex = readIndex - maxDelaySize;
    }
    
    /** input signal in the process to writeIndex then the process pass through the delay time it came out at readIndex*/
    float process(float inputSample)
    {
        //get val of data at read index
        float outputSample = linearinterpolation(); //data[readIndex];
        
        //store val at write index
        data[writeIndex] = inputSample + (outputSample * feedback);
        
        /** wrapping read write head in buffer*/
        
        readIndex++;
        if (readIndex >= maxDelaySize)
        {
            readIndex -= maxDelaySize;
        }
        
        writeIndex ++;
        if(writeIndex >= maxDelaySize)
        {
            writeIndex -= maxDelaySize;
        }
        
        return outputSample;
    }
    
    /** Calculate the empty space between buffer and find it estimation to reduce artifact**/
    float linearinterpolation()
    {
        int indexA = int(readIndex);
        int indexB = indexA + 1;
        
        if (indexB > maxDelaySize)
        {
            indexB -= maxDelaySize;
        }
        
        float valA = data[indexA];
        float valB = data[indexB];
        
        float remainder = readIndex - indexA; // indexA = int so that we can find the remainder.
        
        float interpolatedSample = (1 - remainder) * valA + remainder * valB;
        
        return interpolatedSample;
    }

private:
    
    float* data = nullptr; //dont let the variable point at anything
    
    int maxDelaySize;
    
    float readIndex;
    
    int writeIndex;
    
    int delayTime;
    
    float feedback = 0.5; // must be between 0-1 higher value longer the feedback e.g.0.99
};
