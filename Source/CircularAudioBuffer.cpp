/*
 *  CircularBuffer.cpp
 *  JuceStandalone
 *
 *  Created by Hayden Bursk on 12/24/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "CircularAudioBuffer.h"

CircularAudioBuffer::CircularAudioBuffer (const int numChannels, const int numSamples)
	: mBuffer (numChannels, numSamples)
{
	clear();
}

CircularAudioBuffer::~CircularAudioBuffer()
{
}

void CircularAudioBuffer::clear()
{
	mBuffer.clear();
	const ScopedLock sl (mBufferLock);
	mBufferValidStart = mBufferValidEnd = 0;
}

void CircularAudioBuffer::addSamplesToBuffer (const AudioSampleBuffer& sourceBuffer, int numSamples)
{
	const int bufferSize = mBuffer.getNumSamples();

	mBufferLock.enter();
	int newDataStart = mBufferValidEnd;
	int newDataEnd = newDataStart + numSamples;
	const int actualNewDataEnd = newDataEnd;
	mBufferValidStart = jmax (mBufferValidStart, newDataEnd - bufferSize);
	mBufferLock.exit();

	newDataStart %= bufferSize;
	newDataEnd %= bufferSize;

	if (newDataEnd < newDataStart)
	{
		for (int i = jmin (mBuffer.getNumChannels(), sourceBuffer.getNumChannels()); --i >= 0;)
		{
			mBuffer.copyFrom (i, newDataStart, sourceBuffer, i, 0, bufferSize - newDataStart);
			mBuffer.copyFrom (i, 0, sourceBuffer, i, bufferSize - newDataStart, newDataEnd);
		}
	}
	else
	{
		for (int i = jmin (mBuffer.getNumChannels(), sourceBuffer.getNumChannels()); --i >= 0;)
			mBuffer.copyFrom (i, newDataStart, sourceBuffer, i, 0, newDataEnd - newDataStart);
	}

	const ScopedLock sl (mBufferLock);
	mBufferValidEnd = actualNewDataEnd;
}

int CircularAudioBuffer::readSamplesFromBuffer (AudioSampleBuffer& destBuffer, int numSamples)
{
	const int bufferSize = mBuffer.getNumSamples();

	mBufferLock.enter();
	int availableDataStart = mBufferValidStart;
	const int numSamplesDone = jmin (numSamples, mBufferValidEnd - availableDataStart);
	int availableDataEnd = availableDataStart + numSamplesDone;
	mBufferValidStart = availableDataEnd;
	mBufferLock.exit();

	availableDataStart %= bufferSize;
	availableDataEnd %= bufferSize;

	if (availableDataEnd < availableDataStart)
	{
		for (int i = jmin (mBuffer.getNumChannels(), destBuffer.getNumChannels()); --i >= 0;)
		{
			destBuffer.copyFrom (i, 0, mBuffer, i, availableDataStart, bufferSize - availableDataStart);
			destBuffer.copyFrom (i, bufferSize - availableDataStart, mBuffer, i, 0, availableDataEnd);
		}
	}
	else
	{
		for (int i = jmin (mBuffer.getNumChannels(), destBuffer.getNumChannels()); --i >= 0;)
			destBuffer.copyFrom (i, 0, mBuffer, i, availableDataStart, numSamplesDone);
	}

	return numSamplesDone;
}