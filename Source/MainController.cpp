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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "MainController.h"
#include "ParameterMessage.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]
const float kMinGain = 0.0f;
const float kMaxGain = 4.0f;
const int64 kMinGrainLength = 220;
const int64 kMaxGrainLength = 44100*2;
const int64 kMinAdvance = 0;
const int64 kMaxAdvance = 44100*2;
const float kMinVelocity = 0.0f;
const float kMaxVelocity = 3.0f;



//==============================================================================
MainController::MainController ()
    : mMainEditor (0),
	  mCurrentAudioFileSource(0),
	  mInterleavedBuffer(0),
	  mLeftBuffer(0),
	  mRightBuffer(0)
{

    //[UserPreSize]	
	mDeviceManager.initialise (2, 2, 0, true, String::empty, 0);
	mDeviceManager.addAudioCallback (&mAudioSourcePlayer);
    mAudioSourcePlayer.setSource (&mTransportSource);
    mCurrentAudioFileSource = 0;
    //[/UserPreSize]

    //[Constructor] You can add your own custom stuff here..
	//setup Grain variables for testing
	mCurrentSliceIndex = 0;
	for (int i=0; i< NUM_GRAINS; i++)
	{
		mGranularSlices[i] = 0;
	}
	setupGranularSlices(true);
    //[/Constructor]
}

MainController::~MainController()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]


    //[/Destructor_pre]

    //[Destructor]. You can add your own custom destruction code here..
	mTransportSource.setSource (0);
    mAudioSourcePlayer.setSource (0);

    mDeviceManager.removeAudioCallback (&mAudioSourcePlayer);

    deleteAndZero (mCurrentAudioFileSource);
	if (mLeftBuffer != 0)
	{
		free(mLeftBuffer);
		mLeftBuffer = 0;
	}
	if (mRightBuffer != 0)
	{
		free(mRightBuffer);
		mRightBuffer = 0;
	}
	
	for (int i=0; i< NUM_GRAINS; i++)
	{
		if (mGranularSlices[i] != 0)
		{
			delete mGranularSlices[i];
			mGranularSlices[i] = 0;
		}
	}
    //[/Destructor]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
#pragma mark GUI Button Calls
void MainController::playPressed()
{
	if (mLeftBuffer == 0)
		return;
	if (mPlaying)
	{
		mDeviceManager.removeAudioCallback(this);
		mPlaying = false;
		resetAudioRenderer();
	}
	else
	{
		resetAudioRenderer();
		mDeviceManager.addAudioCallback(this);
		mPlaying = true;
	}
}

void MainController::openFilePressed()
{
	if (mPlaying)
	{
		mPlaying = false;
		mDeviceManager.removeAudioCallback(this);
	}
	WildcardFileFilter wildcardFilter ("*.wav","", "Wave files");

	FileBrowserComponent browser (FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles,
								  File::nonexistent,
								  &wildcardFilter,
								  0);

	FileChooserDialogBox dialogBox ("Open a Wave File",
									"Please choose a Wave file to open...",
									browser, true,
									Colour (0xc65e5c57));

	if (dialogBox.show())
	{
		File mCurrentFile = browser.getSelectedFile(0);
		memStoreAudioFile(mCurrentFile);
		mMainEditor->setupWaveformView(mCurrentFile);
		//mDeviceManager.addAudioCallback(this);
	}
}

void MainController::saveFilePressed()
{
	if (mPlaying)
	{
		mPlaying = false;
		mDeviceManager.removeAudioCallback(this);
	}
	if (mLeftBuffer == 0)
		return;
	WildcardFileFilter wildcardFilter ("*.wav","", "Wave files");

	FileBrowserComponent browser (FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles,
								  File::nonexistent,
								  &wildcardFilter,
								  0);

	FileChooserDialogBox dialogBox ("Save a Wave File",
									"Please choose a Wave file to save...",
									browser, true,
									Colours::lightgrey);

	if (dialogBox.show())
	{
		File savefile = browser.getSelectedFile(0);
		saveAudioFile(savefile);		
	}
}

bool MainController::isPlaying()
{
	return mPlaying;
}

#pragma mark GUI Parameter Functions
//GUI parameters will all receive between 0.0f-1.0f for slider values
//Up to this class to convert them and set them to usable values
void  MainController::setCurrentSliceIndex(int index)
{
	if (index > (NUM_GRAINS - 1))
		index = 0;
	mCurrentSliceIndex = index;
}

int   MainController::getCurrentSliceIndex()
{
	return mCurrentSliceIndex;
}

void  MainController::setGrainGain(float nvalue)
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return;
	float gainvalue = nvalue*(kMaxGain - kMinGain) + kMinGain;
	mGranularSlices[mCurrentSliceIndex]->setGain(gainvalue);
}

float MainController::getGrainGainSliderValue()
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return 0.5f;
	float gainvalue = getGrainGain();
	float slidervalue = (gainvalue - kMinGain)/(kMaxGain - kMinGain);
	return slidervalue;
}

float MainController::getGrainGain()
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return 1.0f;
	return mGranularSlices[mCurrentSliceIndex]->getGain();
}

void  MainController::setGrainPan(float pan)
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return;
	mGranularSlices[mCurrentSliceIndex]->setPan(pan);
}

float MainController::getGrainPan()
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return 0.5f;
	return mGranularSlices[mCurrentSliceIndex]->getPan();
}

void  MainController::setGrainLength(float nvalue)
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return;
	int64 maxlength = kMaxGrainLength;
	if (maxlength >= (int64)(mGranularSlices[mCurrentSliceIndex]->getDataLength()/2.0f))
		maxlength = (int64)(mGranularSlices[mCurrentSliceIndex]->getDataLength()/2.0f) - 1;
	float length = nvalue*(maxlength - kMinGrainLength) + kMinGrainLength;
	mGranularSlices[mCurrentSliceIndex]->setGrainLength(length);
}

int64 MainController::getGrainLength()
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return kMinGrainLength;
	return mGranularSlices[mCurrentSliceIndex]->getGrainLength();
}

float MainController::getGrainLengthSliderValue()
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return 0.0f;
	int64 maxlength = kMaxGrainLength;
	if (maxlength >= (int64)(mGranularSlices[mCurrentSliceIndex]->getDataLength()/2.0f))
		maxlength = (int64)(mGranularSlices[mCurrentSliceIndex]->getDataLength()/2.0f) - 1;
	float length = getGrainLength();
	float slidervalue = (length - kMinGrainLength)/(maxlength - kMinGrainLength);
	return slidervalue;
}

void  MainController::setGrainStartPosition(float nvalue)
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return;
	if (nvalue == 1.0f)
		nvalue = 0.999f;
	int64 startpos = (int64)(nvalue*(1.0f*mGranularSlices[mCurrentSliceIndex]->getDataLength()/2.0f));
	mGranularSlices[mCurrentSliceIndex]->setGrainStartPosition(startpos);
}

int64 MainController::getGrainStartPosition()
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return 0.0f;
	return mGranularSlices[mCurrentSliceIndex]->getGrainStartPosition();
}

float MainController::getGrainStartPositionSliderValue()
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return 0.0f;
	int64 startpos = getGrainStartPosition();
	float slidervalue = (1.0f*startpos)/(1.0f*mGranularSlices[mCurrentSliceIndex]->getDataLength()/2.0f);
	return slidervalue;
}

void  MainController::setGrainAdvanceAmount(float nvalue)
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return;
	int64 maxadvance = kMaxAdvance;
	if (maxadvance >= (int64)(mGranularSlices[mCurrentSliceIndex]->getDataLength()/2.0f))
		maxadvance = (int64)(mGranularSlices[mCurrentSliceIndex]->getDataLength()/2.0f) - 1;
	int64 advanceamount = (int64)(nvalue*(maxadvance - kMinAdvance) + kMinAdvance);
	mGranularSlices[mCurrentSliceIndex]->setGrainAdvanceAmount(advanceamount);
}

int64 MainController::getGrainAdvanceAmount()
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return 0;
	return mGranularSlices[mCurrentSliceIndex]->getGrainAdvanceAmount();
}

float MainController::getGrainAdvanceAmountSliderValue()
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return 0.0f;
	int64 maxadvance = kMaxAdvance;
	if (maxadvance >= (int64)(mGranularSlices[mCurrentSliceIndex]->getDataLength()/2.0f))
		maxadvance = (int64)(mGranularSlices[mCurrentSliceIndex]->getDataLength()/2.0f) - 1;
	float advanceamount = (float)getGrainAdvanceAmount();
	float slidervalue = (1.0f*(advanceamount - kMinAdvance))/(1.0f*(maxadvance - kMinAdvance));
	return slidervalue;
}

void  MainController::setGrainVelocityFactor(float nvalue)
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return;
	float velocity = (nvalue*(kMaxVelocity - kMinVelocity) + kMinVelocity);
	mGranularSlices[mCurrentSliceIndex]->setVelocity(velocity);
}

float MainController::getGrainVelocityFactor()
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return 0.0f;
	return mGranularSlices[mCurrentSliceIndex]->getVelocity();
}

float MainController::getGrainVelocityFactorSliderValue()
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return 0.0f;
	float velocity = getGrainVelocityFactor();
	float slidervalue = (velocity - kMinVelocity)/(kMaxVelocity - kMinVelocity);
	return slidervalue;
}

void  MainController::setGrainStartRandomAmount(float nvalue)
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return;
	mGranularSlices[mCurrentSliceIndex]->setRandomReadFactor(nvalue);
}

float MainController::getGrainStartRandomAmount()
{
	if (mGranularSlices[mCurrentSliceIndex] == 0)
		return 0.0f;
	return mGranularSlices[mCurrentSliceIndex]->getGrainStartRandomFactor();
}

#pragma mark Custom Audio Functions
void MainController::memStoreAudioFile(File &audioFile)
{
	//we only want raw samples, no header
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();
	AudioFormatReader *reader = formatManager.createReaderFor(audioFile);
	if (reader != 0)
	{
		if (mLeftBuffer != 0)
		{
			free(mLeftBuffer);
			mLeftBuffer = 0;
		}
		if (mRightBuffer != 0)
		{
			free(mRightBuffer);
			mRightBuffer = 0;
		}
		int num = reader->numChannels;
		int* destBufs[num];
		mLeftBuffer = (float*)malloc((reader->lengthInSamples/reader->numChannels*sizeof(float))+88200*sizeof(float)); //add some extra zeros at the end for safety
		destBufs[0] = (int*)mLeftBuffer;
		if (reader->numChannels > 1)
		{
			mRightBuffer = (float*)malloc((reader->lengthInSamples/reader->numChannels*sizeof(float))+88200*sizeof(float)); //add some extra zeros at the end for safety
			destBufs[1] = (int*)mRightBuffer;
		}
		reader->read(destBufs, reader->numChannels, 0, reader->lengthInSamples/reader->numChannels, false);
		mBufferLength = reader->lengthInSamples;
		mNumChannels = reader->numChannels;
		
		for (int i=0; i<(mBufferLength/mNumChannels); i++)
		{
			//scale to -1.0 to 1.0 floats
			mLeftBuffer[i] = ((float)destBufs[0][i])/(1.0f*INT_MAX);
			if (mRightBuffer != 0)
				mRightBuffer[i] = ((float)destBufs[1][i])/(1.0f*INT_MAX);
		}
		delete reader;
		
		setupGranularSlices(false);
	}
}

void MainController::setupGranularSlices(bool reset)
{
	for (int i=0; i< NUM_GRAINS; i++)
	{
		
		int64 bufferlength = mBufferLength;
		if (bufferlength == 0)
			bufferlength = 44100;
		int numchannels = mNumChannels;
		if (numchannels == 0)
			numchannels = 2;
		if (mGranularSlices[i] == 0)
		{
			mGranularSlices[i] = new GranularSlice(mLeftBuffer, mRightBuffer, bufferlength, numchannels, true);
		}
		else
		{
			mGranularSlices[i]->setData(mLeftBuffer, mRightBuffer, bufferlength, numchannels, reset);
		}
	}
}

void MainController::saveAudioFile(File &saveFile)
{
	if (mLeftBuffer == 0) //nothing ever loaded, quit here
		return;
	resetAudioRenderer();	
	int num = 1;
	if (mRightBuffer != 0)
		num++;
	float *leftOutputBuffer = (float*)malloc(1024*sizeof(float));
	float *rightOutputBuffer = (float*)malloc(1024*sizeof(float));;
	float* destBufs[num];
	destBufs[0] = leftOutputBuffer;
	if (num>1)
	{
		destBufs[1] = rightOutputBuffer;
	}
	
	saveFile.deleteFile();
	OutputStream *output = saveFile.createOutputStream();
	WavAudioFormat wavformat;
	AudioFormatWriter *writer = wavformat.createWriterFor(output, 44100.0, num, 16, StringPairArray(), 0);
	
	int *destIntBufs[num];
	int *leftIntBuffer = (int*)malloc(1024*sizeof(int));
	int *rightIntBuffer = (int*)malloc(1024*sizeof(int));
	destIntBufs[0] = leftIntBuffer;
	if (num>1)
	{
		destIntBufs[1] = rightIntBuffer;
	}
	while (!renderAudioToBuffer(destBufs, num, 1024))
	{
		for (int j=0; j<1024; j++)
		{
			leftIntBuffer[j] = (int)(leftOutputBuffer[j] * INT_MAX);
			rightIntBuffer[j] = (int)(rightOutputBuffer[j] * INT_MAX);
		}
		writer->write((const int**)destIntBufs, 1024);
	}
	
	free(leftOutputBuffer);
	free(rightOutputBuffer);
	free(leftIntBuffer);
	free(rightIntBuffer);
	delete writer;	
}

void MainController::playAudioFile(File &audioFile)
{
 // unload the previous file source and delete it..
    mTransportSource.stop();
    mTransportSource.setSource (0);
    deleteAndZero (mCurrentAudioFileSource);

    // get a format manager and set it up with the basic types (wav and aiff).
    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    AudioFormatReader* reader = formatManager.createReaderFor (audioFile);

    if (reader != 0)
    {
        mCurrentAudioFileSource = new AudioFormatReaderSource (reader, true);

        // ..and plug it into our transport source
        mTransportSource.setSource (mCurrentAudioFileSource,
                                   32768, // tells it to buffer this many samples ahead
                                   reader->sampleRate);
		mTransportSource.setPosition (0);
		mTransportSource.start();
    }
}

void MainController::resetAudioRenderer()
{
	for (int i=0; i< NUM_GRAINS; i++)
	{
		if (mGranularSlices[i] != 0)
		{
			mGranularSlices[i]->resetAudioPlayback();
		}
	}
}

void MainController::setEditor(MainEditor *editor)
{
	mMainEditor = editor;
	updateGUIParameters();
}

#pragma mark AudioIODeviceCallbacks
void MainController::audioDeviceAboutToStart (AudioIODevice* device)
{
    zeromem (samples, sizeof (samples));
	mPlaying = true;
	//mPlayButton->setButtonText(T("Stop"));
}

void MainController::audioDeviceStopped()
{
    zeromem (samples, sizeof (samples));
	mPlaying = false;
	//mPlayButton->setButtonText(T("Play"));
}

void MainController::audioDeviceIOCallback (const float** inputChannelData, int numInputChannels,
												 float** outputChannelData, int numOutputChannels, int numSamples)
{
	
	if (renderAudioToBuffer(outputChannelData, numOutputChannels, numSamples))
	{
		//should be able to loop without bad access, let's not stop!
		//mDeviceManager.removeAudioCallback(this);
	}
}

#pragma mark MessageListener
void MainController::handleMessage(const Message &message)
{
	const ParameterMessage &msg = (const ParameterMessage&)message;
	//handle message here and do the right stuff
	String command = msg.mStringCommand;
	float floatParameter = msg.mFloatParameter;
	int intParameter = msg.mIntParameter;
	String filePath = msg.mFilePath;
	if (command.compare(T("GRAIN_GAIN_CHANGED"))==0)
	{
		setGrainGain(floatParameter);
	}
	else if (command.compare(T("GRAIN_PAN_CHANGED"))==0)
	{
		setGrainPan(floatParameter);
	}
	else if (command.compare(T("GRAIN_LENGTH_CHANGED"))==0)
	{
		setGrainLength(floatParameter);
	}
	else if (command.compare(T("GRAIN_START_CHANGED"))==0)
	{
		setGrainStartPosition(floatParameter);
	}
	else if (command.compare(T("GRAIN_ADVANCE_CHANGED"))==0)
	{
		setGrainAdvanceAmount(floatParameter);
	}
	else if (command.compare(T("GRAIN_VELOCITY_CHANGED"))==0)
	{
		setGrainVelocityFactor(floatParameter);
	}
	else if (command.compare(T("GRAIN_START_RANDOM_CHANGED"))==0)
	{
		setGrainStartRandomAmount(floatParameter);
	}
	else if (command.compare(T("OPEN_FILE_PRESSED"))==0)
	{
		openFilePressed();
	}
	else if (command.compare(T("SAVE_FILE_PRESSED"))==0)
	{
		saveFilePressed();
	}
	else if (command.compare(T("SLICE_INDEX_CHANGED"))==0)
	{
		setCurrentSliceIndex(intParameter);
		//update view here
		updateGUIParameters();
	}
	else if (command.compare(T("SELECT_PRESSED"))==0)
	{
		setCurrentSliceIndex(intParameter);
		//update view here
		updateGUIParameters();
	}
	else if (command.compare(T("ACTIVE_PRESSED"))==0)
	{
		mGranularSlices[intParameter]->setBypass(!mGranularSlices[intParameter]->isBypass());
	}
	else if (command.compare(T("FILE_DROPPED"))==0)
	{	
		File mCurrentFile = File(filePath);
		memStoreAudioFile(mCurrentFile);
		mMainEditor->setupWaveformView(mCurrentFile);
	}
	else if (command.compare(T("REVERSE_PRESSED"))==0)
	{
		mGranularSlices[mCurrentSliceIndex]->setReverse(!mGranularSlices[mCurrentSliceIndex]->isReverse());
	}
	else if (command.compare(T("MONO_PRESSED"))==0)
	{	
		mGranularSlices[mCurrentSliceIndex]->setMono(!mGranularSlices[mCurrentSliceIndex]->isMono());
	}
}

void MainController::updateGUIParameters()
{
	ParameterMessage *parammsg = new ParameterMessage(getGrainGainSliderValue(), T("GRAIN_GAIN"));
	mMainEditor->postMessage(parammsg);
	parammsg = new ParameterMessage(getGrainPan(), T("GRAIN_PAN"));
	mMainEditor->postMessage(parammsg);
	parammsg = new ParameterMessage(getGrainLengthSliderValue(), T("GRAIN_LENGTH"));
	mMainEditor->postMessage(parammsg);
	parammsg = new ParameterMessage(getGrainStartPositionSliderValue(), T("GRAIN_START"));
	mMainEditor->postMessage(parammsg);
	parammsg = new ParameterMessage(getGrainAdvanceAmountSliderValue(), T("GRAIN_ADVANCE"));
	mMainEditor->postMessage(parammsg);
	parammsg = new ParameterMessage(getGrainVelocityFactorSliderValue(), T("GRAIN_VELOCITY"));
	mMainEditor->postMessage(parammsg);
	parammsg = new ParameterMessage(getGrainStartRandomAmount(), T("GRAIN_START_RANDOM"));
	mMainEditor->postMessage(parammsg);
	parammsg = new ParameterMessage(1, T("REVERSE"),(int)mGranularSlices[mCurrentSliceIndex]->isReverse());
	mMainEditor->postMessage(parammsg);
	parammsg = new ParameterMessage(1, T("MONO"), (int)mGranularSlices[mCurrentSliceIndex]->isMono());
	mMainEditor->postMessage(parammsg);
}

bool MainController::renderAudioToBuffer(float** outputChannelData, int numOutputChannels, int numSamples)
{
	// We need to clear the output buffers, in case they're full of junk..
    for (int i = 0; i < numOutputChannels; ++i)
	{
        if (outputChannelData[i] != 0)
            zeromem (outputChannelData[i], sizeof (float) * numSamples);
	}	 
	bool outofbounds = false;
	
	for (int i=0; i<NUM_GRAINS; i++)
	{
		outofbounds = mGranularSlices[i]->renderAudioBlock(outputChannelData, numOutputChannels, numSamples);
		if (outofbounds)
			outofbounds = true;
	}
	
	//clip output so you don't exceed digital max
	 for (int i = 0; i < numOutputChannels; ++i)
	{
		float *output = outputChannelData[i];
		for (int j=0; j<numSamples; ++j)
		{
			if (output[j] > 1.0f)
				output[j] = 1.0f;
			else if (output[j] < -1.0f)
				output[j] = -1.0f;
		}
	}
	return outofbounds;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainController" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TEXTBUTTON name="OpenFileButton" id="e02073e0540adda" memberName="mOpenFileButton"
              virtualName="" explicitFocusOrder="0" pos="24 272 150 24" buttonText="Open File"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="SaveFileButton" id="3f9e2515deae5ca9" memberName="mSaveFileButton"
              virtualName="" explicitFocusOrder="0" pos="216 272 150 24" buttonText="Save File"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
