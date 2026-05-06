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
class _2526HW4KeyAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    _2526HW4KeyAudioProcessorEditor (_2526HW4KeyAudioProcessor&);
    ~_2526HW4KeyAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    _2526HW4KeyAudioProcessor& audioProcessor;
    
    juce::GroupComponent delayGroup{ "delayGroup", "Delay" };
    juce::GroupComponent modGroup{ "modGroup",   "Modulation" };
    
    juce::Label title;
    juce::Label subtitle;
    
    // sliders
    juce::Slider delaySlider, mixSlider, feedbackSlider;
    juce::Slider rateSlider, depthSlider;

    void styleSlider(juce::Slider& s);
    void styleLabel(juce::Label& l, const juce::String& text);
    void layoutKnob(juce::Slider& slider, juce::Label& label, juce::Rectangle<int> area);

    // button
    juce::TextButton modButton { "LFO ON" };

    // attachments
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<SliderAttachment> delayAttach, mixAttach, feedbackAttach;
    std::unique_ptr<SliderAttachment> rateAttach, depthAttach;
    std::unique_ptr<ButtonAttachment> modAttach;

    // labels
    juce::Label delayLabel, mixLabel, feedbackLabel;
    juce::Label rateLabel, depthLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_2526HW4KeyAudioProcessorEditor)
};

namespace ColoursTheme
{
    const juce::Colour backgroundTop = juce::Colour(0xff151326);
    const juce::Colour backgroundBottom = juce::Colour(0xff080a12);
    const juce::Colour panel = juce::Colour(0xff202033).withAlpha(0.92f);
    const juce::Colour panelShadow = juce::Colour(0xff050509).withAlpha(0.45f);
    const juce::Colour accent = juce::Colour(0xffff7a90); // coral pink
    const juce::Colour accentTwo = juce::Colour(0xff7ee7ff); // cyan
    const juce::Colour text = juce::Colour(0xfff7f3ff);
    const juce::Colour mutedText = juce::Colour(0xffaaa6c3);
    const juce::Colour knob = juce::Colour(0xff34344f);
    const juce::Colour textbox = juce::Colour(0xff12121f);
}
