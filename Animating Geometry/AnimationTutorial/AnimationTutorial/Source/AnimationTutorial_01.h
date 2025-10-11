/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2020 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             AnimationTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Animates a fish.

 dependencies:     juce_core, juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2019, linux_make, xcode_iphone, androidstudio

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

//==============================================================================
class MainContentComponent   : public juce::AnimatedAppComponent
{
public:
    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 600);
        setFramesPerSecond(60); //Remember, constructor is the header [1] this sets the FPS of the animation and restart the re-paint process at that given frequency 
        
    }

    void update() override
    {
        // This function is called at the frequency specified by the setFramesPerSecond() call
        // in the constructor. You can use it to update counters, animate values, etc.
    }

    void paint (juce::Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
        g.setColour(getLookAndFeel().findColour(Slider::thumbColourId)); // [2] This sets the colour of the circle to be draw

        //Creating a path for the animation
        auto numberOfDots = 20; //Here we define the number of dots to create the path
        juce::Path spinePath; //Juce object to create open or closed shapes and uses coordinates to set starting and path points
        
        for (auto i = 0; i < numberOfDots; ++i) // For loop to create the Point object (offseting the animation for every next point)
        {
        //Define the radius of the circle to be draw
        auto radius = 100 + 10 * std::sin (getFrameCounter() * 0.1f + i * 0.5f); 
        
        //This "Point" is a 2D coordinate class, which represent the position of the center of the shape at the given timeframe (with an xy float precision)
        // So, this point is called "p" as the variable
        //
        //that is done by dividing the width and height of the screen by two
        //Then offset the x and y coordinates of the shape by adding the radius value to both of them 
        //The addition of (float) i * 0.12f, allows to update the position of the Point based on the numberOfDots for the spinePath
        juce::Point<float> p((float) getWidth() / 2.0f + 1.0f * radius * std::cos(getFrameCounter() * 0.04f +  i * 0.12f),(float) getHeight() / 2.0f + 1.0f * radius * std::sin (getFrameCounter() * 0.04f + i * 0.12f));

        //This will create circles on the points that are being connected
        g.fillEllipse(p.x - i, p.y - i, 2.0f + 2.0f * i, 2.0f + 2.0f * i); 

        if (i == 0)
            {
            spinePath.startNewSubPath(p); // If this is the first point, starts a new path
            }
        else
            {
            spinePath.lineTo(p); //If this is not the first point, add the next point
            }

        }
                
        //Draw an outline around the defined path that was created on the loop
        g.strokePath(spinePath, juce::PathStrokeType(10.0f));

        //Then Paint the actual circle with fillElipse() Function
        //g.fillEllipse(p.x, p.y, 30.0f, 30.0f);

    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
