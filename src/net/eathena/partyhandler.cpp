/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  Lloyd Bryant <lloyd_bryant@netzero.net>
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

#include "net/eathena/partyhandler.h"

#include "actormanager.h"
#include "notifymanager.h"
#include "party.h"

#include "being/localplayer.h"

#include "gui/windows/chatwindow.h"
#include "gui/windows/socialwindow.h"

#include "net/ea/gui/partytab.h"

#include "net/eathena/messageout.h"
#include "net/eathena/protocol.h"

#include "resources/notifytypes.h"

#include "debug.h"

extern Net::PartyHandler *partyHandler;

namespace EAthena
{

PartyHandler::PartyHandler() :
    MessageHandler(),
    Ea::PartyHandler()
{
    static const uint16_t _messages[] =
    {
        SMSG_PARTY_CREATE,
        SMSG_PARTY_INFO,
        SMSG_PARTY_INVITE_RESPONSE,
        SMSG_PARTY_INVITED,
        SMSG_PARTY_SETTINGS,
        SMSG_PARTY_MOVE,
        SMSG_PARTY_LEAVE,
        SMSG_PARTY_UPDATE_HP,
        SMSG_PARTY_UPDATE_COORDS,
        SMSG_PARTY_MESSAGE,
        SMSG_PARTY_INVITATION_STATS,
        SMSG_PARTY_MEMBER_INFO,
        0
    };
    handledMessages = _messages;
    partyHandler = this;
}

PartyHandler::~PartyHandler()
{
}

void PartyHandler::handleMessage(Net::MessageIn &msg)
{
    switch (msg.getId())
    {
        case SMSG_PARTY_CREATE:
            processPartyCreate(msg);
            break;
        case SMSG_PARTY_INFO:
            processPartyInfo(msg);
            break;
        case SMSG_PARTY_INVITE_RESPONSE:
            processPartyInviteResponse(msg);
            break;
        case SMSG_PARTY_INVITED:
            processPartyInvited(msg);
            break;
        case SMSG_PARTY_SETTINGS:
            processPartySettings(msg);
            break;
        case SMSG_PARTY_MOVE:
            processPartyMove(msg);
            break;
        case SMSG_PARTY_LEAVE:
            processPartyLeave(msg);
            break;
        case SMSG_PARTY_UPDATE_HP:
            processPartyUpdateHp(msg);
            break;
        case SMSG_PARTY_UPDATE_COORDS:
            processPartyUpdateCoords(msg);
            break;
        case SMSG_PARTY_MESSAGE:
            processPartyMessage(msg);
            break;
        case SMSG_PARTY_INVITATION_STATS:
            processPartyInvitationStats(msg);
            break;
        case SMSG_PARTY_MEMBER_INFO:
            processPartyMemberInfo(msg);
            break;

        default:
            break;
    }
}

void PartyHandler::create(const std::string &name) const
{
    MessageOut outMsg(CMSG_PARTY_CREATE);
    outMsg.writeString(name.substr(0, 23), 24);
}

void PartyHandler::invite(const std::string &name) const
{
    if (!actorManager)
        return;

    const Being *const being = actorManager->findBeingByName(
        name, ActorType::Player);
    if (being)
    {
        MessageOut outMsg(CMSG_PARTY_INVITE);
        outMsg.writeInt32(being->getId(), "account id");
    }
    else
    {
        MessageOut outMsg(CMSG_PARTY_INVITE2);
        outMsg.writeString(name, 24, "nick");
    }
}

void PartyHandler::inviteResponse(const std::string &inviter A_UNUSED,
                                  const bool accept) const
{
    if (localPlayer)
    {
        MessageOut outMsg(CMSG_PARTY_INVITED2);
        outMsg.writeInt32(localPlayer->getId());
        outMsg.writeInt8(accept ? 1 : 0);
    }
}

void PartyHandler::leave() const
{
    MessageOut outMsg(CMSG_PARTY_LEAVE);
}

void PartyHandler::kick(const Being *const being) const
{
    if (being)
    {
        MessageOut outMsg(CMSG_PARTY_KICK);
        outMsg.writeInt32(being->getId());
        outMsg.writeString("", 24);  // unused
    }
}

void PartyHandler::kick(const std::string &name) const
{
    if (!Ea::taParty)
        return;

    const PartyMember *const m = Ea::taParty->getMember(name);
    if (!m)
    {
        NotifyManager::notify(NotifyTypes::PARTY_USER_NOT_IN_PARTY, name);
        return;
    }

    MessageOut outMsg(CMSG_PARTY_KICK);
    outMsg.writeInt32(m->getID());
    outMsg.writeString(name, 24);  // unused
}

void PartyHandler::chat(const std::string &text) const
{
    MessageOut outMsg(CMSG_PARTY_MESSAGE);
    const std::string mes = std::string(localPlayer->getName()).append(
        " : ").append(text);

    outMsg.writeInt16(static_cast<int16_t>(mes.length() + 4 + 1), "len");
    outMsg.writeString(mes, static_cast<int>(mes.length()), "nick : message");
    outMsg.writeInt8(0, "null char");
}

// +++ must be 3 types item, exp, pickup
void PartyHandler::setShareExperience(const Net::PartyShare::Type share) const
{
    if (share == Net::PartyShare::NOT_POSSIBLE)
        return;

    MessageOut outMsg(CMSG_PARTY_SETTINGS);
    outMsg.writeInt16(static_cast<int16_t>(share));
    outMsg.writeInt16(static_cast<int16_t>(mShareItems));
}

// +++ must be 3 types item, exp, pickup
void PartyHandler::setShareItems(const Net::PartyShare::Type share) const
{
    if (share == Net::PartyShare::NOT_POSSIBLE)
        return;

    MessageOut outMsg(CMSG_PARTY_SETTINGS);
    outMsg.writeInt16(static_cast<int16_t>(mShareExp));
    outMsg.writeInt16(static_cast<int16_t>(share));
}

void PartyHandler::processPartyInvitationStats(Net::MessageIn &msg)
{
    msg.readUInt8("allow party");
}

void PartyHandler::processPartyMemberInfo(Net::MessageIn &msg)
{
    const int id = msg.readInt32("account id");
    const bool leader = msg.readInt32("leader") == 0U;
    const int x = msg.readInt16("x");
    const int y = msg.readInt16("y");
    const bool online = msg.readInt8("online") == 0U;
    msg.readString(24, "party name");
    const std::string nick = msg.readString(24, "player name");
    const std::string map = msg.readString(16, "map name");
    msg.readInt8("party.item&1");
    msg.readInt8("party.item&2");

    if (!Ea::taParty)
        return;

    PartyMember *const member = Ea::taParty->addMember(id, nick);
    if (member)
    {
        if (Ea::partyTab && member->getOnline() != online)
            Ea::partyTab->showOnline(nick, online);
        member->setLeader(leader);
        member->setOnline(online);
        member->setMap(map);
        member->setX(x);
        member->setY(y);
    }
}

void PartyHandler::processPartySettings(Net::MessageIn &msg)
{
    if (!Ea::partyTab)
    {
        if (!chatWindow)
            return;

        createTab();
    }

    msg.readInt32("party exp");
    const int16_t exp = msg.readInt8("exp");
    const int16_t item = msg.readInt8("item");
    processPartySettingsContinue(exp, item);
}

void PartyHandler::processPartyInfo(Net::MessageIn &msg) const
{
    bool isOldParty = false;
    std::set<std::string> names;
    std::set<std::string> onlineNames;
    if (!Ea::taParty)
    {
        logger->log1("error: party empty in SMSG_PARTY_INFO");
        Ea::taParty = Party::getParty(1);
    }
    if (!Ea::partyTab)
        createTab();

    if (Ea::taParty)
    {
        if (Ea::taParty->getNumberOfElements() > 1)
        {
            isOldParty = true;
            Ea::taParty->getNamesSet(names);
            const Party::MemberList *const members = Ea::taParty->getMembers();
            FOR_EACHP (Party::MemberList::const_iterator, it, members)
            {
                if ((*it)->getOnline())
                    onlineNames.insert((*it)->getName());
            }
            if (localPlayer)
                onlineNames.insert(localPlayer->getName());
        }
    }

    if (!localPlayer)
        logger->log1("error: localPlayer==0 in SMSG_PARTY_INFO");

    if (Ea::taParty)
        Ea::taParty->clearMembers();

    const int length = msg.readInt16("len");
    if (Ea::taParty)
        Ea::taParty->setName(msg.readString(24, "party name"));

    const int count = (length - 28) / 46;
    if (localPlayer && Ea::taParty)
    {
        localPlayer->setParty(Ea::taParty);
        localPlayer->setPartyName(Ea::taParty->getName());
    }

    for (int i = 0; i < count; i++)
    {
        const int id = msg.readInt32("account id");
        std::string nick = msg.readString(24, "nick");
        std::string map = msg.readString(16, "map name");
        const bool leader = msg.readUInt8("leader") == 0U;
        const bool online = msg.readUInt8("online") == 0U;

        if (Ea::taParty)
        {
            bool joined(false);

            if (isOldParty)
            {
                if (names.find(nick) == names.end())
                {
                    NotifyManager::notify(NotifyTypes::PARTY_USER_JOINED,
                        nick);
                    joined = true;
                }
            }
            PartyMember *const member = Ea::taParty->addMember(id, nick);
            if (member)
            {
                if (!joined && Ea::partyTab)
                {
                    if (!names.empty() && ((onlineNames.find(nick)
                        == onlineNames.end() && online)
                        || (onlineNames.find(nick) != onlineNames.end()
                        && !online)))
                    {
                        Ea::partyTab->showOnline(nick, online);
                    }

                    member->setLeader(leader);
                    member->setOnline(online);
                    member->setMap(map);
                }
                else
                {
                    member->setLeader(leader);
                    member->setOnline(online);
                    member->setMap(map);
                }
            }
        }
    }

    if (Ea::taParty)
        Ea::taParty->sort();

    if (localPlayer && Ea::taParty)
    {
        localPlayer->setParty(Ea::taParty);
        localPlayer->setPartyName(Ea::taParty->getName());
        if (socialWindow)
            socialWindow->updateParty();
    }
}

void PartyHandler::processPartyMessage(Net::MessageIn &msg) const
{
    const int msgLength = msg.readInt16() - 8;
    if (msgLength <= 0)
        return;

    const int id = msg.readInt32("id");
    std::string chatMsg = msg.readString(msgLength, "message");

    const size_t pos = chatMsg.find(" : ", 0);
    if (pos != std::string::npos)
        chatMsg.erase(0, pos + 3);

    if (Ea::taParty && Ea::partyTab)
    {
        const PartyMember *const member = Ea::taParty->getMember(id);
        if (member)
        {
            Ea::partyTab->chatLog(member->getName(), chatMsg);
        }
        else
        {
            NotifyManager::notify(NotifyTypes::PARTY_UNKNOWN_USER_MSG,
                chatMsg);
        }
    }
}

void PartyHandler::processPartyInviteResponse(Net::MessageIn &msg) const
{
    if (!Ea::partyTab)
        return;

    const std::string nick = msg.readString(24, "nick");

    switch (msg.readInt32("result"))
    {
        case 0:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_ALREADY_MEMBER,
                nick);
            break;
        case 1:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_REFUSED, nick);
            break;
        case 2:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_DONE, nick);
            break;
        case 3:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_PARTY_FULL,
                nick);
            break;
        case 4:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_PARTY_SAME_ACCOUNT,
                nick);
            break;
        case 5:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_PARTY_BLOCKED_INVITE,
                nick);
            break;
        case 7:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_PARTY_NOT_ONLINE,
                nick);
            break;
        default:
            NotifyManager::notify(NotifyTypes::PARTY_INVITE_ERROR, nick);
            break;
    }
}

void PartyHandler::changeLeader(const std::string &name) const
{
    const Being *const being = actorManager->findBeingByName(
        name, ActorType::Player);
    if (!being)
        return;
    MessageOut outMsg(CMSG_PARTY_CHANGE_LEADER);
    outMsg.writeInt32(being->getId());
}

}  // namespace EAthena
