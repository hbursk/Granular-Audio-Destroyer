/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  8 Dec 2010 6:42:00pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_MAINEDITOR_MAINEDITOR_F1FE5D23__
#define __JUCER_HEADER_MAINEDITOR_MAINEDITOR_F1FE5D23__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
//[/Headers]

class MainController;

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainEditor  : public Component,
                    public ComboBoxListener,
                    public SliderListener,
                    public ButtonListener
{
public:
    //==============================================================================
    MainEditor (MainController *messageListener);
    ~MainEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	virtual void handleMessage(const Message &message);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);

    // Binary resources:
    static const char* backdropgrid_png;
    static const int backdropgrid_pngSize;

    //==============================================================================
    juce_UseDebuggingNewOperator

private:
	friend class MainController;
    //[UserVariables]   -- You can add your own custom variables in this section.
	MainController *mMessageListener;

    //[/UserVariables]

    //==============================================================================
    ComboBox* mSliceChooserBox;
    Slider* mGainSlider;
    Label* mGainLabel;
    Slider* mPanSlider;
    Label* mPanLabel;
    Slider* mGrainLengthSlider;
    Label* mGrainSizeLabel;
    Label* mGainLabel2;
    Slider* mGrainStartSlider;
    Label* mGrainStartLabel;
    Slider* mGrainAdvanceSlider;
    Label* mGrainAdvanceLabel;
    Slider* mGrainVelocitySlider;
    Label* mGrainVelocityLabel;
    TextButton* mOpenFileButton;
    TextButton* mSaveFileButton;
    TextButton* mPlayButton;
    Image* cachedImage_backdropgrid_png;
	
    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    MainEditor (const MainEditor&);
    const MainEditor& operator= (const MainEditor&);
};


#endif   // __JUCER_HEADER_MAINEDITOR_MAINEDITOR_F1FE5D23__
