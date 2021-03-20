/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"
#include <JuceHeader.h>

//==============================================================================
/**
 */
class ModuleView : public juce::Component
{
public:
    ModuleView(juce::Colour color) : color_ {color} { }
    ~ModuleView() override = default;

    void paint(juce::Graphics& g) override { g.fillAll(color_); }

    // void mouseDown(juce::MouseEvent const& event) override;
    // void mouseDrag(juce::MouseEvent const& event) override;
    // void mouseUp(juce::MouseEvent const& event) override;

private:
    juce::Colour color_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleView)
};

class ModularAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    ModularAudioProcessorEditor(ModularAudioProcessor&);
    ~ModularAudioProcessorEditor() override = default;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    void mouseDown(juce::MouseEvent const& event) override;
    void mouseDrag(juce::MouseEvent const& event) override;
    void mouseUp(juce::MouseEvent const& event) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ModularAudioProcessor& audioProcessor;

    juce::Point<int> start_;
    juce::Point<int> end_;
    bool isDragging_ = false;

    float a_ = 1.0f;
    juce::Slider aSlider_;
    ModuleView module1_ {juce::Colours::red};
    ModuleView module2_ {juce::Colours::green};
    ModuleView module3_ {juce::Colours::blue};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModularAudioProcessorEditor)
};
