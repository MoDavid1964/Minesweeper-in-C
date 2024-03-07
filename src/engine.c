/**
 * @ Author: MMMM
 * @ Create Time: 2024-02-24 14:26:01
 * @ Modified time: 2024-03-07 23:38:45
 * @ Description:
 * 
 * This combines the different utility function and manages the relationships between them.
 * Note that this file is annotated differently (demarcated with more comments) because of how
 *    verbose some of our APIs are... theyre kinda bulky so it looks prettier having a lot
 *    of separators around.
 */

#ifndef ENGINE_
#define ENGINE_

#include "./utils/utils.page.h"
#include "./utils/utils.asset.h"
#include "./utils/utils.buffer.h"
#include "./utils/utils.event.h"
#include "./utils/utils.file.h"
#include "./utils/utils.thread.h"
#include "./utils/utils.types.h"

#include "./pages.c"
#include "./events.c"

// Some definitions for identifiers
#define ENGINE_EVENT_LISTENERS "engine-event-listeners"
#define ENGINE_EVENT_LISTENERS_MUTEX "engine-events-listeners-mutex"
#define ENGINE_EVENT_LISTENERS_THREAD "engine-event-listeners-thread"

#define ENGINE_EVENT_HANDLERS "engine-event-handlers"
#define ENGINE_EVENT_HANDLERS_MUTEX "engine-events-handlers-mutex"
#define ENGINE_EVENT_HANDLERS_THREAD "engine-event-handlers-thread"

#define ENGINE_MAIN "engine-main"
#define ENGINE_MAIN_THREAD "engine-main-thread"

typedef struct Engine Engine;

/**
 * //
 * ////
 * //////    Engine struct
 * ////////
 * ////////// 
*/

/**
 * The engine struct handles the interactions between the different utility libraries.
 * It only deals with the libraries that have backend functionality.
 * 
 * @struct 
*/
struct Engine {

  // Some front end managers
  AssetManager assetManager;          // Deals with the game assets; this is shared across pages
  PageManager pageManager;            // The page manager

  // Some back end managers
  EventStore eventStore;              // Stores values updated by events
  EventManager eventManager;          // Deals with events
  ThreadManager threadManager;        // Manages the different threads of the program

  int bState;                         // The state of the engine

};

void Engine_init(Engine *this);

void Engine_main(p_obj pArgs_Engine, int tArg_NULL);

void Engine_exit(Engine *this);

/**
 * //
 * ////
 * //////    Engine init
 * ////////
 * ////////// 
*/

/**
 * Initializes the engine.
 * This function is annotated differently because it does a lot of things.
 * 
 * @param   { Engine * }  this      The engine object.
*/
void Engine_init(Engine *this) {
  
  // The engine is currently running
  this->bState = 1;

  /**
   * Initialize the managers
  */
  AssetManager_init(&this->assetManager);
  PageManager_init(&this->pageManager, &this->assetManager, &this->eventStore);

  EventStore_init(&this->eventStore);
  EventManager_init(&this->eventManager, &this->eventStore);
  ThreadManager_init(&this->threadManager);

  /**
   * Creates all our assets
  */
  AssetManager_readAssetFile(&this->assetManager, "//", "./src/assets/main-font.asset.txt");
  AssetManager_readAssetFile(&this->assetManager, "//", "./src/assets/menu.asset.txt");
  AssetManager_readAssetFile(&this->assetManager, "//", "./src/assets/game.asset.txt");

  /**
   * Creates all our pages
  */
  PageManager_createPage(&this->pageManager, "menu", PageHandler_menu);
  PageManager_createPage(&this->pageManager, "intro", PageHandler_intro);

  /**
   * Creates event listeners and handlers, alongside their mutexes
  */
  EventManager_createEventListener(&this->eventManager, EVENT_KEY, EventListener_keyPressed);
  EventManager_createEventHandler(&this->eventManager, EVENT_KEY, EventHandler_keyPressed);

  ThreadManager_createMutex(&this->threadManager, ENGINE_EVENT_LISTENERS_MUTEX);              
  ThreadManager_createMutex(&this->threadManager, ENGINE_EVENT_HANDLERS_MUTEX);              

  /**
   * Thread for event listeners
  */
  ThreadManager_createThread(
    &this->threadManager, 
    
    ENGINE_EVENT_LISTENERS_THREAD,                // The name of the thread
    ENGINE_EVENT_LISTENERS_MUTEX,                 // The name of the mutex
    
    EventManager_triggerEvent,                    // The routine that triggers events
    &this->eventManager,                          // The event manager
    EVENT_KEY);                                   // What type of event the thread triggers

  /**
   * Thread for event handlers
  */
  ThreadManager_createThread(
    &this->threadManager,

    ENGINE_EVENT_HANDLERS_THREAD,
    ENGINE_EVENT_HANDLERS_MUTEX,

    EventManager_resolveEvent,                    // The routine that resolves events
    &this->eventManager,                          // The event manager
    0);                                           // A dummy value

  /**
   * Thread for the main program
  */
  ThreadManager_createThread(
    &this->threadManager,
    ENGINE_MAIN_THREAD,                           // The main thread
    ENGINE_EVENT_HANDLERS_MUTEX,                  // It will share a mutex with the event handlers to prevent it from
                                                  //    reading the shared state while the handlers are changing this.

    Engine_main,                                  // The main routine
    this,                                         // The engine itself
    0);                                           // A dummy value
}

/**
 * //
 * ////
 * //////    Engine exit
 * ////////
 * ////////// 
*/

/**
 * Do some clean up after the entire program runs.
 * Frees whatever was allocated.
 * 
 * @param   { Engine * }  this  The engine object.
*/
void Engine_exit(Engine *this) {

  // Exit the event manager first, since it relies on the threads
  EventManager_exit(&this->eventManager);

  // Exit the thread manager last
  ThreadManager_exit(&this->threadManager);
}

/**
 * //
 * ////
 * //////    Engine main
 * ////////
 * ////////// 
*/

/**
 * The main thread of the engine.
 * 
 * @param   { p_obj * }   pArgs_Engine  The engine object.
 * @param   { int }       tArg_NULL     A dummy value.
*/
void Engine_main(p_obj pArgs_Engine, int tArg_NULL) {

  // Get the engine
  Engine *this = (Engine *) pArgs_Engine;

  // Go to menu when intro is done
  if(PageManager_getActiveState(&this->pageManager) == PAGE_ACTIVE_IDLE)
    PageManager_setActive(&this->pageManager, "menu");
  PageManager_update(&this->pageManager);

  // Update 
  if(EventStore_get(&this->eventStore, "key-pressed") == 'q')
    this->bState = 0;

  // Reset event store each time
  // This has to happen on this thread because this is where the "key-pressed" data is read
  EventStore_clear(&this->eventStore, "key-pressed");
}

/**
 * //
 * ////
 * //////    Engine getState
 * ////////
 * ////////// 
*/

/**
 * Returns the state of the engine.
 * Returns a 1 when the engine is currently running.
 * Returns a 0 when all its processes have exited.
 * 
 * @param   { Engine * }  this  The engine object.
 * @return  { int }             Whether or not the engine is still running.
*/
int Engine_getState(Engine *this) {
  return this->bState;
}

#endif