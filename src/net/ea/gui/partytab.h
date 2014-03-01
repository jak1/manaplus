/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
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

#ifndef NET_EA_GUI_PARTYTAB_H
#define NET_EA_GUI_PARTYTAB_H

#include "gui/widgets/tabs/chattab.h"

namespace Ea
{

/**
 * A tab for a party chat channel.
 */
class PartyTab : public ChatTab,
                 public ConfigListener
{
    public:
        explicit PartyTab(const Widget2 *const widget);

        A_DELETE_COPY(PartyTab)

        virtual ~PartyTab();

        bool handleCommand(const std::string &restrict type,
                           const std::string &restrict args) override final;

        int getType() const override final A_WARN_UNUSED
        { return ChatTab::TAB_PARTY; }

        void saveToLogFile(const std::string &msg) const override final;

        void playNewMessageSound() const override final;

        void optionChanged(const std::string &value);

    protected:
        void handleInput(const std::string &msg) override final;

        void getAutoCompleteList(StringVect&) const override final;

        void getAutoCompleteCommands(StringVect &names) const override final;
};

extern PartyTab *partyTab;

}  // namespace Ea

#endif  // NET_EA_GUI_PARTYTAB_H
