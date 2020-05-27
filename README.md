# The Mana Plus Client

![ManaPlus logo](data/icons/manaplus.png)

Version: 1.10.5.21        Date: 2020-05-21

### Build status

Gitlab CI: [![gitlab build status](https://gitlab.com/manaplus2/manaplus/badges/master/pipeline.svg)](https://gitlab.com/manaplus2/manaplus/commits/master)

Cicle CI: [![CircleCI](https://circleci.com/gh/ManaPlus/ManaPlus/tree/master.svg?style=svg)](https://circleci.com/gh/ManaPlus/ManaPlus/tree/master)

Travis CI: [![travis build status](https://travis-ci.com/jak1/manaplus.svg?branch=master)](https://travis-ci.com/jak1/manaplus/builds/)

Azure CI: [![Build Status macosx](https://dev.azure.com/schnuffi0072001/manaplus/_apis/build/status/maxOSX?branchName=master)](https://dev.azure.com/schnuffi0072001/manaplus/_build/latest?definitionId=2&branchName=master)
          [![Build Status windows](https://dev.azure.com/schnuffi0072001/manaplus/_apis/build/status/windows32bit%2664bit?branchName=master)](https://dev.azure.com/schnuffi0072001/manaplus/_build/latest?definitionId=1&branchName=master)

Coverage: [![coverage report](https://gitlab.com/manaplus/manaplus/badges/master/coverage.svg)](http://stats.manaplus.org/gcov/gcc-6.html)

Doxygen: [![doxygen](https://img.shields.io/badge/Docs-Doxygen/manaplus-blue.svg)](http://stats.manaplus.org/docs/index.html)

Coverity: [![coverity](https://scan.coverity.com/projects/318/badge.svg)](https://scan.coverity.com/projects/manaplus)

### Development team:
 - See [AUTHORS](AUTHORS) file for a list

### Dependencies:

 - [SDL](http://libsdl.org/), SDL_image, SDL_mixer, SDL_ttf, SDL_net (Media framework), SDL_gfx
 - [libxml2](http://xmlsoft.org/) or [pugixml](http://pugixml.org/) or [tinyxml2](http://www.grinninglizard.com/tinyxml2/) (XML parsing and writing)
 - [libcurl](http://curl.haxx.se/) (HTTP downloads)
 - [libpng](http://www.libpng.org/) (save screenshots)
 - [zlib](http://zlib.net/) (Archives)

#### Optional dependencies:

 - [gettext](https://www.gnu.org/software/gettext/) (translations)

#### Embedded libs:

  - [catch](https://github.com/philsquared/Catch) and [doctest](https://github.com/onqtam/doctest) for unit testing.
  - [nvwa](https://github.com/adah1972/nvwa) for debug memory leaks.
  - [SaferCPlusPlus](https://github.com/duneroadrunner/SaferCPlusPlus) for debug issues in std::vector usage.

### Additional run time deps:
 - xsel (for clipboard access, used only in SDL 1.2)
 - xdg-open (from xdg-utils, for open url in browser)

### Images and fonts sources:
 - data/icons
 - data/fonts/src


## 0. Index

1. Account
2. Commands
3. Skills
4. Support
5. License

## 1. Account

To create an account you can usually press the "Register" button after
choosing your server. When this doesn't work, visit the website of the server
you'd like to register on, since they may be using an online registration
form instead.

## 2. Commands

### KEYBOARD:

Use arrow keys to move around. Other keys:

- Ctrl               attack
- F1                 toggle the online help
- F2                 toggle profile window
- F3                 toggle inventory window
- F4                 toggle equipment window
- F5                 toggle skills window
- F6                 toggle minimap
- F7                 toggle chat window
- F8                 toggle shortcut window
- F9                 show setup window
- F10                toggle debug window
- Alt + 0-9,-, etc   show emotions.
- S                  sit down / stand up.
- F                  toggle debug pathfinding feature (change map mode)
- P                  take screenshot
- R                  turns on anti-trade function.
- A                  target nearest monster
- H                  hide all non-sticky windows
- Z                  pick up item
- Enter              focus chat window / send message

### MOUSE:

Left click to execute default action: walk, pick up an item, attack a monster
and talk to NPCs (be sure to click on their feet). Right click to show up a
context menu. Holding [Left Shift] prevents from walking when attacking.

### /Commands:

Whispers:
- /closeall          close all whispers.
- /ignoreall         add all whispers to ignore list.
- /msg NICK text
- /whisper NICK text
- /w NICK text       send whisper message to nick.
- /query NICK
- /q NICK            open new whisper tab for nick.

### Actions:
- /help              show small help about chat commands. /target NICK - select nick as target. Can be monster or player nick.
- /outfit N          wear outfit number N.
- /outfit next       wear next outfit.
- /outfit prev       wear previous outfit.
- /emote N           use emotion number N.
- /away
- /away MSG          set away mode.
- /follow NICK       start follow mode.
- /imitation NICK    start imitation mode.
- /heal NICK         heal nick.
- /move X Y          move to X,Y position in short distance.
- /navigate x y      move to position x,y in current map in any distance.
- /mail NICK MSG     send offline message to NICK. Working only in tmw server.
- /disconnect        quick disconnect from server.
- /attack            attack target.
- /undress NICK      remove all clothes from nick. Local effect only.

### Trade:
- /trade NICK        start trade with nick.
- /priceload         load shop price from disc.
- /pricesave         save shop price to disc.

### Player relations:
- /ignore NICK       add nick to ignore list.
- /unignore NICK     Remove nick from ignore list.
- /friend NICK
- /befriend NICK     add nick to friends list.
- /disregard NICK    add nick to disregarded list.
- /neutral NICK      add nick to neutral relation list.
- /erase NICK        add nick to erased list.
- /clear             clear current chat tab.
- /createparty NAME  create party with selected name.
- /me text           send text to chat as /me command in irc.

### Debug:
- /who               print online players number to chat.
- /all               show visible beings list in debug tab.
- /where             print current player position to chat.
- /cacheinfo         show text cache info.
- /dirs              show client directories in debug window.

### Other:
- /help              Displays the list of commands
- /announce          broadcasts a global msg(Gm Cammand only)
- /who               shows how many players are online
- /where             displays the map name your currently on

## 4. Support

If you're having issues with this client, feel free to report them to us.
You can report on [![forum](https://img.shields.io/badge/forum-forums.themanaworld.org-yellow.svg)](http://forums.themanaworld.org/viewforum.php?f=12),
[bugtracker](https://gitlab.com/manaplus/manaplus/issues/) or
[![IRC](https://img.shields.io/badge/IRC-FreeNode/manaplus-yellow.svg)](webchat.freenode.net/?channels=manaplus)
on irc.freenode.net in the #manaplus channel.

If you have feedback about a specific game that uses the ManaPlus client, be sure
to contact the developers of the game instead.

## 5. Licenses

All code in this repository under ![GPL 2+](https://img.shields.io/badge/license-GPL2+-blue.svg) license. Art can be under ![CC-BY-SA](https://img.shields.io/badge/license-CC_BY_SA-blue.svg) or ![GPL 2+](https://img.shields.io/badge/license-GPL2+-blue.svg).

Github, Bitbucked or other non free hostings may use strange TOS what may violate this licenses. If any of this hostings want to violate GPL or/and CC-BY-SA licenses,
they not have any rights to do so. This hostings have rights to remove ManaPlus repositories if they want violate given licenses.

ManaPlus main repository hosted on [gitlab.com](https://gitlab.com/manaplus/manaplus/)
