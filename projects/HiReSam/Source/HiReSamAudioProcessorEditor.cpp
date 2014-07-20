/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "HiReSamAudioProcessor.h"
#include "HiReSamAudioProcessorEditor.h"


//==============================================================================
HiReSamAudioProcessorEditor::HiReSamAudioProcessorEditor (HiReSamAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      spectroscope(11), // FFT Size of 2^11 = 2048
      renderThread("FFT Render Thread")

{
    // The plugin's initial editor size.
    setSize (1000, 400);
    
    sampleRate.addListener (this);
    // The sampleRate has already been set in the HiReSamAudioProcessor before
    // the creation of the HiReSamAudioProcessorEditor.
    // Because of this it is important to have the referTo call after the
    // addListener. Only in this order the valueChanged member function
    // will be called implicitly.
    sampleRate.referTo (getProcessor()->sampleRate);
    
    renderThread.addTimeSliceClient (&spectroscope);
    renderThread.startThread (3);
    
    header.setText("High Resolution Spectrum Analyse Meter", dontSendNotification);
    Font headerFont = Font (18.0f);
    header.setFont (headerFont);
    header.setJustificationType(Justification::centred);
    header.setColour (Label::textColourId, Colours::lightgoldenrodyellow);
    header.setColour (Label::backgroundColourId, Colours::black);
    
    samWithBubble.referToFrequencyTextValue (pitchDetector.getPitchTextValue());
    
    addAndMakeVisible (&header);
    addAndMakeVisible (&spectroscope);
    addAndMakeVisible (&pitchDetector);
    addAndMakeVisible (&samWithBubble);
}

HiReSamAudioProcessorEditor::~HiReSamAudioProcessorEditor()
{
    renderThread.removeTimeSliceClient (&spectroscope);
    renderThread.stopThread (500);
}

//==============================================================================
void HiReSamAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void HiReSamAudioProcessorEditor::resized()
{
    const int minWidth = 360;
    const int minHeight = 320;
    if (getWidth() < minWidth || getHeight() < minHeight)
    {
        setSize (jmax (minWidth, getWidth()), jmax (minHeight, getHeight()));
    }
    
    header.setBounds(0, 0, getWidth(), 24);
    
    const int minimalWithForSpectroscope = 120;
    const int widthForSamWithBubble = jmin (320, getWidth() - minimalWithForSpectroscope);
    spectroscope.setBounds (0, header.getHeight(), getWidth() - widthForSamWithBubble, getHeight() - header.getHeight());
    pitchDetector.setBounds (0, header.getHeight(), spectroscope.getWidth(), getHeight() - header.getHeight() - spectroscope.getHeightOfFrequencyCaption());
    
    const int maxHeight = getHeight() - header.getHeight();
    const int height = jmin (360, maxHeight);
    samWithBubble.setBounds(spectroscope.getWidth(), header.getHeight() + 0.5f * (maxHeight - height), widthForSamWithBubble, height);
}

void HiReSamAudioProcessorEditor::valueChanged (Value & value)
{
    if (value.refersToSameSourceAs (sampleRate))
    {
        spectroscope.setSampleRate (sampleRate.getValue());
        pitchDetector.setSampleRate (sampleRate.getValue());
    }
}

HiReSamAudioProcessor* HiReSamAudioProcessorEditor::getProcessor() const
{
    return static_cast <HiReSamAudioProcessor*> (getAudioProcessor());
}
