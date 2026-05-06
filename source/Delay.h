/*
  ==============================================================================

    Delay.h
 
 This code contains the implementation needed for a simple feedback delay.

  ==============================================================================
*/
#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class Delay
{

public:
    void prepare(double samplingRate, int maxDelayInSamples, int numChannels);

    float processSample(float inputSample, int channel);

    void setMaxDelayInSamples(int maxDelayInSamples);

    int getMaxDelayInSamples();

    void setDelayLength(float delayInSamples);

    void setWetMix(float wetAmt);
    
    void setDelayTime(float delaySecond);
    
    void setFeedbackAmt(float feedbackAmt);

    void setModRate(float rateHz);

    void setModDepth(float depthSecond);

    void setModTrue(bool onValue);

    void setModValue(bool onValue);

private:

    juce::AudioBuffer<float> delayBuffer;

    std::vector<int> writeHeads;
    
    float interpRead(float* delayData, int writeHead, float delaySamples);

    float delaySamples = 0;
    float delaySeconds = 0;
    int maxDelayInSamples = 0;
    int delayBufferSize = 0;
    
    juce::SmoothedValue<float> smoothDelay;
    float currDelay;

    double sampleRate;
    float mix = 0.5;
    float feedback = 0.2;

    void nextLfoVal();

    bool modOn = false;
    float phase = 0;
    float freq = 0.5;
    float amp = 0.1;
    float lfo = 0;
    float rate = 0;
    float depth = 0;
};
