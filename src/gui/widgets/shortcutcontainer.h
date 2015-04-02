/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#ifndef GUI_WIDGETS_SHORTCUTCONTAINER_H
#define GUI_WIDGETS_SHORTCUTCONTAINER_H

#include "gui/widgets/widget.h"

#include "listeners/mouselistener.h"
#include "listeners/widgetlistener.h"

class Image;
class ImageCollection;

/**
 * A generic shortcut container.
 *
 * \ingroup GUI
 */
class ShortcutContainer notfinal : public Widget,
                                   public WidgetListener,
                                   public MouseListener
{
    public:
        A_DELETE_COPY(ShortcutContainer)

        /**
         * Destructor.
         */
        virtual ~ShortcutContainer();

        /**
         * Draws the shortcuts
         */
        void draw(Graphics *graphics) override = 0;

        /**
         * Invoked when a widget changes its size. This is used to determine
         * the new height of the container.
         */
        void widgetResized(const Event &event) override final;

        void widgetMoved(const Event& event) override final;

        /**
         * Handles mouse when dragged.
         */
        void mouseDragged(MouseEvent &event A_UNUSED) override
        {
        }

        /**
         * Handles mouse when pressed.
         */
        void mousePressed(MouseEvent &event A_UNUSED) override
        {
        }

        /**
         * Handles mouse release.
         */
        void mouseReleased(MouseEvent &event A_UNUSED) override
        {
        }

        int getMaxItems() const A_WARN_UNUSED
        { return mMaxItems; }

        int getBoxWidth() const A_WARN_UNUSED
        { return mBoxWidth; }

        int getBoxHeight() const A_WARN_UNUSED
        { return mBoxHeight; }

        void drawBackground(Graphics *g);

    protected:
        /**
         * Constructor. Initializes the shortcut container.
         */
        explicit ShortcutContainer(Widget2 *const widget);

        /**
         * Gets the index from the grid provided the point is in an item box.
         *
         * @param pointX X coordinate of the point.
         * @param pointY Y coordinate of the point.
         * @return index on success, -1 on failure.
         */
        int getIndexFromGrid(const int pointX,
                             const int pointY) const A_WARN_UNUSED;

        Image *mBackgroundImg;
        static float mAlpha;

        unsigned mMaxItems;
        int mBoxWidth;
        int mBoxHeight;
        int mGridWidth;
        int mGridHeight;
        ImageCollection *mVertexes;
};

#endif  // GUI_WIDGETS_SHORTCUTCONTAINER_H
