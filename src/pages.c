/**
 * @ Author: MMMM
 * @ Create Time: 2024-02-25 15:06:24
 * @ Modified time: 2024-03-07 23:00:20
 * @ Description:
 * 
 * This file defines page configurers so we can define the different pages of our application.
 * Note that these functions are passed as callbacks to the pageManager.
 * Also, a page may have more than one runner.
 */

#ifndef PAGES_
#define PAGES_

#include "./utils/utils.asset.h"
#include "./utils/utils.page.h"
#include "./utils/utils.component.h"

/**
 * //
 * ////
 * //////    Page configurers
 * ////////
 * ////////// 
*/

/**
 * Configures the title page.
 * 
 * @param   { p_obj }   pArgs_Page  The instance page we need to configure.
*/
void PageHandler_intro(p_obj pArgs_Page) {
  
  Page *this = (Page *) pArgs_Page;
  int dWidth = IO_getWidth();
  int dHeight = IO_getHeight();

  // Component names
  char *sIntroComponent = "intro-row.col-center.y-center.x";
  char *sOuterBoxComponent = "box.outer-row.col-center.y-center.x";
  char *sInnerBoxComponent = "box.inner-row.col-center.y-center.x";
  char *sLogoComponent = "logo-acenter.x-acenter.y";

  // Do stuff based on page status
  switch(this->ePageStatus) {

    case PAGE_ACTIVE_INIT:

      // Create component tree
      Page_addComponent(this, sIntroComponent, "root", 0, 0, dWidth, dHeight, 0, NULL, 0x080808, 0x080808);
      Page_addComponent(this, sOuterBoxComponent, sIntroComponent, 0, 0, 160, 80, 0, NULL, 0x888888, 0x888888);
      Page_addComponent(this, sInnerBoxComponent, sOuterBoxComponent, 0, 0, 156, 78, 0, NULL, 0x080808, 0xf0f0f0);
      Page_addComponent(this, sLogoComponent, "root", dWidth / 2, 100, 
        AssetManager_getAssetWidth(this->pSharedAssetManager, "logo"),
        AssetManager_getAssetHeight(this->pSharedAssetManager, "logo"),
        AssetManager_getAssetHeight(this->pSharedAssetManager, "logo"),
        AssetManager_getAssetText(this->pSharedAssetManager, "logo"), -1, -1);

      // Set initials
      Page_resetComponentInitial(this, sOuterBoxComponent, 80, 40, 0, 0, -1, -1);
      Page_resetComponentInitial(this, sInnerBoxComponent, 80, 40, 0, 0, -1, -1);
      
    break;

    case PAGE_ACTIVE_RUNNING:
    
      switch(this->dStage) {
        
        case 0:   // Currently empty screen  
          if(this->dT > 16) 
            Page_nextStage(this);
        break;

        case 1:   // Make the box enlarge
          Page_setComponentTarget(this, sOuterBoxComponent, 60, 30, 40, 20, -1, -1, 0.5);
          Page_setComponentTarget(this, sInnerBoxComponent, 60, 30, 36, 18, -1, -1, 0.5);

          if(Page_getComponentDist(this, sInnerBoxComponent, 0) < MATH_E_NEG1)
            Page_nextStage(this);
        break;

        case 2:   // Make the logo fly to the center
          Page_setComponentTarget(this, sLogoComponent, PAGE_NULL_INT, dHeight / 2, -1, -1, -1, -1, 0.84);

          if(Page_getComponentDist(this, sLogoComponent, 1) < MATH_E_NEG3)
            Page_nextStage(this);
        break;

        case 3:   // Make the screen white
          Page_setComponentTarget(this, sLogoComponent, PAGE_NULL_INT, -16, -1, -1, -1, -1, -0.9);
          Page_setComponentTarget(this, sOuterBoxComponent, 0, 0, 160, 80, -1, -1, 0.45);
          Page_setComponentTarget(this, sInnerBoxComponent, 0, 0, 156, 78, -1, -1, 0.45);

          if(Page_getComponentDist(this, sLogoComponent, 1) < MATH_E_NEG1)
            Page_nextStage(this);
        break;
        
        default:  // Make the page idle
          Page_idle(this);
        break;
      }

    break;

    default:
      // ! exit the page
    break;
  }
}

/**
 * Configures the main menu.
 * 
 * @param   { p_obj }   pArgs_Page  The page instance we need to configure.
*/
void PageHandler_menu(p_obj pArgs_Page) {

  Page *this = (Page *) pArgs_Page;
  int dWidth = IO_getWidth();
  int dHeight = IO_getHeight();

  int i;

  // Component names
  char *sMenuComponent = "menu-fixed";
  char *sTitleComponent = "title-row";
  char *sLogoComponent = "logo";

  // Some other holding vars
  char *sTitle = "minesweeper";
  char *sFontName = "main-font";
  int dTitleLength = strlen(sTitle);
  int dTotalLength;

  // Refers to a single asset
  char sComponentKey[STRING_KEY_MAX_LENGTH];
  char sAssetKey[STRING_KEY_MAX_LENGTH];

  // Do stuff based on page status
  switch(this->ePageStatus) {

    case PAGE_ACTIVE_INIT:

      // A container for the title
      Page_addComponent(this, sMenuComponent, "root", 0, 0, 0, 0, 0, NULL, -1, -1);
      Page_addComponent(this, sTitleComponent, sMenuComponent, 0, 10, 0, 0, 0, NULL, -1, -1);

      // Get the total length
      for(i = 0, dTotalLength = 0; i < dTitleLength; i++) {
        
        // Get the key
        String_keyAndChar(sAssetKey, sFontName, sTitle[i]);

        // Compute the total length
        dTotalLength += AssetManager_getAssetWidth(this->pSharedAssetManager, sAssetKey);
      }

      // Add each of the letters to the tree
      for(i = 0; i < dTitleLength; i++) {

        // Generate the keys first
        String_keyAndId(sComponentKey, "title", i);
        String_keyAndChar(sAssetKey, sFontName, sTitle[i]);

        // Add the component
        Page_addComponent(this, sComponentKey, sTitleComponent,
          dWidth / 2 - dTotalLength / 2, i * i * i * -10 + 128, 
          AssetManager_getAssetWidth(this->pSharedAssetManager, sAssetKey), 
          AssetManager_getAssetHeight(this->pSharedAssetManager, sAssetKey),
          AssetManager_getAssetHeight(this->pSharedAssetManager, sAssetKey),
          AssetManager_getAssetText(this->pSharedAssetManager, sAssetKey),
          -1, -1);  

        // Make all the letters want to go to their places
        Page_setComponentTarget(this, sComponentKey, PAGE_NULL_INT, 0, -1, -1, -1, -1, 0.69);
      }

      // Shift it down a bit
      Page_setComponentTarget(this, sTitleComponent, PAGE_NULL_INT, 4, -1, -1, -1, -1, 0.69);

    break;

    case PAGE_ACTIVE_RUNNING:
      
      switch(this->dStage) {
        
        case 0:

        break;
      }

    break;

    default:
      // ! exit the page
    break;
  }
}

#endif