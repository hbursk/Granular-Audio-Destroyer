/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  1 Dec 2010 11:30:39pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_9FE0D261__
#define __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_9FE0D261__

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "GranularSlice.h"
#include "MainEditor.h"
//[/Headers]

#define NUM_GRAINS 8

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainController  : public AudioIODeviceCallback,
					    public MessageListener

{
public:
    //==============================================================================
    MainController ();
    ~MainController();

    //==============================================================================
    //[UserMethods] -- You can add your own custom methods in this section.
	
	
	void playPressed();
	void openFilePressed();
	void saveFilePressed();
	bool isPlaying();
	
	//GUI parameters
	void  setCurrentSliceIndex(int index);
	int   getCurrentSliceIndex();
	void  setGrainGain(float nvalue);
	float getGrainGainSliderValue();
	float getGrainGain();
	void  setGrainPan(float pan);
	float getGrainPan();
	void  setGrainLength(float nvalue);
	int64 getGrainLength();
	float getGrainLengthSliderValue();
	void  setGrainStartPosition(float nvalue);
	int64 getGrainStartPosition();
	float getGrainStartPositionSliderValue();
	void  setGrainAdvanceAmount(float nvalue);
	int64 getGrainAdvanceAmount();
	float getGrainAdvanceAmountSliderValue();
	void  setGrainVelocityFactor(float nvalue);
	float getGrainVelocityFactor();
	float getGrainVelocityFactorSliderValue();
	void  setGrainStartRandomAmount(float nvalue);
	float getGrainStartRandomAmount();
	
	void playAudioFile(File &audioFile);
	void memStoreAudioFile(File &audioFile);
	void saveAudioFile(File &saveFile);
	void resetAudioRenderer();
	void setupGranularSlices(bool reset);
	bool renderAudioToBuffer(float** outputChannelData, int numOutputChannels, int numSamples);
	
	void updateGUIParameters();
	
	//MessageListener
	virtual void handleMessage(const Message &message);
	
	void setEditor(MainEditor *editor);
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();	

	
	void audioDeviceAboutToStart (AudioIODevice* device);
    void audioDeviceStopped();
    void audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
                                float** outputChannelData, int numOutputChannels, int numSamples);



    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	
	AudioDeviceManager mDeviceManager;

    AudioSourcePlayer mAudioSourcePlayer;
    AudioTransportSource mTransportSource;
    AudioFormatReaderSource* mCurrentAudioFileSource;
	
	File mCurrentFile;

	float samples [1024];
	float *mInterleavedBuffer;
	float *mLeftBuffer;
	float *mRightBuffer;
	int nextSample, subSample;
	int64 mBufferLength;
    float accumulator;
	int	  mNumChannels;
	int64 mGrainStartPositionAbsolute;
	int64 mGrainLength;
	int64 mGrainCurrentPositionRelativeLeft;
	int64 mGrainCurrentPositionRelativeRight;
	int64 mSampleCounter;
	float mVelocityFactor;
	int64 mGrainAdvanceAmount;
	bool  mPlaying;
	GranularSlice *mGranularSlices[NUM_GRAINS];
	
	int   mCurrentSliceIndex;
	
	MainEditor *mMainEditor;
	
    //[/UserVariables]

    //==============================================================================
	
    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    MainController (const MainController&);
    const MainController& operator= (const MainController&);
};


#endif   // __JUCER_HEADER_MAINCOMPONENT_MAINCOMPONENT_9FE0D261__
