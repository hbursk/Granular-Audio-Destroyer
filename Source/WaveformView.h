/*
 *  WaveformView.h
 *  JuceStandalone
 *
 *  Created by Hayden Bursk on 12/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "../JuceLibraryCode/JuceHeader.h"




//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class WaveformView  : public Component,
					  public ChangeListener,
					  public FileDragAndDropTarget
{
public:
	 WaveformView ();
    ~WaveformView();
	 
	 void setMessageListener(MessageListener *listener);
	 void setFile (const File& file);
	 void setZoomFactor (double amount);
	 void paint (Graphics& g);
	 virtual void changeListenerCallback (ChangeBroadcaster* source);
	 virtual bool isInterestedInFileDrag (const StringArray& /*files*/);
	 virtual void filesDropped (const StringArray& files, int /*x*/, int /*y*/);
	 
private:
	AudioFormatManager  mFormatManager;
	AudioThumbnail		mThumbnail;
	AudioThumbnailCache mThumbnailCache;
	double				mStartTime;
	double				mEndTime;
	MessageListener*	mMessageListener;
	


};