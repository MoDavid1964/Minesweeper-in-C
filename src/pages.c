/**
 * @ Author: MMMM
 * @ Create Time: 2024-02-25 15:06:24
 * @ Modified time: 2024-03-06 17:47:27
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
  char *sLogoComponent = "logo-fixed";

  // Do stuff based on page status
  switch(this->ePageStatus) {

    case PAGE_ACTIVE_INIT:

      Page_addComponent(this, sIntroComponent, "root", 0, 0, dWidth, dHeight, 0, NULL, 0xffffff, 0x080808);

      Page_addComponent(this, sOuterBoxComponent, sIntroComponent, 0, 0, 20, 10, 0, NULL, 0x111111, 0x888888);
      Page_addComponent(this, sInnerBoxComponent, sOuterBoxComponent, 0, 0, 16, 8, 0, NULL, 0x111111, 0xcccccc);
      // Page_addComponent(this, sLogoComponent, sIntroComponent, 
      //   0, 0, 
        
      //   AssetManager_getAssetWidth(this->pSharedAssetManager, "logo"),
      //   AssetManager_getAssetHeight(this->pSharedAssetManager, "logo"),
      //   AssetManager_getAssetHeight(this->pSharedAssetManager, "logo"),
      //   AssetManager_getAssetText(this->pSharedAssetManager, "logo"), 
      //   -1, -1);
      
    break;

    case PAGE_ACTIVE_RUNNING:
      
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
  char *sTitle = "minesweeper";
  int dTitleLength = strlen(sTitle);
  int dTotalLength;
  
  char sComponentKey[STRING_KEY_MAX_LENGTH];
  char sAssetKey[STRING_KEY_MAX_LENGTH];

  // Do stuff based on page status
  switch(this->ePageStatus) {

    case PAGE_ACTIVE_INIT:

      // A container for the title
      Page_addComponent(this, "title-row", "root",
        0, 10, 0, 0, 0, NULL, -1, -1);

      // Get the total length
      for(i = 0, dTotalLength = 0; i < dTitleLength; i++) {
        
        // Get the key
        String_keyAndChar(sAssetKey, "main-font", sTitle[i]);

        // Compute the total length
        dTotalLength += AssetManager_getAssetWidth(this->pSharedAssetManager, sAssetKey);
      }

      // Add each of the letters to the tree
      for(i = 0; i < dTitleLength; i++) {

        // Generate the keys first
        String_keyAndId(sComponentKey, "title", i);
        String_keyAndChar(sAssetKey, "main-font", sTitle[i]);

        // Add the component
        Page_addComponent(this, sComponentKey, "title-row",
          dWidth / 2 - dTotalLength / 2, i * i * i * 10 - 128, 
          AssetManager_getAssetWidth(this->pSharedAssetManager, sAssetKey), 
          AssetManager_getAssetHeight(this->pSharedAssetManager, sAssetKey),
          AssetManager_getAssetHeight(this->pSharedAssetManager, sAssetKey),
          AssetManager_getAssetText(this->pSharedAssetManager, sAssetKey),
          -1, -1);  

        Page_setComponentTarget(this, sComponentKey, PAGE_NULL_INT, 0, -1, -1, -1, -1, 0.69);
      }

      Page_setComponentTarget(this, "title-row", PAGE_NULL_INT, 10, -1, -1, -1, -1, 0.69);
    break;

    case PAGE_ACTIVE_RUNNING:
      
    break;

    default:
      // ! exit the page
    break;
  }
}

#endif