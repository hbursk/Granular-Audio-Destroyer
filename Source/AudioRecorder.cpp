/*
 *  AudioRecorder.cpp
 *  JuceStandalone
 *
 *  Created by Hayden Bursk on 12/24/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "AudioRecorder.h"
#define SAMPLE_RATE 44100.0
#define CBUFFER_SIZE (44100*6) 

AudioRecorder::AudioRecorder()
        : Thread ("audio recorder"),
          mCircularBuffer (2, CBUFFER_SIZE),
          mRecording (false),
		  mSampleRate (SAMPLE_RATE)
{
}

AudioRecorder::~AudioRecorder()
{
	stop();
}

//==============================================================================
void AudioRecorder::startRecording (const File& file)
{
	stop();

	if (mSampleRate > 0)
	{
		mFileToRecord = file;
		startThread();

		mCircularBuffer.clear();
		mRecording = true;
	}
}

void AudioRecorder::stop()
{
	mRecording = false;

	stopThread (5000);
}

bool AudioRecorder::isRecording() const
{
	return isThreadRunning() && mRecording;
}

void AudioRecorder::addSamplesToBuffer(float** writeData, int numChannels, int numSamples)
{
	if (mRecording)
	{
		const AudioSampleBuffer incomingData ((float**) writeData, numChannels, numSamples);
		mCircularBuffer.addSamplesToBuffer (incomingData, numSamples);
	}
}


void AudioRecorder::run()
{
	mFileToRecord.deleteFile();

	OutputStream* outStream = mFileToRecord.createOutputStream();
	if (outStream == 0)
		return;

	WavAudioFormat wavFormat;
	AudioFormatWriter* writer = wavFormat.createWriterFor (outStream, mSampleRate, 2, 16, StringPairArray(), 0);

	if (writer == 0)
	{
		delete outStream;
		return;
	}

	AudioSampleBuffer tempBuffer (2, 8192);

	while (! threadShouldExit())
	{
		int numSamplesReady = mCircularBuffer.readSamplesFromBuffer (tempBuffer, tempBuffer.getNumSamples());

		if (numSamplesReady > 0)
			tempBuffer.writeToAudioWriter (writer, 0, numSamplesReady);

		Thread::sleep (1);
	}

	delete writer;
}