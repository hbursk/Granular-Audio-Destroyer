/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  1 Dec 2010 11:30:39pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/


//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
//[/Headers]


//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ParameterMessage  : public Message

{
public:
    //==============================================================================
    ParameterMessage (float floatParameter, String stringCommand, int intParameter = 0, void* controlPtr = 0) throw();
    ~ParameterMessage() throw();

	float  mFloatParameter;
	int	   mIntParameter;
	String mStringCommand;
	void*  mControlPtr;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.

};
