/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_2526HW4KeyAudioProcessorEditor::_2526HW4KeyAudioProcessorEditor (_2526HW4KeyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (720, 520);
    
    // get apvts ref
    auto& apvts = audioProcessor.apvts;

    // quick set up for sliders
    auto setupSlider = [this](juce::Slider& s)
        {
            s.setSliderStyle(juce::Slider::Rotary);
            s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
            s.setRotaryParameters(juce::MathConstants<float>::pi * 1.25f,
                                  juce::MathConstants<float>::pi * 2.75f,
                                  true);
            styleSlider(s);
        };

    setupSlider(delaySlider);
    setupSlider(mixSlider);
    setupSlider(feedbackSlider);
    setupSlider(rateSlider);
    setupSlider(depthSlider);
    
    rateSlider.setColour(juce::Slider::rotarySliderFillColourId, ColoursTheme::accentTwo);
    rateSlider.setColour(juce::Slider::thumbColourId, ColoursTheme::accentTwo);

    depthSlider.setColour(juce::Slider::rotarySliderFillColourId, ColoursTheme::accentTwo);
    depthSlider.setColour(juce::Slider::thumbColourId, ColoursTheme::accentTwo);
    
    delaySlider.setTextValueSuffix(" s");
    rateSlider.setTextValueSuffix(" Hz");
    depthSlider.setTextValueSuffix(" s");
    
    title.setText("MOD DELAY", juce::dontSendNotification);
    title.setJustificationType(juce::Justification::centred);
    title.setColour(juce::Label::textColourId, ColoursTheme::text);
    title.setFont(juce::FontOptions{}.withHeight(45.0f).withStyle("Bold"));
    
    addAndMakeVisible(title);
    
    delayGroup.setColour(juce::GroupComponent::outlineColourId, ColoursTheme::accent.withAlpha(0.65f));
    delayGroup.setColour(juce::GroupComponent::textColourId, ColoursTheme::text);
    modGroup.setColour(juce::GroupComponent::outlineColourId, ColoursTheme::accentTwo.withAlpha(0.65f));
    modGroup.setColour(juce::GroupComponent::textColourId, ColoursTheme::text);

    // components
    addAndMakeVisible(delayGroup);
    addAndMakeVisible(modGroup);

    addAndMakeVisible(delaySlider);
    addAndMakeVisible(mixSlider);
    addAndMakeVisible(feedbackSlider);

    addAndMakeVisible(rateSlider);
    addAndMakeVisible(depthSlider);
    addAndMakeVisible(modButton);

    styleLabel(delayLabel, "Time");
    styleLabel(mixLabel, "Dry / Wet");
    styleLabel(feedbackLabel, "Feedback");
    styleLabel(rateLabel, "LFO Rate");
    styleLabel(depthLabel, "LFO Depth");

    addAndMakeVisible(delayLabel);
    addAndMakeVisible(mixLabel);
    addAndMakeVisible(feedbackLabel);
    addAndMakeVisible(rateLabel);
    addAndMakeVisible(depthLabel);
    
    modButton.setButtonText("LFO ON");
    modButton.setClickingTogglesState(true);

    modButton.setClickingTogglesState(true);

    modButton.setColour(juce::TextButton::buttonColourId, ColoursTheme::knob);
    modButton.setColour(juce::TextButton::buttonOnColourId, ColoursTheme::accentTwo);
    modButton.setColour(juce::TextButton::textColourOffId, ColoursTheme::text);
    modButton.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    
    // attachments
    delayAttach = std::make_unique<SliderAttachment>(apvts, "delay", delaySlider);
    mixAttach = std::make_unique<SliderAttachment>(apvts, "mix", mixSlider);
    feedbackAttach = std::make_unique<SliderAttachment>(apvts, "feedback", feedbackSlider);

    rateAttach = std::make_unique<SliderAttachment>(apvts, "rate", rateSlider);
    depthAttach = std::make_unique<SliderAttachment>(apvts, "depth", depthSlider);

    modAttach = std::make_unique<ButtonAttachment>(apvts, "modOn", modButton);
}

_2526HW4KeyAudioProcessorEditor::~_2526HW4KeyAudioProcessorEditor()
{
}

//==============================================================================
void _2526HW4KeyAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    juce::ColourGradient background(ColoursTheme::backgroundTop, 0.0f, 0.0f,
                                    ColoursTheme::backgroundBottom, 0.0f, bounds.getBottom(), false);
    g.setGradientFill(background);
    g.fillRect(bounds);
    
    auto area = getLocalBounds().reduced(22);
    area.removeFromTop(58);

    auto delayPanel = area.removeFromTop(200).toFloat();
    area.removeFromTop(30);
    auto modulationPanel = area.removeFromTop(200).toFloat();
    
    g.setColour(ColoursTheme::panelShadow);
    g.fillRoundedRectangle(delayPanel.translated(0.0f, 5.0f), 18.0f);
    g.fillRoundedRectangle(modulationPanel.translated(0.0f, 5.0f), 18.0f);

    g.setColour(ColoursTheme::panel);
    g.fillRoundedRectangle(delayPanel, 18.0f);
    g.fillRoundedRectangle(modulationPanel, 18.0f);

    g.setColour(ColoursTheme::accent.withAlpha(0.18f));
    g.fillRoundedRectangle(delayPanel.reduced(1.0f), 18.0f);

    g.setColour(ColoursTheme::accentTwo.withAlpha(0.14f));
    g.fillRoundedRectangle(modulationPanel.reduced(1.0f), 18.0f);
}

void _2526HW4KeyAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(22);

    auto header = area.removeFromTop(58);
    title.setBounds(header.removeFromTop(32));
    subtitle.setBounds(header);

    auto delayPanel = area.removeFromTop(200);
    area.removeFromTop(30);
    auto modulationPanel = area.removeFromTop(200);

    delayGroup.setBounds(delayPanel);
    auto delayArea = delayPanel.reduced(18, 30);
    delayArea.removeFromTop(8);

    auto knobWidth = delayArea.getWidth() / 3;
    layoutKnob(delaySlider, delayLabel, delayArea.removeFromLeft(knobWidth).reduced(8, 0));
    layoutKnob(mixSlider, mixLabel, delayArea.removeFromLeft(knobWidth).reduced(8, 0));
    layoutKnob(feedbackSlider, feedbackLabel, delayArea.reduced(8, 0));

    modGroup.setBounds(modulationPanel);
    auto modArea = modulationPanel.reduced(24, 26);
    modArea.removeFromTop(8);

    auto buttonArea = modArea.removeFromRight(150).reduced(12, 45);
    modButton.setBounds(buttonArea);

    auto modKnobWidth = modArea.getWidth() / 2;
    layoutKnob(rateSlider, rateLabel, modArea.removeFromLeft(modKnobWidth).reduced(8, 0));
    layoutKnob(depthSlider, depthLabel, modArea.reduced(8, 0));
}
void _2526HW4KeyAudioProcessorEditor::styleSlider(juce::Slider& s)
{
    s.setColour(juce::Slider::rotarySliderFillColourId, ColoursTheme::accent);
    s.setColour(juce::Slider::thumbColourId, ColoursTheme::accent);
    s.setColour(juce::Slider::rotarySliderOutlineColourId, ColoursTheme::knob);
    s.setColour(juce::Slider::textBoxTextColourId, ColoursTheme::text);
    s.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
}

void _2526HW4KeyAudioProcessorEditor::styleLabel(juce::Label& l, const juce::String& text)
{
    l.setText(text, juce::dontSendNotification);
    l.setJustificationType(juce::Justification::centred);
    l.setColour(juce::Label::textColourId, ColoursTheme::text);
    l.setFont(juce::FontOptions{}.withHeight(18.0f));
}

void _2526HW4KeyAudioProcessorEditor::layoutKnob(juce::Slider& slider, juce::Label& label, juce::Rectangle<int> area)
{
    label.setBounds(area.removeFromTop(22));
    area.removeFromTop(4);
    slider.setBounds(area);
}
