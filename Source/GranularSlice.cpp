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

#define RAND_WEIGHT(a) (rand() % a + 1)

const int kGrainLength = 44100;
const float kVelocityFactor = 1.0f;
const int kGrainAdvanceAmount = kGrainLength/3.0;
const int64 kSampleRamp = 500;
const int kGrainRandPosition = 20000;

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
	mGrainRandomPositionAbsolute[0] = 0;
	mGrainRandomPositionAbsolute[1] = 0;
	mSampleCounter = 0;
	mVelocityFactor = kVelocityFactor;	//higher is slower moving through audio file
	mGrainAdvanceAmount = kGrainAdvanceAmount;//how many samples the window will advance through the audio file
	mSampleAdvanceAmount = 1; //normally 1, could be -1 for reverse
	mRandomReadPosition = 1.0f; //between 0.0f and 1.0f
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

float GranularSlice::getGain()
{
	return mGain;
}

float GranularSlice::getPan()
{
	return mPan;
}

int64 GranularSlice::getGrainLength()
{
	return mGrainLength;
}

int64 GranularSlice::getGrainStartPosition()
{
	return mGrainStartPositionAbsolute;
}

float GranularSlice::getVelocity()
{
	return mVelocityFactor;
}

int64 GranularSlice::getGrainAdvanceAmount()
{
	return mGrainAdvanceAmount;
}

void GranularSlice::resetAudioPlayback()
{
	mGrainCurrentPositionRelativeLeft = 0;
	mGrainCurrentPositionRelativeRight = 0;
	mGrainCurrentPositionRelative[0] = 0;
	mGrainCurrentPositionRelative[1] = 0;
	mGrainStartPositionAbsolute = 0;
	mGrainRandomPositionAbsolute[0] = 0;
	mGrainRandomPositionAbsolute[1] = 0;
	mSampleCounter = 0;
}

int64 GranularSlice::getDataLength()
{
	return mDataLength;
}

bool GranularSlice::renderAudioBlock (float** outputData, int numChannels, int numSamples)
{
	// The outputData pointer could have existing audio in it.  Mix into that. 
	// Store this locally so that they won't change during an audio buffer render - psuedo variable lock 
	int grainPositionAbsolute = 0;
	int grainLength = mGrainLength;

	// This renders the granular audio into the output buffer for playback or file writing
	for (int i = 0; i < numChannels; ++i)
	{
		float *output = outputData[i];
		int samplesToRead = numSamples;

		grainPositionAbsolute = mGrainStartPositionAbsolute + mGrainRandomPositionAbsolute[i];

		if ((grainPositionAbsolute + mGrainCurrentPositionRelative[i] + numSamples) > mDataLength/mNumChannels)
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
			int samppos = grainPositionAbsolute + mGrainCurrentPositionRelative[i];
			output[j] += mData[i][samppos] * mGain * mPanGain[i];

			//clip output so you don't exceed digital max
			if (output[j] > 1.0f)
				output[j] = 1.0f;
			else if (output[j] < -1.0f)
				output[j] = -1.0f;

			//ramp in and out on grain boundaries	
			if (mGrainCurrentPositionRelative[i] <= kSampleRamp)
				output[j] = output[j] * ((float)(1.0f*mGrainCurrentPositionRelative[i]/(1.0f*kSampleRamp)));
			else if ((grainLength - mGrainCurrentPositionRelative[i]) <= kSampleRamp)
				output[j] = output[j] * ((float)(1.0f*(grainLength - mGrainCurrentPositionRelative[i]))/(1.0f*kSampleRamp));

			mGrainCurrentPositionRelative[i]++;

			//if we've reached the end of the Grain Slice, loop back to start
			if (mGrainCurrentPositionRelative[i] >= grainLength) {
				int relativeStartPosition;

				mGrainCurrentPositionRelative[i] = 0;
				mGrainRandomPositionAbsolute[i] = (RAND_WEIGHT(kGrainRandPosition) * mRandomReadPosition);

				relativeStartPosition = mGrainStartPositionAbsolute + mGrainRandomPositionAbsolute[i];

				// bound upper and lower limit on randomness affecting grain start sample position
				// either set to 0 or datalength - the grain length.
				if (relativeStartPosition + mGrainLength > mDataLength/mNumChannels)
					grainPositionAbsolute = mGrainStartPositionAbsolute;
				else if (relativeStartPosition < 0)
					grainPositionAbsolute = mGrainStartPositionAbsolute;
				else
					grainPositionAbsolute = relativeStartPosition;
			}
		}
	}
	
	mSampleCounter += numSamples;
	if (mSampleCounter >= (int64)(mGrainLength * mVelocityFactor) && (mVelocityFactor != 0))
	{
		mGrainStartPositionAbsolute += mGrainAdvanceAmount;
		mSampleCounter = 0;
		if (mGrainAdvanceAmount != 0)
		{
			mGrainCurrentPositionRelative[0] = 0;
			mGrainCurrentPositionRelative[1] = 0;
		}

		// randomize grain absolute position using random weight
		// make sure it doesn't exceed upper and lower bounds
	}
	
	return false;
}
