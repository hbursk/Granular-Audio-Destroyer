/*
 *  ParameterMessage.cpp
 *  JuceStandalone
 *
 *  Created by Hayden Bursk on 12/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParameterMessage.h"

ParameterMessage::ParameterMessage (float floatParameter, String stringCommand, int intParameter, void* controlPtr, String filePath) throw()
    : mFloatParameter(floatParameter),
	  mStringCommand(stringCommand),
	  mIntParameter(intParameter),
	  mControlPtr(controlPtr),
	  mFilePath(filePath)
{

}

ParameterMessage::~ParameterMessage() throw()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

   

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

