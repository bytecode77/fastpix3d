/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2025 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/**
 * Main include header for the SDL library, version 3.2.4
 *
 * It is almost always best to include just this one header instead of
 * picking out individual headers included here. There are exceptions to
 * this rule--SDL_main.h is special and not included here--but usually
 * letting SDL.h include the kitchen sink for you is the correct approach.
 */

#ifndef SDL_h_
#define SDL_h_

#include "SDL/SDL_stdinc.h"
#include "SDL/SDL_assert.h"
#include "SDL/SDL_asyncio.h"
#include "SDL/SDL_atomic.h"
#include "SDL/SDL_audio.h"
#include "SDL/SDL_bits.h"
#include "SDL/SDL_blendmode.h"
#include "SDL/SDL_camera.h"
#include "SDL/SDL_clipboard.h"
#include "SDL/SDL_cpuinfo.h"
#include "SDL/SDL_dialog.h"
#include "SDL/SDL_endian.h"
#include "SDL/SDL_error.h"
#include "SDL/SDL_events.h"
#include "SDL/SDL_filesystem.h"
#include "SDL/SDL_gamepad.h"
#include "SDL/SDL_gpu.h"
#include "SDL/SDL_guid.h"
#include "SDL/SDL_haptic.h"
#include "SDL/SDL_hidapi.h"
#include "SDL/SDL_hints.h"
#include "SDL/SDL_init.h"
#include "SDL/SDL_iostream.h"
#include "SDL/SDL_joystick.h"
#include "SDL/SDL_keyboard.h"
#include "SDL/SDL_keycode.h"
#include "SDL/SDL_loadso.h"
#include "SDL/SDL_locale.h"
#include "SDL/SDL_log.h"
#include "SDL/SDL_messagebox.h"
#include "SDL/SDL_metal.h"
#include "SDL/SDL_misc.h"
#include "SDL/SDL_mouse.h"
#include "SDL/SDL_mutex.h"
#include "SDL/SDL_pen.h"
#include "SDL/SDL_pixels.h"
#include "SDL/SDL_platform.h"
#include "SDL/SDL_power.h"
#include "SDL/SDL_process.h"
#include "SDL/SDL_properties.h"
#include "SDL/SDL_rect.h"
#include "SDL/SDL_render.h"
#include "SDL/SDL_scancode.h"
#include "SDL/SDL_sensor.h"
#include "SDL/SDL_storage.h"
#include "SDL/SDL_surface.h"
#include "SDL/SDL_system.h"
#include "SDL/SDL_thread.h"
#include "SDL/SDL_time.h"
#include "SDL/SDL_timer.h"
#include "SDL/SDL_tray.h"
#include "SDL/SDL_touch.h"
#include "SDL/SDL_version.h"
#include "SDL/SDL_video.h"
#include "SDL/SDL_oldnames.h"

#endif /* SDL_h_ */
