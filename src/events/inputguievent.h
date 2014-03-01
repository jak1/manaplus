/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2014  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*      _______   __   __   __   ______   __   __   _______   __   __
 *     / _____/\ / /\ / /\ / /\ / ____/\ / /\ / /\ / ___  /\ /  |\/ /\
 *    / /\____\// / // / // / // /\___\// /_// / // /\_/ / // , |/ / /
 *   / / /__   / / // / // / // / /    / ___  / // ___  / // /| ' / /
 *  / /_// /\ / /_// / // / // /_/_   / / // / // /\_/ / // / |  / /
 * /______/ //______/ //_/ //_____/\ /_/ //_/ //_/ //_/ //_/ /|_/ /
 * \______\/ \______\/ \_\/ \_____\/ \_\/ \_\/ \_\/ \_\/ \_\/ \_\/
 *
 * Copyright (c) 2004 - 2008 Olof Naessén and Per Larsson
 *
 *
 * Per Larsson a.k.a finalman
 * Olof Naessén a.k.a jansem/yakslem
 *
 * Visit: http://guichan.sourceforge.net
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Guichan nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef EVENTS_INPUTGUIEVENT_H
#define EVENTS_INPUTGUIEVENT_H

#include "events/event.h"

/**
  * Base class for all events concerning input.
  *
  * @author Olof Naessén
  * @since 0.6.0
  */
class InputGuiEvent: public Event
{
    public:
        /**
          * Constructor.
          *
          * @param source The source widget of the event.
          * @param isShiftPressed True if shift is pressed, false otherwise.
          * @param isControlPressed True if control is pressed, false otherwise.
          * @param isAltPressed True if alt is pressed, false otherwise.
          * @param isMetaPressed True if meta is pressed, false otherwise.
          */
        InputGuiEvent(Widget *const source,
                      const bool shiftPressed,
                      const bool controlPressed,
                      const bool altPressed,
                      const bool metaPressed) :
            Event(source),
            mShiftPressed(shiftPressed),
            mControlPressed(controlPressed),
            mAltPressed(altPressed),
            mMetaPressed(metaPressed),
            mIsConsumed(false)
        { }

        /**
          * Checks if shift is pressed.
          *
          * @return True if shift was pressed at the same time as the key,
          *         false otherwise.
          */
        bool isShiftPressed() const A_WARN_UNUSED
        { return mShiftPressed; }

        /**
          * Checks if control is pressed.
          *
          * @return True if control was pressed at the same time as the key,
          *         false otherwise.
          */
        bool isControlPressed() const A_WARN_UNUSED
        { return mControlPressed; }

        /**
          * Checks if alt is pressed.
          *
          * @return True if alt was pressed at the same time as the key,
          *         false otherwise.
          */
        bool isAltPressed() const A_WARN_UNUSED
        { return mAltPressed; }

        /**
          * Checks whether meta is pressed.
          *
          * @return True if meta was pressed at the same time as the key,
          *         false otherwise.
          */
        bool isMetaPressed() const A_WARN_UNUSED
        { return mMetaPressed; }

        /**
          * Marks the event as consumed. Input event listeners may discard
          * consumed input or act on consumed input. An example of a widget
          * that discards consumed input is the ScrollArea widget that 
          * discards consumed mouse wheel events so the ScrollArea will not 
          * scroll if for instance a Slider's value inside the ScrollArea was 
          * changed with the mouse wheel.
          *
          * @see isConsumed
          */
        void consume()
        { mIsConsumed = true; }

        /**
          * Checks if the input event is consumed.
          *
          * @return True if the input event is consumed, 
          *         false otherwise.
          * @see consume
          */
        bool isConsumed() const A_WARN_UNUSED
        { return mIsConsumed; }

    protected:
        /**
          * True if shift is pressed, false otherwise.
          */
        bool mShiftPressed;

        /**
          * True if control is pressed, false otherwise.
          */
        bool mControlPressed;

        /**
          * True if alt is pressed, false otherwise.
          */
        bool mAltPressed;

        /**
          * True if meta is pressed, false otherwise.
          */
        bool mMetaPressed;

        /**
          * True if the input event is consumed, 
          * false otherwise.
          */
        bool mIsConsumed;
};

#endif  // EVENTS_INPUTGUIEVENT_H
