/*
 *  GranularSlice.h
 *  JuceStandalone
 *
 *  Created by Hayden Bursk on 12/7/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
 #include "../JuceLibraryCode/JuceHeader.h"


class GranularSlice
{
public:
	GranularSlice(float* leftchanneldata, float* rightchanneldata, int64 datalength, int numchannels);
	~GranularSlice();
	//setters
	void setGain(float gain);
	void setPan(float pan);
	void setGrainLength(int64 length);
	void setGrainStartPosition(int64 start);
	void setGrainCurrentPosition(int64 pos);
	void setVelocity(float velocity);
	void setGrainAdvanceAmount(int64 advance);
	void setSampleAdvanceAmount(int64 advance);
	void setRandomReadFactor(float randamount);
	void setRandomStartFactor(float randamount);
	void setRenderMono(bool mono);
	void setBypass(bool bypass);
	
	float getGain();
	float getPan();
	int64 getGrainLength();
	int64 getGrainStartPosition();
	float getVelocity();
	int64 getGrainAdvanceAmount();
	bool  isBypass();
	
	int64 getDataLength();
	void resetAudioPlayback();
	
	bool renderAudioBlock(float** outputData, int numChannels, int numSamples);

private:
	float mGain;
	float mPan;
	float mPanLeft;
	float mPanRight;
	float mPanGain[2];
	
	int64 mGrainLength;
	int64 mGrainStartPositionAbsolute;

	int64 mGrainStartPositionControlValue;
	int64 mGrainRandomPositionAbsolute[2];
	int64 mGrainCurrentPositionRelativeLeft;
	int64 mGrainCurrentPositionRelativeRight;
	int64 mGrainCurrentPositionRelative[2];
	int64 mSampleCounter;
	float mVelocityFactor;
	int64 mGrainAdvanceAmount;
	int64 mSampleAdvanceAmount; //normally 1, could be -1 for reverse
	float mRandomReadPosition; //between 0.0f and 1.0f
	float mRandomStartPosition; //between 0.0f and 1.0f
	bool  mRenderMono;
	bool  mBypass;
	bool  mReversed;
	float* mLeftChannelData;
	float* mRightChannelData;
	float* mData[2];
	int64  mDataLength;
	int	   mNumChannels;
};
