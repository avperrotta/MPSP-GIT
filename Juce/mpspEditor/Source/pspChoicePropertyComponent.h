//
//  pspChoicePropertyComponent.h
//  mpspEditor
//
//  Created by André Venturoti Perrotta on 18/08/15.
//
//

#ifndef __mpspEditor__pspChoicePropertyComponent__
#define __mpspEditor__pspChoicePropertyComponent__

#include <stdio.h>
#include "../JuceLibraryCode/JuceHeader.h"

class pspChoicePropertyComponent : public PropertyComponent,
private ComboBoxListener  // (can't use ComboBox::Listener due to idiotic VC2005 bug)
{
protected:
    /** Creates the component.
     Your subclass's constructor must add a list of options to the choices member variable.
     */
    pspChoicePropertyComponent (const String& propertyName);
    
public:
    /** Creates the component.
     
     Note that if you call this constructor then you must use the Value to interact with the
     index, and you can't override the class with your own setIndex or getIndex methods.
     If you want to use those methods, call the other constructor instead.
     
     @param valueToControl       the value that the combo box will read and control
     @param propertyName         the name of the property
     @param choices              the list of possible values that the drop-down list will contain
     @param correspondingValues  a list of values corresponding to each item in the 'choices' StringArray.
     These are the values that will be read and written to the
     valueToControl value. This array must contain the same number of items
     as the choices array
     */
    pspChoicePropertyComponent (const Value& valueToControl,
                             const String& propertyName,
                             const StringArray& choices,
                             const Array<var>& correspondingValues);
    
    /** Destructor. */
    ~pspChoicePropertyComponent();
    
    //==============================================================================
    /** Called when the user selects an item from the combo box.
     
     Your subclass must use this callback to update the value that this component
     represents. The index is the index of the chosen item in the choices
     StringArray.
     */
    virtual void setIndex (int newIndex);
    
    /** Returns the index of the item that should currently be shown.
     This is the index of the item in the choices StringArray that will be shown.
     */
    virtual int getIndex() const;
    
    /** Returns the list of options. */
    const StringArray& getChoices() const;
    
    
    //==============================================================================
    /** @internal */
    void refresh();
    
    void repopulateMenu();
    ComboBox comboBox;
    
protected:
    /** The list of options that will be shown in the combo box.
     
     Your subclass must populate this array in its constructor. If any empty
     strings are added, these will be replaced with horizontal separators (see
     ComboBox::addSeparator() for more info).
     */
    StringArray choices;
    
private:
    
    bool isCustomClass;
    
    class pspRemapperValueSource;
    void createComboBox();
    void comboBoxChanged (ComboBox*);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspChoicePropertyComponent)
};

#endif /* defined(__mpspEditor__pspChoicePropertyComponent__) */
