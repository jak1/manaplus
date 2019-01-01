/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2019  The ManaPlus Developers
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

#ifndef LISTENERS_NEWPINCODELISTENER_H
#define LISTENERS_NEWPINCODELISTENER_H

#include "listeners/actionlistener.h"

#include "localconsts.h"

class NewPincodeListener final : public ActionListener
{
    public:
        NewPincodeListener() :
            ActionListener()
        { }

        A_DELETE_COPY(NewPincodeListener)

        void action(const ActionEvent &event) override final;
};

extern NewPincodeListener newPincodeListener;

#endif  // LISTENERS_NEWPINCODELISTENER_H
