//
//  pspChoicePropertyComponent.cpp
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 18/08/15.
//
//

#include "pspChoicePropertyComponent.h"

class pspChoicePropertyComponent::pspRemapperValueSource : public Value::ValueSource,
private ValueListener
{
public:
    pspRemapperValueSource (const Value& source, const Array<var>& map)
    : sourceValue (source), mappings (map)
    {
        sourceValue.addListener (this);
    }
    
    var getValue() const
    {
        const var targetValue (sourceValue.getValue());
        
        for (int i = 0; i < mappings.size(); ++i)
            if (mappings.getReference(i).equalsWithSameType (targetValue))
                return i + 1;
        
        return mappings.indexOf (targetValue) + 1;
    }
    
    void setValue (const var& newValue)
    {
        const var remappedVal (mappings [static_cast <int> (newValue) - 1]);
        
        if (! remappedVal.equalsWithSameType (sourceValue))
            sourceValue = remappedVal;
    }
    
protected:
    Value sourceValue;
    Array<var> mappings;
    
    void valueChanged (Value&)
    {
        sendChangeMessage (true);
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspRemapperValueSource)
};


//==============================================================================
pspChoicePropertyComponent::pspChoicePropertyComponent (const String& name)
: PropertyComponent (name),
isCustomClass (true)
{
}

pspChoicePropertyComponent::pspChoicePropertyComponent (const Value& valueToControl,
                                                  const String& name,
                                                  const StringArray& choiceList,
                                                  const Array<var>& correspondingValues)
: PropertyComponent (name),
choices (choiceList),
isCustomClass (false)
{
    // The array of corresponding values must contain one value for each of the items in
    // the choices array!
    jassert (correspondingValues.size() == choices.size());
    
    createComboBox();
    
    comboBox.getSelectedIdAsValue().referTo (Value (new pspRemapperValueSource (valueToControl,
                                                                             correspondingValues)));
}

pspChoicePropertyComponent::~pspChoicePropertyComponent()
{
}

//==============================================================================
void pspChoicePropertyComponent::createComboBox()
{
    addAndMakeVisible (comboBox);
    
    for (int i = 0; i < choices.size(); ++i)
    {
        if (choices[i].isNotEmpty())
            comboBox.addItem (choices[i], i + 1);
        else
            comboBox.addSeparator();
    }
    
    comboBox.setEditableText (false);
}

void pspChoicePropertyComponent::setIndex (const int /*newIndex*/)
{
    jassertfalse; // you need to override this method in your subclass!
}

int pspChoicePropertyComponent::getIndex() const
{
    jassertfalse; // you need to override this method in your subclass!
    return -1;
}

const StringArray& pspChoicePropertyComponent::getChoices() const
{
    return choices;
}

//==============================================================================
void pspChoicePropertyComponent::refresh()
{
    if (isCustomClass)
    {
        if (! comboBox.isVisible())
        {
            createComboBox();
            comboBox.addListener (this);
        }
        
        comboBox.setSelectedId (getIndex() + 1, dontSendNotification);
    }
}

void pspChoicePropertyComponent::comboBoxChanged (ComboBox*)
{
    if (isCustomClass)
    {
        const int newIndex = comboBox.getSelectedId() - 1;
        
        if (newIndex != getIndex())
            setIndex (newIndex);
    }
}

void pspChoicePropertyComponent::repopulateMenu(){
    
}

