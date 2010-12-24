/*
 *  AudioRecorder.h
 *  JuceStandalone
 *
 *  Created by Hayden Bursk on 12/24/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "../JuceLibraryCode/JuceHeader.h"
#include "CircularAudioBuffer.h"

class AudioRecorder  : public Thread
{
public:
    AudioRecorder();
    ~AudioRecorder();


    //==============================================================================
    void startRecording (const File& file);
    void stop();
    bool isRecording() const;
	void run();
	void addSamplesToBuffer(float** writeData,int numChannels,int numSamples);
    
  
private:
	File mFileToRecord;
    double mSampleRate;
    bool mRecording;

    CircularAudioBuffer mCircularBuffer;
};