/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2012  The ManaPlus Developers
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

#ifndef GUI_Setup_Other_H
#define GUI_Setup_Other_H

#include "guichanfwd.h"

#include "gui/widgets/setupitem.h"
#include "gui/widgets/setuptabscroll.h"

#include <guichan/actionlistener.hpp>

class EditDialog;
class TextField;

class Setup_Other : public SetupTabScroll
{
    public:
        Setup_Other();

        ~Setup_Other();

        void apply();

    protected:
        SetupItemNames *mProxyTypeList;
};

#endif
