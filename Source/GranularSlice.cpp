/*
 *  GranularSlice.cpp
 *  JuceStandalone
 *
 *  Created by Hayden Bursk on 12/7/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GranularSlice.h"
#include <math.h>

const int kGrainLength = 44100;
const float kVelocityFactor = 1.0f;
const int kGrainAdvanceAmount = kGrainLength/3.0;

GranularSlice::GranularSlice (float* leftchanneldata, float* rightchanneldata, int64 datalength, int numchannels)
	: mLeftChannelData (leftchanneldata),
	  mRightChannelData (rightchanneldata)
{
	mGain = 1.0f;
	mPan = 0.5f;//center
	mPanLeft = 1.0f;//(float)cos(pan*PI/2);
	mPanRight = 1.0f;//(float)sin(pan*PI/2);	
	mPanGain[0] = mPanLeft;
	mPanGain[1] = mPanRight;
	
	mGrainLength = kGrainLength;	//smaller value is smaller grain lengths and less recognizable sounds
	mGrainCurrentPositionRelativeLeft = 0;
	mGrainCurrentPositionRelativeRight = 0;
	mGrainCurrentPositionRelative[0] = 0;
	mGrainCurrentPositionRelative[1] = 0;
	mGrainStartPositionAbsolute = 0;
	mSampleCounter = 0;
	mVelocityFactor = kVelocityFactor;	//higher is slower moving through audio file
	mGrainAdvanceAmount = kGrainAdvanceAmount;//how many samples the window will advance through the audio file
	mSampleAdvanceAmount = 1; //normally 1, could be -1 for reverse
	mRandomReadPosition = 0.0f; //between 0.0f and 1.0f
	mRandomStartPosition = 0.0f; //between 0.0f and 1.0f
	mRenderMono = false;
	
	mDataLength = datalength;
	mNumChannels = numchannels;
	
	mData[0] = mLeftChannelData;
	mData[1] = mRightChannelData;

}

GranularSlice::~GranularSlice ()
{

}

void GranularSlice::setGain (float gain)
{
	mGain = gain;
}

void GranularSlice::setPan (float pan)
{
	mPan = pan;
	mPanLeft = (float)cos(pan*M_PI/2);
	mPanRight = (float)sin(pan*M_PI/2);
	mPanGain[0] = mPanLeft;
	mPanGain[1] = mPanRight;
}

void GranularSlice::setGrainLength (int64 length)
{
	mGrainLength = length;
}

void GranularSlice::setGrainStartPosition (int64 start)
{
	mGrainStartPositionAbsolute = start;
}

void GranularSlice::setGrainCurrentPosition (int64 pos)
{
	mGrainCurrentPositionRelativeLeft = pos;
	mGrainCurrentPositionRelativeRight = pos;
	mGrainCurrentPositionRelative[0] = pos;
	mGrainCurrentPositionRelative[1] = pos;
}

void GranularSlice::setVelocity (float velocity)
{
	mVelocityFactor = velocity;
}

void GranularSlice::setGrainAdvanceAmount (int64 advance)
{
	mGrainAdvanceAmount = advance;
}

void GranularSlice::setSampleAdvanceAmount (int64 advance)
{
	mSampleAdvanceAmount = advance;
}

void GranularSlice::setRandomReadFactor (float randamount)
{
	mRandomReadPosition = randamount;
}

void GranularSlice::setRandomStartFactor (float randamount)
{
	mRandomStartPosition = randamount;
}

void GranularSlice::setRenderMono (bool mono)
{
	mRenderMono = mono;
}

void GranularSlice::resetAudioPlayback()
{
	mGrainCurrentPositionRelativeLeft = 0;
	mGrainCurrentPositionRelativeRight = 0;
	mGrainCurrentPositionRelative[0] = 0;
	mGrainCurrentPositionRelative[1] = 0;
	mGrainStartPositionAbsolute = 0;
	mSampleCounter = 0;
}

bool GranularSlice::renderAudioBlock (float** outputData, int numChannels, int numSamples)
{
	// The outputData pointer could have existing audio in it.  Mix into that.  
	
	// This renders the granular audio into the output buffer for playback or file writing
	for (int i = 0; i < numChannels; ++i)
	{
		float *output = outputData[i];
		int samplesToRead = numSamples;
		if ((mGrainStartPositionAbsolute + mGrainCurrentPositionRelative[i] + numSamples) > mDataLength/mNumChannels)
		{
			//do something here, we're about to go out of bounds
			for (int j= 0; j<numChannels; ++j)
			{
				zeromem (outputData[j], sizeof (float) * numSamples);
			}
			resetAudioPlayback();
			//returns true if we've hit the end of our data.  The caller should stop playing or writing audio here
			// This will just kill the last audio buffer instead of silencing the remaining space with zeros.  
			// Better to silence with zeros, but that's more work!
			return true;
		}
		
		for (int j=0; j<samplesToRead; ++j)
		{
			int samppos = mGrainStartPositionAbsolute + mGrainCurrentPositionRelative[i];
			output[j] += mData[i][samppos] * mGain * mPanGain[i];
			//clip output so you don't exceed digital max
			if (output[j] > 1.0f)
				output[j] = 1.0f;
			else if (output[j] < -1.0f)
				output[j] = -1.0f;
			mGrainCurrentPositionRelative[i]++;
			if (mGrainCurrentPositionRelative[i] >= mGrainLength)
				mGrainCurrentPositionRelative[i] = 0;
		}
	}
	
	mSampleCounter += numSamples;
	if (mSampleCounter >= (int64)(mGrainLength * mVelocityFactor))
	{
		mGrainStartPositionAbsolute += mGrainAdvanceAmount;
		mSampleCounter = 0;
	}
	
	return false;
}
