/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2016  The ManaPlus Developers
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

#include "net/ea/inventoryhandler.h"

#include "net/ea/equipbackend.h"
#include "net/ea/inventoryrecv.h"

#include "utils/delete2.h"

#include "debug.h"

namespace Ea
{

InventoryHandler::InventoryHandler()
{
    InventoryRecv::mEquips.clear();
    InventoryRecv::mInventoryItems.clear();
    InventoryRecv::mStorage = nullptr;
    storageWindow = nullptr;
    while (!InventoryRecv::mSentPickups.empty())
        InventoryRecv::mSentPickups.pop();
    InventoryRecv::mDebugInventory = true;
}

InventoryHandler::~InventoryHandler()
{
    if (storageWindow)
    {
        storageWindow->close();
        storageWindow = nullptr;
    }

    delete2(InventoryRecv::mStorage);
}

void InventoryHandler::clear() const
{
    delete2(InventoryRecv::mStorage);
}

bool InventoryHandler::canSplit(const Item *const item A_UNUSED) const
{
    return false;
}

void InventoryHandler::splitItem(const Item *const item A_UNUSED,
                                 const int amount A_UNUSED) const
{
    // Not implemented for eAthena (possible?)
}

void InventoryHandler::moveItem(const int oldIndex A_UNUSED,
                                const int newIndex A_UNUSED) const
{
    // Not implemented for eAthena (possible?)
}

size_t InventoryHandler::getSize(const InventoryTypeT type) const
{
    switch (type)
    {
        case InventoryType::Inventory:
            return 100;
        case InventoryType::Storage:
            return 0;  // Comes from server after items
        case InventoryType::Trade:
            return 12;
        case InventoryType::Npc:
#ifdef EATHENA_SUPPORT
        case InventoryType::Cart:
        case InventoryType::Vending:
        case InventoryType::Mail:
        case InventoryType::Craft:
#endif
        case InventoryType::TypeEnd:
        default:
            return 0;
    }
}
void InventoryHandler::destroyStorage() const
{
    BLOCK_START("InventoryHandler::closeStorage")
    if (storageWindow)
    {
        InventoryWindow *const inv = storageWindow;
        storageWindow->close();
        inv->unsetInventory();
    }
    BLOCK_END("InventoryHandler::closeStorage")
}

void InventoryHandler::forgotStorage() const
{
    storageWindow = nullptr;
}

void InventoryHandler::pushPickup(const BeingId floorId)
{
    InventoryRecv::mSentPickups.push(floorId);
}

Inventory *InventoryHandler::getStorage() const
{
    return InventoryRecv::mStorage;
}

}  // namespace Ea
