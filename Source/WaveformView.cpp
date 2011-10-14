/*
 *  WaveformView.cpp
 *  JuceStandalone
 *
 *  Created by Hayden Bursk on 12/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "WaveformView.h"
#include "ParameterMessage.h"

WaveformView::WaveformView ()
    : mThumbnailCache (5),
	  mThumbnail(1024, mFormatManager, mThumbnailCache),
	  mMessageListener(0)
{
	mStartTime = mEndTime = 0;
	mFormatManager.registerBasicFormats();
	mThumbnail.addChangeListener(this);
}

WaveformView::~WaveformView()
{
	mThumbnail.removeChangeListener(this);
}

void WaveformView::setMessageListener(MessageListener *listener)
{
	mMessageListener = listener;
}

void WaveformView::setFile (const File& file)
{
	mThumbnail.setSource (new FileInputSource (file));
	mStartTime = 0;
	mEndTime = mThumbnail.getTotalLength();
}

void WaveformView::setZoomFactor (double amount)
{
	if (mThumbnail.getTotalLength() > 0)
	{
		double timeDisplayed = jmax (0.001, (mThumbnail.getTotalLength() - mStartTime) * (1.0 - jlimit (0.0, 1.0, amount)));
		mEndTime = mStartTime + timeDisplayed;
		repaint();
	}
}

void WaveformView::paint (Graphics& g)
{
	g.fillAll (Colour(0x00000000));

	g.setColour (Colour (0x50374750));

	if (mThumbnail.getTotalLength() > 0)
	{
		mThumbnail.drawChannels (g, getLocalBounds().reduced (2, 2), mStartTime, mEndTime, 1.0f);
	}
	else
	{
		g.setFont (14.0f);
		g.drawFittedText ("(Drag audio file here to load)", 0, 0, getWidth(), getHeight(),
						  Justification::centred, 2);
	}
}

void WaveformView::changeListenerCallback (ChangeBroadcaster* source)
{
	// this method is called by the thumbnail when it has changed, so we should repaint it..
	repaint();
}

bool WaveformView::isInterestedInFileDrag (const StringArray& /*files*/)
{
	return true;
}

void WaveformView::filesDropped (const StringArray& files, int /*x*/, int /*y*/)
{
	if (mMessageListener->isValidMessageListener())
	{
		ParameterMessage *msg = new ParameterMessage(1, T("FILE_DROPPED"), 1,0,files[0]);
		mMessageListener->postMessage(msg);
	}
}