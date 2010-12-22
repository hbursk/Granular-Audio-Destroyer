/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  22 Dec 2010 3:14:29am

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_MAINEDITOR_MAINEDITOR_78631152__
#define __JUCER_HEADER_MAINEDITOR_MAINEDITOR_78631152__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformView.h"
class MainController;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainEditor  : public Component,
                    public SliderListener,
                    public ButtonListener
{
public:
    //=============================================================================

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    MainEditor (MainController *controller);
    ~MainEditor();

    //==============================================================================

	virtual void handleMessage(const Message &message);
	void selectRadioPressed(int index);
	void setupWaveformView(File& file);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);

    // Binary resources:
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
    static const char* _1_btn_idle_png;
    static const int _1_btn_idle_pngSize;
    static const char* _1_btn_press_png;
    static const int _1_btn_press_pngSize;
    static const char* _2_btn_idle_png;
    static const int _2_btn_idle_pngSize;
    static const char* _2_btn_press_png;
    static const int _2_btn_press_pngSize;
    static const char* _3_btn_idle_png;
    static const int _3_btn_idle_pngSize;
    static const char* _3_btn_press_png;
    static const int _3_btn_press_pngSize;
    static const char* _4_btn_idle_png;
    static const int _4_btn_idle_pngSize;
    static const char* _4_btn_press_png;
    static const int _4_btn_press_pngSize;
    static const char* _5_btn_idle_png;
    static const int _5_btn_idle_pngSize;
    static const char* _5_btn_press_png;
    static const int _5_btn_press_pngSize;
    static const char* _6_btn_idle_png;
    static const int _6_btn_idle_pngSize;
    static const char* _6_btn_press_png;
    static const int _6_btn_press_pngSize;
    static const char* _7_btn_idle_png;
    static const int _7_btn_idle_pngSize;
    static const char* _7_btn_press_png;
    static const int _7_btn_press_pngSize;
    static const char* _8_btn_idle_png;
    static const int _8_btn_idle_pngSize;
    static const char* _8_btn_press_png;
    static const int _8_btn_press_pngSize;
    static const char* offon_mini_idle_png;
    static const int offon_mini_idle_pngSize;
    static const char* offon_mini_press_png;
    static const int offon_mini_press_pngSize;
    static const char* screentoggle_btn_press_png;
    static const int screentoggle_btn_press_pngSize;
    static const char* screentoggle_btn_idle_png;
    static const int screentoggle_btn_idle_pngSize;

    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	//friend class MainController;
	friend class MainController;
	MainController *mMessageListener;
	WaveformView *mWaveformView;
    //[/UserVariables]

    //==============================================================================
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
    ImageButton* mReverseButton;
    ImageButton* mMonoButton;
    ImageButton* mPlayButton;
    ImageButton* mOpenFileButton;
    ImageButton* mRecordButton;
    ImageButton* mActiveButton1;
    ImageButton* mActiveButton2;
    ImageButton* mActiveButton3;
    ImageButton* mActiveButton4;
    ImageButton* mActiveButton5;
    ImageButton* mActiveButton6;
    ImageButton* mActiveButton7;
    ImageButton* mActiveButton8;
    ImageButton* mSelectButton1;
    ImageButton* mSelectButton2;
    ImageButton* mSelectButton3;
    ImageButton* mSelectButton4;
    ImageButton* mSelectButton5;
    ImageButton* mSelectButton6;
    ImageButton* mSelectButton7;
    ImageButton* mSelectButton8;
    Image* cachedImage_granularbackdropcropped_png;

    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    MainEditor (const MainEditor&);
    const MainEditor& operator= (const MainEditor&);
};


#endif   // __JUCER_HEADER_MAINEDITOR_MAINEDITOR_78631152__
