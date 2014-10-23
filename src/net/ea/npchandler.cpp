/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#include "net/ea/npchandler.h"

#include "gui/windows/npcdialog.h"

#include "net/messagein.h"

#include "utils/langs.h"

#include "debug.h"

namespace Ea
{

NpcDialog *NpcHandler::mDialog = nullptr;
bool NpcHandler::mRequestLang = false;

NpcHandler::NpcHandler()
{
    mDialog = nullptr;
    mRequestLang = false;
}

void NpcHandler::startShopping(const int beingId A_UNUSED) const
{
}

void NpcHandler::endShopping(const int beingId A_UNUSED) const
{
}

void NpcHandler::processNpcChoice(Net::MessageIn &msg)
{
    npcHandler->getNpc(msg);
    mRequestLang = false;

    if (mDialog)
    {
        mDialog->choiceRequest();
        mDialog->parseListItems(msg.readString(msg.getLength() - 8,
            "select items"));
    }
    else
    {
        msg.readString(msg.getLength() - 8, "select items");
    }
}

void NpcHandler::processNpcMessage(Net::MessageIn &msg)
{
    npcHandler->getNpc(msg);
    mRequestLang = false;

    const std::string message = msg.readString(msg.getLength() - 8, "message");
    // ignore future legacy npc commands.
    if (message.size() > 3 && message.substr(0, 3) == "###")
        return;
    if (mDialog)
        mDialog->addText(message);
}

void NpcHandler::processNpcClose(Net::MessageIn &msg)
{
    // Show the close button
    npcHandler->getNpc(msg);
    mRequestLang = false;
    if (mDialog)
        mDialog->showCloseButton();
}

void NpcHandler::processNpcNext(Net::MessageIn &msg)
{
    // Show the next button
    npcHandler->getNpc(msg);
    mRequestLang = false;
    if (mDialog)
        mDialog->showNextButton();
}

void NpcHandler::processNpcIntInput(Net::MessageIn &msg)
{
    // Request for an integer
    npcHandler->getNpc(msg);
    mRequestLang = false;
    if (mDialog)
        mDialog->integerRequest(0);
}

void NpcHandler::processNpcStrInput(Net::MessageIn &msg)
{
    // Request for a string
    int npcId = npcHandler->getNpc(msg);
    if (mRequestLang)
    {
        mRequestLang = false;
        npcHandler->stringInput(npcId, getLangSimple());
    }
    else if (mDialog)
    {
        mDialog->textRequest("");
    }
}

}  // namespace Ea
