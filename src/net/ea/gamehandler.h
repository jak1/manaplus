/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef NET_EA_GAMEHANDLER_H
#define NET_EA_GAMEHANDLER_H

#include "net/gamehandler.h"

namespace Ea
{

class GameHandler notfinal : public Net::GameHandler
{
    public:
        GameHandler();

        A_DELETE_COPY(GameHandler)

        void who() const override final;

        bool removeDeadBeings() const override final A_WARN_UNUSED
        { return true; }

        virtual void setMap(const std::string &map);

        void clear() override final;

        void initEngines() const override final;
};

}  // namespace Ea

#endif  // NET_EA_GAMEHANDLER_H
