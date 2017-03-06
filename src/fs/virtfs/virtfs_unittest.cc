/*
 *  The ManaPlus Client
 *  Copyright (C) 2016-2017  The ManaPlus Developers
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

#ifndef USE_PHYSFS

#include "catch.hpp"

#include "fs/files.h"
#include "fs/virtfs.h"
#include "fs/virtfstools.h"
#include "fs/virtlist.h"

#include "fs/virtfs/virtdirentry.h"
#include "fs/virtfs/virtfs.h"
#include "fs/virtfs/ziplocalheader.h"

#include "utils/checkutils.h"
#include "utils/delete2.h"

#include "debug.h"

TEST_CASE("VirtFs1 getEntries")
{
    VirtFs::init(".");
    REQUIRE(VirtFs::getEntries().empty());
    REQUIRE(VirtFs::searchEntryByRootInternal("test") == nullptr);
    VirtFs::deinit();
}

TEST_CASE("VirtFs1 getBaseDir")
{
    VirtFs::init(".");
    REQUIRE(VirtFs::getBaseDir() != nullptr);
    VirtFs::deinit();
}

TEST_CASE("VirtFs1 addDirToSearchPath")
{
    VirtFs::init(".");
    logger = new Logger();
    SECTION("simple 1")
    {
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir1",
            Append_false));
        REQUIRE(VirtFs::searchEntryByRootInternal("dir1/") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal("test/") == nullptr);
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1/");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
    }

    SECTION("simple 2")
    {
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir1/",
            Append_true));
        REQUIRE(VirtFs::searchEntryByRootInternal("dir1/") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal("test/") == nullptr);
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1/");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1/");
    }

    SECTION("simple 3")
    {
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir1",
            Append_false));
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir2",
            Append_false));
        REQUIRE(VirtFs::searchEntryByRootInternal("dir1/") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal("dir2/") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal("test/") == nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir2/");
        REQUIRE(VirtFs::getEntries()[1]->root == "dir1/");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir2");
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir1");
    }

    SECTION("simple 4")
    {
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir1\\",
            Append_true));
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir2",
            Append_true));
        REQUIRE(VirtFs::searchEntryByRootInternal("dir1/") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal("dir2/") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal("test/") == nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1/");
        REQUIRE(VirtFs::getEntries()[1]->root == "dir2/");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1/");
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir2");
    }

    SECTION("simple 5")
    {
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir1",
            Append_true));
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir2",
            Append_true));
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir3/test",
            Append_true));
        REQUIRE(VirtFs::searchEntryByRootInternal("dir1/") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal("dir2/") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal("dir3/test/") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal("test/") == nullptr);
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1/");
        REQUIRE(VirtFs::getEntries()[1]->root == "dir2/");
        REQUIRE(VirtFs::getEntries()[2]->root == "dir3/test/");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir2");
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[2])->userDir == "dir3/test");
    }

    SECTION("simple 6")
    {
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir1",
            Append_true));
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir2",
            Append_true));
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir3/test",
            Append_false));
        REQUIRE(VirtFs::searchEntryByRootInternal("dir1/") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal("dir2/") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal("dir3/test/") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal("test/") == nullptr);
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir3/test/");
        REQUIRE(VirtFs::getEntries()[1]->root == "dir1/");
        REQUIRE(VirtFs::getEntries()[2]->root == "dir2/");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir3/test");
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir1");
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[2])->userDir == "dir2");
    }

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 addZipToSearchPath")
{
    VirtFs::init(".");
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix;
    std::vector<ZipLocalHeader*> headers;
    if (Files::existsLocal(name) == false)
        prefix = "../";

    SECTION("simple 1")
    {
        REQUIRE(VirtFs::addZipToSearchPath(prefix + "data/test/test.zip",
            Append_false));
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test.zip") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test2.zip") == nullptr);
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data/test/test.zip");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
    }

    SECTION("simple 2")
    {
        REQUIRE(VirtFs::addZipToSearchPath(prefix + "data/test/test.zip",
            Append_false));
        REQUIRE(VirtFs::addZipToSearchPath(prefix + "data/test/test2.zip",
            Append_false));
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test.zip") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test2.zip") != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data/test/test2.zip");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data/test/test.zip");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Zip);
    }

    SECTION("simple 3")
    {
        REQUIRE(VirtFs::addZipToSearchPath(prefix + "data/test/test.zip",
            Append_true));
        REQUIRE(VirtFs::addZipToSearchPath(prefix + "data/test/test2.zip",
            Append_true));
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test.zip") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test2.zip") != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data/test/test.zip");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data/test/test2.zip");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Zip);
    }

    SECTION("simple 4")
    {
        REQUIRE(VirtFs::addZipToSearchPath(prefix + "data/test/test.zip",
            Append_false));
        REQUIRE(VirtFs::addDirToSearchPath(prefix + "data/test",
            Append_false));
        REQUIRE(VirtFs::addZipToSearchPath(prefix + "data/test/test2.zip",
            Append_false));
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test.zip") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test2.zip") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/") != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data/test/test2.zip");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data/test/");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[2]->root ==
            prefix + "data/test/test.zip");
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Zip);
    }

    SECTION("simple 5")
    {
        REQUIRE(VirtFs::addZipToSearchPath(prefix + "data/test/test.zip",
            Append_false));
        REQUIRE(VirtFs::addDirToSearchPath(prefix + "data/test",
            Append_false));
        REQUIRE(VirtFs::addZipToSearchPath(prefix + "data/test/test2.zip",
            Append_true));
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test.zip") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test2.zip") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/") != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data/test/");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data/test/test.zip");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[2]->root ==
            prefix + "data/test/test2.zip");
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Zip);
    }

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 removeFromSearchPath")
{
    VirtFs::init(".");
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix;
    std::vector<ZipLocalHeader*> headers;
    if (Files::existsLocal(name) == false)
        prefix = "../";

    SECTION("simple 1")
    {
        REQUIRE_THROWS(VirtFs::removeDirFromSearchPath("dir1"));
        REQUIRE_THROWS(VirtFs::removeDirFromSearchPath("dir1/"));
    }

    SECTION("simple 2")
    {
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir1",
            Append_true));
        REQUIRE_THROWS(VirtFs::removeDirFromSearchPath("dir2"));
        REQUIRE(VirtFs::removeDirFromSearchPath("dir1"));
    }

    SECTION("simple 3")
    {
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir1",
            Append_true));
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir2//dir3",
            Append_true));
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir3",
            Append_false));
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE_THROWS(VirtFs::removeDirFromSearchPath("dir2"));
        REQUIRE(VirtFs::removeDirFromSearchPath("dir1"));
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir3/");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir3");
        REQUIRE(VirtFs::getEntries()[1]->root == "dir2/dir3/");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir2/dir3");
        REQUIRE_THROWS(VirtFs::removeDirFromSearchPath("dir1"));
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir3/");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir3");
        REQUIRE(VirtFs::getEntries()[1]->root == "dir2/dir3/");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[1])->userDir == "dir2/dir3");
        REQUIRE(VirtFs::removeDirFromSearchPath("dir2/dir3"));
        REQUIRE_THROWS(VirtFs::removeDirFromSearchPath("dir2/dir3/"));
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir3/");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir3");
    }

    SECTION("simple 4")
    {
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir1",
            Append_true));
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1/");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
        REQUIRE_THROWS(VirtFs::removeDirFromSearchPath("dir2"));
        REQUIRE(VirtFs::removeDirFromSearchPath("dir1"));
        REQUIRE(VirtFs::getEntries().size() == 0);
        REQUIRE(VirtFs::addDirToSearchPathSilent2("dir1",
            Append_true));
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root == "dir1/");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Dir);
        REQUIRE(static_cast<VirtDirEntry*>(
            VirtFs::getEntries()[0])->userDir == "dir1");
    }

    SECTION("simple 5")
    {
        REQUIRE(VirtFs::addZipToSearchPath(prefix + "data/test/test.zip",
            Append_true));
        REQUIRE(VirtFs::addZipToSearchPath(prefix + "data/test/test2.zip",
            Append_true));

        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test.zip") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test2.zip") != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data/test/test.zip");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data/test/test2.zip");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Zip);

        VirtFs::removeZipFromSearchPath(prefix + "data/test/test.zip");
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test.zip") == nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test2.zip") != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 1);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data/test/test2.zip");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
    }

    SECTION("simple 6")
    {
        REQUIRE(VirtFs::addZipToSearchPath(prefix + "data/test/test.zip",
            Append_false));
        REQUIRE(VirtFs::addDirToSearchPath(prefix + "data/test",
            Append_false));
        REQUIRE(VirtFs::addZipToSearchPath(prefix + "data/test/test2.zip",
            Append_false));

        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test.zip") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test2.zip") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/") != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 3);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data/test/test2.zip");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data/test/");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
        REQUIRE(VirtFs::getEntries()[2]->root ==
            prefix + "data/test/test.zip");
        REQUIRE(VirtFs::getEntries()[2]->type == FsEntryType::Zip);

        VirtFs::removeZipFromSearchPath(prefix + "data/test/test.zip");
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test.zip") == nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/test2.zip") != nullptr);
        REQUIRE(VirtFs::searchEntryByRootInternal(
            prefix + "data/test/") != nullptr);
        REQUIRE(VirtFs::getEntries().size() == 2);
        REQUIRE(VirtFs::getEntries()[0]->root ==
            prefix + "data/test/test2.zip");
        REQUIRE(VirtFs::getEntries()[0]->type == FsEntryType::Zip);
        REQUIRE(VirtFs::getEntries()[1]->root ==
            prefix + "data/test/");
        REQUIRE(VirtFs::getEntries()[1]->type == FsEntryType::Dir);
    }

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 exists")
{
    VirtFs::init(".");
    logger = new Logger();
    const bool dir1 = VirtFs::addDirToSearchPathSilent("data/",
        Append_false);
    VirtFs::addDirToSearchPathSilent("..\\data",
        Append_false);

    REQUIRE(VirtFs::exists("test"));
    REQUIRE(VirtFs::exists("test/"));
    REQUIRE(VirtFs::exists("test/dir1"));
    REQUIRE(VirtFs::exists("test/dir1/"));
    REQUIRE(VirtFs::exists("test/dir") == false);
    REQUIRE(VirtFs::exists("test//units.xml") == true);
    REQUIRE(VirtFs::exists("test/\\units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == false);

    if (dir1 == true)
    {
        VirtFs::addDirToSearchPath("data//test",
            Append_false);
    }
    else
    {
        VirtFs::addDirToSearchPathSilent("..//data\\test",
            Append_false);
    }

    REQUIRE(VirtFs::exists("test") == true);
    REQUIRE(VirtFs::exists("test/dir1"));
    REQUIRE(VirtFs::exists("test/dir1\\"));
    REQUIRE(VirtFs::exists("test/dir") == false);
    REQUIRE(VirtFs::exists("test\\units.xml") == true);
    REQUIRE(VirtFs::exists("test/units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == true);
    REQUIRE(VirtFs::exists("units.xml/") == false);

    if (dir1 == true)
        VirtFs::removeDirFromSearchPathSilent("data/test");
    else
        VirtFs::removeDirFromSearchPathSilent("../data/test");

    REQUIRE(VirtFs::exists("test") == true);
    REQUIRE(VirtFs::exists("test/dir1"));
    REQUIRE(VirtFs::exists("test/dir") == false);
    REQUIRE(VirtFs::exists("test\\units.xml") == true);
    REQUIRE(VirtFs::exists("test/units123.xml") == false);
    REQUIRE(VirtFs::exists("tesQ/units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml") == false);
    REQUIRE(VirtFs::exists("units.xml/") == false);

    REQUIRE_THROWS(VirtFs::exists("test/../units.xml"));

    VirtFs::deinit();
    delete2(logger);
}

static void removeTemp(StringVect &restrict list)
{
    int cnt = 0;
    std::sort(list.begin(), list.end());

    FOR_EACH (StringVectIter, it, list)
    {
        if (*it != "serverlistplus.xml.part")
        {
            logger->log("file: %d %s",
                cnt,
                (*it).c_str());
            cnt ++;
        }
    }

    FOR_EACH (StringVectIter, it, list)
    {
        if (*it == "serverlistplus.xml.part")
        {
            list.erase(it);
            return;
        }
    }
}

TEST_CASE("VirtFs1 getRealDir1")
{
    VirtFs::init(".");
    logger = new Logger();
    REQUIRE(VirtFs::getRealDir(".") == "");
    REQUIRE(VirtFs::getRealDir("..") == "");
    const bool dir1 = VirtFs::addDirToSearchPathSilent("data",
        Append_false);
    REQUIRE((dir1 || VirtFs::addDirToSearchPathSilent("../data",
        Append_false)) == true);
    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("test") == "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("test\\test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("test//test.txt") ==
            "data");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("test") == "../data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "../data");
        REQUIRE(VirtFs::getRealDir("test\\test.txt") ==
            "../data");
        REQUIRE(VirtFs::getRealDir("test//test.txt") ==
            "../data");
    }
    REQUIRE(VirtFs::getRealDir("zzz") == "");

    VirtFs::addDirToSearchPathSilent("data/test",
        Append_false);
    VirtFs::addDirToSearchPathSilent("../data/test",
        Append_false);
    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("test") == "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("test\\test.txt") ==
            "data");
        REQUIRE(VirtFs::getRealDir("test.txt") ==
            "data/test");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("test") == "../data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "../data");
        REQUIRE(VirtFs::getRealDir("test\\test.txt") ==
            "../data");
        REQUIRE(VirtFs::getRealDir("test.txt") ==
            "../data/test");
    }
    REQUIRE(VirtFs::getRealDir("zzz") == "");

    VirtFs::removeDirFromSearchPathSilent("data/test");
    VirtFs::removeDirFromSearchPathSilent("../data/test");

    if (dir1 == true)
    {
        REQUIRE(VirtFs::getRealDir("test") == "data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "data");
    }
    else
    {
        REQUIRE(VirtFs::getRealDir("test") == "../data");
        REQUIRE(VirtFs::getRealDir("test/test.txt") ==
            "../data");
    }
    REQUIRE(VirtFs::getRealDir("zzz") == "");

    VirtFs::removeDirFromSearchPathSilent("data");
    VirtFs::removeDirFromSearchPathSilent("../data");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 getRealDir2")
{
    VirtFs::init(".");
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix("data/test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;
    VirtFs::addZipToSearchPath(prefix + "test2.zip",
        Append_false);

    REQUIRE(VirtFs::getRealDir(".") == "");
    REQUIRE(VirtFs::getRealDir("..") == "");
    REQUIRE(VirtFs::getRealDir("test.txt") == prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir/1") == prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir\\dye.png") ==
        prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("zzz") == "");

    VirtFs::addZipToSearchPath(prefix + "test.zip",
        Append_false);
    REQUIRE(VirtFs::getRealDir("dir//dye.png") ==
        prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir///hide.png") ==
        prefix + "test.zip");
    REQUIRE(VirtFs::getRealDir("dir\\\\brimmedhat.png") ==
        prefix + "test.zip");
    REQUIRE(VirtFs::getRealDir("zzz") == "");

    VirtFs::removeZipFromSearchPath(prefix + "test.zip");

    REQUIRE(VirtFs::getRealDir("dir/brimmedhat.png") == "");
    REQUIRE(VirtFs::getRealDir("test.txt") == prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("dir//dye.png") ==
        prefix + "test2.zip");
    REQUIRE(VirtFs::getRealDir("zzz") == "");

    VirtFs::removeZipFromSearchPath(prefix + "test2.zip");
    VirtFs::deinit();
    delete2(logger);
}

static bool inList(VirtList *list,
                   const std::string &name)
{
    FOR_EACH (StringVectCIter, it, list->names)
    {
        if (*it == name)
            return true;
    }
    return false;
}

TEST_CASE("VirtFs1 enumerateFiles1")
{
    VirtFs::init(".");
    logger = new Logger;

    VirtFs::addDirToSearchPathSilent("data",
        Append_false);
    VirtFs::addDirToSearchPathSilent("../data",
        Append_false);

    VirtList *list = nullptr;

    const int cnt1 = VirtFs::exists("test/test2.txt") ? 28 : 27;
    const int cnt2 = 28;

    VirtFs::permitLinks(false);
    list = VirtFs::enumerateFiles("test");
    removeTemp(list->names);
    const size_t sz = list->names.size();
    REQUIRE(sz == cnt1);
    VirtFs::freeList(list);

    VirtFs::permitLinks(true);
    list = VirtFs::enumerateFiles("test/");
    removeTemp(list->names);
    REQUIRE(list->names.size() == cnt2);
    VirtFs::freeList(list);

    VirtFs::permitLinks(true);
    list = VirtFs::enumerateFiles("test/units.xml");
    REQUIRE(list->names.size() == 0);
    VirtFs::freeList(list);

    VirtFs::permitLinks(false);
    list = VirtFs::enumerateFiles("test\\");
    removeTemp(list->names);
    REQUIRE(list->names.size() == cnt1);
    VirtFs::freeList(list);

    VirtFs::removeDirFromSearchPathSilent("data");
    VirtFs::removeDirFromSearchPathSilent("../data");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 enumerateFiles2")
{
    VirtFs::init(".");
    logger = new Logger;

    VirtFs::addDirToSearchPathSilent("data/test/dir1",
        Append_false);
    VirtFs::addDirToSearchPathSilent("../data/test/dir1",
        Append_false);

    VirtList *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    const size_t sz = list->names.size();
    REQUIRE(list->names.size() == 5);
    REQUIRE(inList(list, "file1.txt"));
    REQUIRE_FALSE(inList(list, "file2.txt"));
    VirtFs::freeList(list);
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 enumerateFiles3")
{
    VirtFs::init(".");
    logger = new Logger;

    VirtFs::addDirToSearchPathSilent("data/test/dir1",
        Append_false);
    VirtFs::addDirToSearchPathSilent("../data/test/dir1",
        Append_false);
    VirtFs::addDirToSearchPathSilent("data/test/dir2",
        Append_false);
    VirtFs::addDirToSearchPathSilent("../data/test/dir2",
        Append_false);

    VirtList *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    const size_t sz = list->names.size();
    REQUIRE(list->names.size() == 6);
    REQUIRE(inList(list, "file1.txt"));
    REQUIRE(inList(list, "file2.txt"));
    VirtFs::freeList(list);
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip enumerateFiles4")
{
    VirtFs::init(".");
    logger = new Logger;
    std::string name("data/test/test.zip");
    std::string prefix("data\\test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::addZipToSearchPath(prefix + "test.zip",
        Append_false);

    VirtList *list = nullptr;

    list = VirtFs::enumerateFiles("dir");
    REQUIRE(list->names.size() == 2);
    REQUIRE(inList(list, "brimmedhat.png"));
    REQUIRE(inList(list, "hide.png"));
    VirtFs::freeList(list);

    VirtFs::removeZipFromSearchPath(prefix + "test.zip");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip enumerateFiles5")
{
    VirtFs::init(".");
    logger = new Logger;
    std::string name("data/test/test.zip");
    std::string prefix("data//test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::addZipToSearchPath(prefix + "test.zip",
        Append_true);
    VirtFs::addZipToSearchPath(prefix + "test2.zip",
        Append_true);

    VirtList *list = nullptr;

    list = VirtFs::enumerateFiles("dir");
    FOR_EACH (StringVectCIter, it, list->names)
    {
        logger->log("filename: " + *it);
    }

    REQUIRE(list->names.size() == 5);
    REQUIRE(inList(list, "brimmedhat.png"));
    REQUIRE(inList(list, "hide.png"));
    REQUIRE(inList(list, "1"));
    REQUIRE(inList(list, "gpl"));
    REQUIRE(inList(list, "dye.png"));
    VirtFs::freeList(list);

    VirtFs::removeZipFromSearchPath(prefix + "test.zip");
    VirtFs::removeZipFromSearchPath(prefix + "test2.zip");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip enumerateFiles6")
{
    VirtFs::init(".");
    logger = new Logger;
    std::string name("data/test/test.zip");
    std::string prefix("data\\test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::addZipToSearchPath(prefix + "test.zip",
        Append_false);

    VirtList *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    REQUIRE(list->names.size() == 1);
    REQUIRE(inList(list, "dir"));
    VirtFs::freeList(list);

    VirtFs::removeZipFromSearchPath(prefix + "test.zip");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip enumerateFiles7")
{
    VirtFs::init(".");
    logger = new Logger;
    std::string name("data/test/test.zip");
    std::string prefix("data\\test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::addZipToSearchPath(prefix + "test2.zip",
        Append_false);

    VirtList *list = nullptr;

    list = VirtFs::enumerateFiles("/");
    REQUIRE(list->names.size() == 4);
    REQUIRE(inList(list, "dir"));
    REQUIRE(inList(list, "dir2"));
    REQUIRE(inList(list, "test.txt"));
    REQUIRE(inList(list, "units.xml"));
    VirtFs::freeList(list);

    VirtFs::removeZipFromSearchPath(prefix + "test2.zip");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFsZip enumerateFiles8")
{
    VirtFs::init(".");
    logger = new Logger;
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::addZipToSearchPath(prefix + "data/test/test2.zip",
        Append_false);
    VirtFs::addDirToSearchPathSilent(prefix + "data/test",
        Append_false);

    VirtList *list = nullptr;

    list = VirtFs::enumerateFiles("dir2");
    REQUIRE(list->names.size() >= 6);
    REQUIRE(inList(list, "file1.txt"));
    REQUIRE(inList(list, "file2.txt"));
    REQUIRE(inList(list, "hide.png"));
    REQUIRE(inList(list, "paths.xml"));
    REQUIRE(inList(list, "test.txt"));
    REQUIRE(inList(list, "units.xml"));
    VirtFs::freeList(list);

    VirtFs::removeZipFromSearchPath(prefix + "data/test/test2.zip");
    VirtFs::removeDirFromSearchPath(prefix + "data/test");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 isDirectory1")
{
    VirtFs::init(".");
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::addDirToSearchPath(prefix + "data",
        Append_false);

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("test//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("test//units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test") == true);
    REQUIRE(VirtFs::isDirectory("test/") == true);
    REQUIRE(VirtFs::isDirectory("test//") == true);
    REQUIRE(VirtFs::isDirectory("test/dir1") == true);
    REQUIRE(VirtFs::isDirectory("test//dir1") == true);
    REQUIRE(VirtFs::isDirectory("test//dir1/") == true);
    REQUIRE(VirtFs::isDirectory("test//dir1//") == true);
    REQUIRE(VirtFs::isDirectory("test\\dir1/") == true);
    REQUIRE(VirtFs::isDirectory("test/dir1//") == true);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("testQ/") == false);
    REQUIRE(VirtFs::isDirectory("testQ//") == false);

    VirtFs::addDirToSearchPath(prefix + "data/test",
        Append_false);

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("test//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test") == true);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("test/dir1") == true);
    REQUIRE(VirtFs::isDirectory("test\\dir1") == true);

    VirtFs::removeDirFromSearchPath(prefix + "data/test");

    REQUIRE(VirtFs::isDirectory("test/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("test/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("test") == true);
    REQUIRE(VirtFs::isDirectory("test/") == true);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("test/dir1") == true);

    VirtFs::removeDirFromSearchPathSilent(prefix + "data");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 isDirectory2")
{
    VirtFs::init(".");
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::addZipToSearchPath(prefix + "data/test/test2.zip",
        Append_false);

    REQUIRE(VirtFs::isDirectory("dir2/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2/units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("dir2//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2//units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir") == true);
    REQUIRE(VirtFs::isDirectory("dir2/") == true);
    REQUIRE(VirtFs::isDirectory("dir2//") == true);
    REQUIRE(VirtFs::isDirectory("dir/1") == true);
    REQUIRE(VirtFs::isDirectory("dir//1") == true);
    REQUIRE(VirtFs::isDirectory("dir\\1/") == true);
    REQUIRE(VirtFs::isDirectory("dir/1") == true);
    REQUIRE(VirtFs::isDirectory("dir/1/zzz") == false);
    REQUIRE(VirtFs::isDirectory("test/dir1\\") == false);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("testQ/") == false);
    REQUIRE(VirtFs::isDirectory("testQ//") == false);

    VirtFs::addZipToSearchPath(prefix + "data/test/test.zip",
        Append_false);

    REQUIRE(VirtFs::isDirectory("dir2/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2/units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("dir2\\units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2//units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir") == true);
    REQUIRE(VirtFs::isDirectory("dir2/") == true);
    REQUIRE(VirtFs::isDirectory("dir2\\") == true);
    REQUIRE(VirtFs::isDirectory("dir/1") == true);
    REQUIRE(VirtFs::isDirectory("dir//1") == true);
    REQUIRE(VirtFs::isDirectory("dir//1/") == true);
    REQUIRE(VirtFs::isDirectory("dir/1") == true);
    REQUIRE(VirtFs::isDirectory("dir/1/zzz") == false);
    REQUIRE(VirtFs::isDirectory("test/dir1//") == false);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("testQ/") == false);
    REQUIRE(VirtFs::isDirectory("testQ//") == false);

    VirtFs::removeZipFromSearchPath(prefix + "data/test/test2.zip");

    REQUIRE(VirtFs::isDirectory("dir2/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2/units.xml/") == false);
    REQUIRE(VirtFs::isDirectory("dir2//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2/units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir2//units123.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ/units.xml") == false);
    REQUIRE(VirtFs::isDirectory("tesQ//units.xml") == false);
    REQUIRE(VirtFs::isDirectory("units.xml") == false);
    REQUIRE(VirtFs::isDirectory("dir") == true);
    REQUIRE(VirtFs::isDirectory("dir2/") == false);
    REQUIRE(VirtFs::isDirectory("dir2//") == false);
    REQUIRE(VirtFs::isDirectory("dir/1") == false);
    REQUIRE(VirtFs::isDirectory("dir\\1") == false);
    REQUIRE(VirtFs::isDirectory("dir//1/") == false);
    REQUIRE(VirtFs::isDirectory("dir/1") == false);
    REQUIRE(VirtFs::isDirectory("dir/1/zzz") == false);
    REQUIRE(VirtFs::isDirectory("test/dir1//") == false);
    REQUIRE(VirtFs::isDirectory("testQ") == false);
    REQUIRE(VirtFs::isDirectory("testQ/") == false);
    REQUIRE(VirtFs::isDirectory("testQ//") == false);

    VirtFs::removeZipFromSearchPath(prefix + "data/test/test.zip");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 openRead1")
{
    VirtFs::init(".");
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::addDirToSearchPath(prefix + "data",
        Append_false);

    VirtFile *file = nullptr;

    file = VirtFs::openRead("test/units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("test\\units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("test/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);

    VirtFs::addDirToSearchPath(prefix + "data/test",
        Append_false);

    file = VirtFs::openRead("test/units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("test/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);

    VirtFs::removeDirFromSearchPath(prefix + "data/test");

    file = VirtFs::openRead("test/units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("test/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);

    VirtFs::removeDirFromSearchPath(prefix + "data");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 openRead2")
{
    VirtFs::init(".");
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix("data/test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::addZipToSearchPath(prefix + "test2.zip",
        Append_false);

    VirtFile *file = nullptr;

    file = VirtFs::openRead("dir2/units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir2\\units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir2/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml1");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("dir/brimmedhat.png");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("dir//brimmedhat.png");
    REQUIRE(file == nullptr);

    VirtFs::addZipToSearchPath(prefix + "test.zip",
        Append_false);

    file = VirtFs::openRead("dir2/units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir2//units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir2/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml1");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("dir/brimmedhat.png");
    REQUIRE(file != nullptr);
    VirtFs::close(file);

    VirtFs::removeZipFromSearchPath(prefix + "test.zip");

    file = VirtFs::openRead("dir2/units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir2\\/\\units.xml");
    REQUIRE(file != nullptr);
    VirtFs::close(file);
    file = VirtFs::openRead("dir2/units123.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("tesQ/units.xml");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("units.xml1");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("testQ");
    REQUIRE(file == nullptr);
    file = VirtFs::openRead("dir/brimmedhat.png");
    REQUIRE(file == nullptr);

    VirtFs::removeZipFromSearchPath(prefix + "test2.zip");

    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 permitLinks")
{
    VirtFs::init(".");
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::addDirToSearchPath(prefix + "data",
        Append_false);

    const int cnt1 = VirtFs::exists("test/test2.txt") ? 26 : 25;
    const int cnt2 = 26;

    StringVect list;
    VirtFs::permitLinks(false);
    VirtFs::getFiles("test", list);
    removeTemp(list);
    const size_t sz = list.size();
    REQUIRE(sz == cnt1);

    list.clear();
    VirtFs::permitLinks(true);
    VirtFs::getFiles("test", list);
    removeTemp(list);
    REQUIRE(list.size() == cnt2);

    list.clear();
    VirtFs::permitLinks(false);
    VirtFs::getFiles("test", list);
    removeTemp(list);
    REQUIRE(list.size() == cnt1);

    VirtFs::removeDirFromSearchPathSilent(prefix + "data");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 read1")
{
    VirtFs::init(".");
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix;
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::addDirToSearchPath(prefix + "data",
        Append_false);

    VirtFile *file = VirtFs::openRead("test/test.txt");
    REQUIRE(file != nullptr);
    REQUIRE(VirtFs::fileLength(file) == 23);
    const int fileSize = VirtFs::fileLength(file);

    void *restrict buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFs::read(file, buffer, 1, fileSize) == fileSize);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 1\ntest line 2") == 0);
    REQUIRE(VirtFs::tell(file) == fileSize);
    REQUIRE(VirtFs::eof(file) != 0);

    free(buffer);
    buffer = calloc(fileSize + 1, 1);
    REQUIRE(VirtFs::seek(file, 12) != 0);
    REQUIRE(VirtFs::eof(file) == 0);
    REQUIRE(VirtFs::tell(file) == 12);
    REQUIRE(VirtFs::read(file, buffer, 1, 11) == 11);
    REQUIRE(strcmp(static_cast<char*>(buffer),
        "test line 2") == 0);
    REQUIRE(VirtFs::eof(file) != 0);

    VirtFs::close(file);
    free(buffer);

    VirtFs::removeDirFromSearchPath(prefix + "data");
    VirtFs::deinit();
    delete2(logger);
}

TEST_CASE("VirtFs1 read2")
{
    VirtFs::init(".");
    logger = new Logger();
    std::string name("data/test/test.zip");
    std::string prefix("data/test/");
    if (Files::existsLocal(name) == false)
        prefix = "../" + prefix;

    VirtFs::addZipToSearchPath(prefix + "test2.zip",
        Append_false);
    VirtFile *file = nullptr;
    void *restrict buffer = nullptr;

    SECTION("test 1")
    {
        file = VirtFs::openRead("dir2//test.txt");
        REQUIRE(file != nullptr);
        REQUIRE(VirtFs::fileLength(file) == 23);
        const int fileSize = VirtFs::fileLength(file);

        buffer = calloc(fileSize + 1, 1);
        REQUIRE(VirtFs::read(file, buffer, 1, fileSize) == fileSize);
        REQUIRE(strcmp(static_cast<char*>(buffer),
            "test line 1\ntest line 2") == 0);
        REQUIRE(VirtFs::tell(file) == fileSize);
        REQUIRE(VirtFs::eof(file) != 0);
    }

    SECTION("test 2")
    {
        file = VirtFs::openRead("dir2\\/test.txt");
        REQUIRE(file != nullptr);
        REQUIRE(VirtFs::fileLength(file) == 23);
        const int fileSize = VirtFs::fileLength(file);

        buffer = calloc(fileSize + 1, 1);
        REQUIRE(VirtFs::seek(file, 12) != 0);
        REQUIRE(VirtFs::eof(file) == 0);
        REQUIRE(VirtFs::tell(file) == 12);
        REQUIRE(VirtFs::read(file, buffer, 1, 11) == 11);
        REQUIRE(strcmp(static_cast<char*>(buffer),
            "test line 2") == 0);
        REQUIRE(VirtFs::eof(file) != 0);
    }

    SECTION("test 3")
    {
        file = VirtFs::openRead("dir2//test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = VirtFs::fileLength(file);

        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize; f ++)
        {
            REQUIRE(VirtFs::seek(file, f) != 0);
            REQUIRE(VirtFs::eof(file) == 0);
            REQUIRE(VirtFs::tell(file) == f);
        }
    }

    SECTION("test 4")
    {
        file = VirtFs::openRead("dir2/test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = VirtFs::fileLength(file);
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f ++)
        {
            REQUIRE(VirtFs::read(file, buffer, 1, 1) == 1);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(VirtFs::eof(file) == 0);
            REQUIRE(VirtFs::tell(file) == f + 1);
        }
        REQUIRE(VirtFs::read(file, buffer, 1, 1) == 1);
        REQUIRE(static_cast<char*>(buffer)[0] == str[22]);
        REQUIRE(VirtFs::eof(file) != 0);
        REQUIRE(VirtFs::tell(file) == fileSize);
    }

    SECTION("test 5")
    {
        file = VirtFs::openRead("dir2\\\\test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = VirtFs::fileLength(file);
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f += 2)
        {
            REQUIRE(VirtFs::read(file, buffer, 2, 1) == 1);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(static_cast<char*>(buffer)[1] == str[f + 1]);
            REQUIRE(VirtFs::eof(file) == 0);
            REQUIRE(VirtFs::tell(file) == f + 2);
        }
        REQUIRE(VirtFs::eof(file) == 0);
        REQUIRE(VirtFs::tell(file) == 22);
        REQUIRE(VirtFs::read(file, buffer, 2, 1) == 0);
        REQUIRE(VirtFs::eof(file) == 0);
    }

    SECTION("test 6")
    {
        file = VirtFs::openRead("dir2//test.txt");
        REQUIRE(file != nullptr);
        const int fileSize = VirtFs::fileLength(file);
        const char *restrict const str = "test line 1\ntest line 2";
        buffer = calloc(fileSize + 1, 1);
        for (int f = 0; f < fileSize - 1; f += 2)
        {
            REQUIRE(VirtFs::read(file, buffer, 1, 2) == 2);
            REQUIRE(static_cast<char*>(buffer)[0] == str[f]);
            REQUIRE(static_cast<char*>(buffer)[1] == str[f + 1]);
            REQUIRE(VirtFs::eof(file) == 0);
            REQUIRE(VirtFs::tell(file) == f + 2);
        }
        REQUIRE(VirtFs::eof(file) == 0);
        REQUIRE(VirtFs::tell(file) == 22);
        REQUIRE(VirtFs::read(file, buffer, 1, 2) == 1);
        REQUIRE(static_cast<char*>(buffer)[0] == str[22]);
        REQUIRE(VirtFs::eof(file) != 0);
    }

    VirtFs::close(file);
    free(buffer);
    VirtFs::removeZipFromSearchPath(prefix + "test2.zip");
    VirtFs::deinit();
    delete2(logger);
}

#endif  // USE_PHYSFS
