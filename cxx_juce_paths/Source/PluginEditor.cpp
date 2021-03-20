#include "PluginProcessor.h"

#include "PluginEditor.h"

ModularAudioProcessorEditor::ModularAudioProcessorEditor(
    ModularAudioProcessor& p)
    : juce::AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(500, 400);

    addAndMakeVisible(aSlider_);
    aSlider_.setValue(1.0);
    aSlider_.setRange(0.01, 2.0, 0.001);
    aSlider_.onValueChange = [this] {
        a_ = aSlider_.getValue();
        repaint();
    };

    // addAndMakeVisible(module1_);
    // addAndMakeVisible(module2_);
    // addAndMakeVisible(module3_);
}

void ModularAudioProcessorEditor::paint(juce::Graphics& g)
{
    auto colorID = juce::ResizableWindow::backgroundColourId;
    g.fillAll(getLookAndFeel().findColour(colorID));

    if (isDragging_)
    {
        g.setColour(juce::Colours::white);
        g.drawLine(juce::Line {start_, end_}.toFloat(), 10.0f);
        g.setColour(juce::Colours::red);
    }

    auto a    = a_ * (-1);
    auto b    = 2;
    auto c    = 2;
    auto f    = [a, b, c](auto x) { return a * (x * x) + (b * x) + c; };
    auto path = juce::Path {};
    path.startNewSubPath(-400, f(-400));
    for (auto x = -399; x < 400; x += 1)
    {
        path.lineTo(x + 250, f(x) + 300);
    }

    g.strokePath(path, juce::PathStrokeType(5.0f));
}

void ModularAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    aSlider_.setBounds(area.removeFromTop(50));
    // auto moduleWidth = area.getWidth() / 3;
    // module1_.setBounds(area.removeFromLeft(moduleWidth));
    // module2_.setBounds(area.removeFromLeft(moduleWidth));
    // module3_.setBounds(area.removeFromLeft(moduleWidth));
}

void ModularAudioProcessorEditor::mouseDown(juce::MouseEvent const& event) { }

void ModularAudioProcessorEditor::mouseDrag(juce::MouseEvent const& event)
{
    start_.x    = event.mouseDownPosition.x;
    start_.y    = event.mouseDownPosition.y;
    end_.x      = event.x;
    end_.y      = event.y;
    isDragging_ = true;
    repaint();
}
void ModularAudioProcessorEditor::mouseUp(juce::MouseEvent const& event)
{
    isDragging_ = false;
    repaint();
}