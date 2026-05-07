/*
  ==============================================================================

    Delay.cpp
 
    This code contains the implementation needed for a simple feedback delay.

  ==============================================================================
*/

#include "Delay.h"

void Delay::prepare(double samplingRate, int maxDelay, int numChannels)
{
    sampleRate = samplingRate;

    delayBufferSize = maxDelay;

    delayBuffer.setSize(numChannels, delayBufferSize);
    delayBuffer.clear();

    writeHeads.resize(numChannels);
    for (int c = 0; c < numChannels; ++c)
    {
        writeHeads[c] = 0;
    }
}

void Delay::setMaxDelayInSamples(int maxDelay)
{
    maxDelayInSamples = maxDelay;
}

int Delay::getMaxDelayInSamples()
{
    return maxDelayInSamples;
}

void Delay::setDelayLength(float delayInSamples)
{
    delaySamples = delayInSamples;
}

void Delay::setDelayTime(float delaySecond)
{
    delaySeconds = delaySecond;
}

void Delay::setWetMix(float wetAmount)
{
    mix = wetAmount;
}

void Delay::setFeedbackAmt(float feedbackAmt)
{
    feedback = feedbackAmt;
}

void Delay::setModRate(float rateHz)
{
    rate = rateHz;
}

void Delay::setModDepth(float depthSecond)
{
    depth = depthSecond;
}

void Delay::setModValue(bool onValue)
{
    modOn = onValue;
}

void Delay::nextLfoVal()
{
    lfo = depth * getLfoValue();

    float twoPi = 2.0f * juce::MathConstants<float>::pi;

    phase += twoPi * rate / sampleRate;

    if (phase >= twoPi)
    {
        phase -= twoPi;
    }
}

float Delay::getLfoValue()
{
    switch (lfoShape)
    {
        case 0: // sine
            return sinf(phase);

        case 1: // triangle
            return (2.0f / juce::MathConstants<float>::pi) * asinf(sinf(phase));

        case 2: // square
            return sinf(phase) >= 0.0f ? 1.0f : -1.0f;

        default:
            return sinf(phase);
    }
}

void Delay::setLfoShape(int shape)
{
    lfoShape = shape;
}

float Delay::interpRead(float* delayData, int writeHead, float delaySamples)
{
    float readTail = writeHead - delaySamples;
    while (readTail < 0)
        readTail += delayBufferSize;
    readTail = std::fmod(readTail, (float)delayBufferSize);

    int before = floor(readTail);
    int after = (before + 1) % delayBufferSize;

    float frac = readTail - before;

    return (delayData[before] * (1 - frac)) + (delayData[after] * frac);
}

float Delay::processSample(float inputSample, int channel)
{
    float* delayData = delayBuffer.getWritePointer(channel);

    int writeHead = writeHeads[channel];
    if (channel == 0)
    {
        if (modOn)
            nextLfoVal();
        else
            lfo = 0.0f;
    }

    float modDelay = delaySeconds + lfo;

    modDelay = std::clamp<float>(modDelay, 0.001f, (delayBufferSize / sampleRate));
    delaySamples = modDelay * sampleRate;
    
    float delaySample = interpRead(delayData, writeHead, delaySamples);

    delayData[writeHead] = inputSample + (feedback * delaySample);
    
    float delayed = (1.0f - mix) * inputSample + mix * delaySample;

    writeHead = (writeHead + 1) % delayBufferSize;

    writeHeads[channel] = writeHead;

    return delayed;
}
