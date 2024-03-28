/**
 * @ Author: MMMM
 * @ Create Time: 2024-02-25 15:06:24
 * @ Modified time: 2024-03-28 21:01:05
 * @ Description:
 * 
 * This file defines the page handler for the editor page.
 */

#ifndef PAGE_EDITOR_
#define PAGE_EDITOR_

#include "../game/game.c"

#include "../utils/utils.asset.h"
#include "../utils/utils.page.h"
#include "../utils/utils.component.h"

/**
 * Configures the main menu.
 * 
 * @param   { p_obj }   pArgs_Page  The page instance we need to configure.
*/
void PageHandler_editor(p_obj pArgs_Page) {

  Page *this = (Page *) pArgs_Page;
  Game *pGame = (Game *) this->pSharedObject;
  int dWidth, dHeight, dMargin;

  // Header details
  char *sHeader = "level editor";
  char *sHeaderFont = "body-font";
  char sHeaderKey[STRING_KEY_MAX_LENGTH];

  // Component names
  char *sEditorComponent = "editor.fixed";
  char *sEditorFormComponent = "editor-form.col";
  char *sHeaderComponent = "header.acenter-x.atop-y";
  char *sFieldContainerComponent = "field-container.col.aleft-x.atop-y";
  char *sFilenamePromptComponent = "filename-prompt.aleft-x";
  char *sWidthPromptComponent = "width-prompt.aleft-x";
  char *sHeightPromptComponent = "height-prompt.aleft-x";
  char *sFilenameComponent = "filename.aleft-x";
  char *sWidthComponent = "width.aleft-x";
  char *sHeightComponent = "height.aleft-x";
  char *sFieldPromptComponent = "field-prompt.aleft-x";
  char *sErrorPromptComponent = "error-prompt.aleft-x";

  // Input fields
  char *sFilenameField;
  char *sWidthField;
  char *sHeightField;
  char cEditorCurrentField = 0;
  char cEditorFieldCount = 3;

  // Do stuff based on page status
  switch(this->ePageStatus) {

    case PAGE_ACTIVE_INIT:

      // Get the dimensions 
      dWidth = IO_getWidth();
      dHeight = IO_getHeight();
      dMargin = 44;

      // Create the header
      String_keyAndStr(sHeaderKey, sHeaderFont, sHeader);
      AssetManager_createTextAsset(this->pSharedAssetManager, sHeader, sHeaderFont);

      // Create component tree
      Page_addComponentContext(this, sEditorComponent, "root", 0, 0, dWidth, dHeight, "primary", "secondary");
      Page_addComponentContainer(this, sEditorFormComponent, sEditorComponent, 0, 0);
      Page_addComponentAsset(this, sHeaderComponent, sEditorFormComponent, dWidth / 2, 6, "", "", sHeaderKey);
      Page_addComponentContainer(this, sFieldContainerComponent, sEditorFormComponent, dWidth / 2 - dMargin / 2, 4);
      Page_addComponentText(this, sFilenamePromptComponent, sFieldContainerComponent, 1, 0, "", "", "Enter filename:");
      Page_addComponentText(this, sFilenameComponent, sFieldContainerComponent, 1, 0, "", "", "");
      Page_addComponentText(this, sWidthPromptComponent, sFieldContainerComponent, 1, 1, "", "", "Enter number of cols (5-15):");
      Page_addComponentText(this, sWidthComponent, sFieldContainerComponent, 1, 0, "", "", "");
      Page_addComponentText(this, sHeightPromptComponent, sFieldContainerComponent, 1, 1, "", "", "Enter number of rows (5-10):");
      Page_addComponentText(this, sHeightComponent, sFieldContainerComponent, 1, 0, "", "", "");
      Page_addComponentText(this, sErrorPromptComponent, sFieldContainerComponent, 1, 2, "secondary", "accent", "");
      Page_addComponentText(this, sFieldPromptComponent, sFieldContainerComponent, 1, 1, "primary-darken-0.5", "", "[tab] to switch fields; [enter] to submit");
      
      // Define initial user states
      if(Page_getUserState(this, "editor-current-field") == -1) Page_setUserState(this, "editor-current-field", cEditorCurrentField);
      if(Page_getUserState(this, "editor-field-count") == -1) Page_setUserState(this, "editor-field-count", cEditorFieldCount);
    break;

    case PAGE_ACTIVE_RUNNING:
      
      // Key handling
      cEditorCurrentField = Page_getUserState(this, "editor-current-field");
      cEditorFieldCount = Page_getUserState(this, "editor-field-count");

      // Retrieve the user input 
      sFilenameField = EventStore_getString(this->pSharedEventStore, "filename-input");
      sWidthField = EventStore_getString(this->pSharedEventStore, "width-input");
      sHeightField = EventStore_getString(this->pSharedEventStore, "height-input");

      // Switch based on what key was last pressed
      switch(EventStore_get(this->pSharedEventStore, "key-pressed")) {

        // Escape character to go back
        case 27:
          Page_idle(this);
          Page_setNext(this, "menu");
        break;

        // Switch fields
        case '\t':
          Page_setUserState(this, "editor-current-field", ((int) cEditorCurrentField + 1) % (int) cEditorFieldCount);
        break;

        // Do input checking then go to level editor when valid
        case '\n': case '\r':

          // If one of the fields are empty
          if(!strlen(sFilenameField) || 
            !strlen(sWidthField) || 
            !strlen(sHeightField)) {
            Page_setComponentText(this, sErrorPromptComponent, "Error: some fields are empty.");

          // If the filename is invalid            
          } else if(!String_isValidFilename(sFilenameField)) {
            Page_setComponentText(this, sErrorPromptComponent, "Error: filename has invalid characters.");

          // If the width or height is invalid
          } else if(!atoi(sWidthField) || !atoi(sHeightField)) {
            Page_setComponentText(this, sErrorPromptComponent, "Error: invalid width or height.");

          // If the width or height is out of range
          // ! DONT MAKE THIS HARDCODED, STORE THESE IN MACROS IN ONE OF THE GAME FILES
          } else if(atoi(sWidthField) > 15 || atoi(sWidthField) < 5 || 
            atoi(sHeightField) > 10 || atoi(sHeightField) < 5) {
            Page_setComponentText(this, sErrorPromptComponent, "Error: width or height is out of range.");

          // If all the information is valid
          } else {
            Page_idle(this);
            Page_setNext(this, "editor-i"); 
          }
        break;

        default:
        
          // Update the values of the current inputted username
          if(cEditorCurrentField == 0) { 
            EventStore_setString(this->pSharedEventStore, "key-pressed", "filename-input");

            Page_setComponentColor(this, sFilenamePromptComponent, "primary", "");
            Page_setComponentColor(this, sFilenameComponent, "accent", "");
            Page_setComponentColor(this, sWidthPromptComponent, "primary-darken-0.75", "");
            Page_setComponentColor(this, sWidthComponent, "primary-darken-0.75", "");
            Page_setComponentColor(this, sHeightPromptComponent, "primary-darken-0.75", "");
            Page_setComponentColor(this, sHeightComponent, "primary-darken-0.75", "");

          // Update the values of the current inputted password
          } else if(cEditorCurrentField == 1) {
            EventStore_setString(this->pSharedEventStore, "key-pressed", "width-input");      

            Page_setComponentColor(this, sFilenamePromptComponent, "primary-darken-0.75", "");
            Page_setComponentColor(this, sFilenameComponent, "primary-darken-0.75", "");
            Page_setComponentColor(this, sWidthPromptComponent, "primary", "");
            Page_setComponentColor(this, sWidthComponent, "accent", "");    
            Page_setComponentColor(this, sHeightPromptComponent, "primary-darken-0.75", "");
            Page_setComponentColor(this, sHeightComponent, "primary-darken-0.75", "");      
          
          } else if(cEditorCurrentField == 2) {
            EventStore_setString(this->pSharedEventStore, "key-pressed", "height-input");      

            Page_setComponentColor(this, sFilenamePromptComponent, "primary-darken-0.75", "");
            Page_setComponentColor(this, sFilenameComponent, "primary-darken-0.75", "");
            Page_setComponentColor(this, sWidthPromptComponent, "primary-darken-0.75", "");
            Page_setComponentColor(this, sWidthComponent, "primary-darken-0.75", "");    
            Page_setComponentColor(this, sHeightPromptComponent, "primary", "");
            Page_setComponentColor(this, sHeightComponent, "accent", "");     
          }

          // Clear the error
          if(EventStore_get(this->pSharedEventStore, "key-pressed"))
            Page_setComponentText(this, sErrorPromptComponent, "");

        break;
      }

      // Indicate the user input on screen
      Page_setComponentText(this, sFilenameComponent, strlen(sFilenameField) ? sFilenameField : "<filename>");
      Page_setComponentText(this, sWidthComponent, strlen(sWidthField) ? sWidthField : "<width>");
      Page_setComponentText(this, sHeightComponent, strlen(sHeightField) ? sHeightField : "<height>");

    break;

    default:

    break;
  }
}

#endif