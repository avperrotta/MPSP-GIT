//
//  pspMainWindowComponent.h
//  psPsa-001
//
//  Created by Andre Perrotta on 21/11/14.
//
//

#ifndef __psPsa_001__pspMainWindowComponent__
#define __psPsa_001__pspMainWindowComponent__

#include <iostream>
#include "../JuceLibraryCode/JuceHeader.h"
#include "pspParticleSystemsManager.h"
#include "pspRoomConfigGUI.h"


class MainContentComponent;

class pspMainWindowComponent : public Component, public MenuBarModel, private Button::Listener, public ListBoxModel, public ComboBox::Listener
{
    
public:
    
    pspMainWindowComponent(MainContentComponent* mcc, pspParticleSystemsManager* pm);
    ~pspMainWindowComponent();
    
    void paint (Graphics& g) override;
    void resized() override;
    
    //Button::Listener virtuals
    void buttonClicked (Button* buttonThatWasClicked) override;
    
    
    //MenuBArModel virtuals
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String &menuName) override;
    void menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/) override;
    
    
    //ListBoxModel virtuals
    ListBox systemsList;
    int lastSelectedPSystem;
    bool deletedSystem;
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, Graphics &g, int width, int height, bool rowIsSelected) override;
    void listBoxItemClicked(int row, const MouseEvent &) override;
    void selectedRowsChanged(int lastRowSelected) override;
    void deleteKeyPressed(int lastRowSelected) override;
    /*
    Component* refreshComponentForRow(int rowNumber, bool isRowSelected, Component *existingComponentToUpdate) override;
    void listBoxItemDoubleClicked(int row, const MouseEvent &) override;
    void backgroundClicked(const MouseEvent &) override;
    void returnKeyPressed(int lastRowSelected) override;
    void listWasScrolled() override;
    var getDragSourceDescription(const SparseSet< int > &currentlySelectedRows) override;
    String getTooltipForRow(int row) override;
    MouseCursor getMouseCursorForRow(int row) override;
    */
    
    //ComboBox stuff
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
    
    pspParticleSystemsManager* getPspManager();
    
    //openGL conexion
    double xOffset;
    

private:
    //==============================================================================
    
    MainContentComponent* mainGLcomponent;
    
    //misc
    LookAndFeel_V1 lookAndFeelV1;
    LookAndFeel_V2 lookAndFeelV2;
    LookAndFeel_V3 lookAndFeelV3;
    
    int activeView;
    
    //main menu bar
    
    ScopedPointer<MenuBarComponent> menuBar;
    //editor view
    ComboBox addPSystemMenu;
    void populateAddPSystemsMenu();
    static void psItemDeleted(int result, pspMainWindowComponent* p);
    
    //room config
    pspRoomConfigGUI* roomConfigGui;
    Array<PropertyComponent*> comps;
    
    
    //psp
    pspParticleSystemsManager* pspManager;
    
    
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pspMainWindowComponent)
};


class newSlider : public SliderPropertyComponent
{
public:
    newSlider (const String& propertyName)
    : SliderPropertyComponent (propertyName, 0.0, 100.0, 0.001)
    {
        setValue (Random::getSystemRandom().nextDouble() * 42.0);
    }
    
    void setValue (double newValue) override
    {
        slider.setValue (newValue);
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (newSlider)
};



#endif /* defined(__psPsa_001__pspMainWindowComponent__) */

