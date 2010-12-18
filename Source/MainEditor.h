/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  18 Dec 2010 3:30:58am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_MAINEDITOR_MAINEDITOR_54B5D358__
#define __JUCER_HEADER_MAINEDITOR_MAINEDITOR_54B5D358__

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
    MainEditor (MainController *controller);
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
    static const char* granularbackdropcropped_png;
    static const int granularbackdropcropped_pngSize;
    static const char* offon_btn_idle_png;
    static const int offon_btn_idle_pngSize;
    static const char* offon_btn_press_png;
    static const int offon_btn_press_pngSize;
    static const char* open_btn_idle_png;
    static const int open_btn_idle_pngSize;
    static const char* open_btn_press_png;
    static const int open_btn_press_pngSize;
    static const char* play_btn_idle_png;
    static const int play_btn_idle_pngSize;
    static const char* play_btn_press_png;
    static const int play_btn_press_pngSize;
    static const char* record_btn_idle_png;
    static const int record_btn_idle_pngSize;
    static const char* record_btn_press_png;
    static const int record_btn_press_pngSize;

    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	//friend class MainController;
	friend class MainController;
	MainController *mMessageListener;

    //[/UserVariables]

    //==============================================================================
    ComboBox* mSliceChooserBox;
    Slider* mPanSlider;
    TextButton* mSaveFileButton;
    Slider* mGainSlider;
    Slider* mGrainLengthSlider;
    Slider* mGrainStartSlider;
    Slider* mGrainAdvanceSlider;
    Slider* mGrainVelocitySlider;
    Slider* mGrainLengthRandomSlider;
    Slider* mGrainStartRandomSlider;
    Slider* mGrainAdvanceRandomSlider;
    Slider* mVelocityFactorRandomSlider;
    ImageButton* mActiveButton;
    ImageButton* mMonoButton;
    ImageButton* mPlayButton;
    ImageButton* mOpenFileButton;
    ImageButton* mRecordButton;
    Image* cachedImage_granularbackdropcropped_png;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    MainEditor (const MainEditor&);
    const MainEditor& operator= (const MainEditor&);
};


#endif   // __JUCER_HEADER_MAINEDITOR_MAINEDITOR_54B5D358__
