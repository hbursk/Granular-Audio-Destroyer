/*
 *  CircularBuffer.h
 *  JuceStandalone
 *
 *  Created by Hayden Bursk on 12/24/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "../JuceLibraryCode/JuceHeader.h"

class CircularAudioBuffer
{
public:
    CircularAudioBuffer (const int numChannels, const int numSamples);
    ~CircularAudioBuffer();

    void clear();
    void addSamplesToBuffer (const AudioSampleBuffer& sourceBuffer, int numSamples);
	int readSamplesFromBuffer (AudioSampleBuffer& destBuffer, int numSamples);
   
private:
    CriticalSection mBufferLock;
    AudioSampleBuffer mBuffer;
    int mBufferValidStart, mBufferValidEnd;
};

