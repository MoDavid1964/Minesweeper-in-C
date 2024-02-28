/**
 * @ Author: MMMM
 * @ Create Time: 2024-01-29 12:01:16
 * @ Modified time: 2024-02-28 16:45:08
 * @ Description:
 */

#include "./engine.c"
#include "game/field.obj.h"

#include "utils/utils.io.h"

#include <stdio.h>

int main() {

  // Some IO stuff
  IO io;
  IO_init(&io);
  IO_setSize(128, 32);
  
  // Set up the engine
  Engine engine;
  Engine_init(&engine);

  // Keep the main thread open while the engine is running
  while(Engine_getState(&engine)) {};

  // Clean up some stuff
  Engine_exit(&engine);
  IO_exit(&io);

  return 0;
}