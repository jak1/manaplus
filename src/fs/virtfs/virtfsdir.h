/*
 *  The ManaPlus Client
 *  Copyright (C) 2013-2017  The ManaPlus Developers
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

#ifndef UTILS_VIRTFSDIR_H
#define UTILS_VIRTFSDIR_H
#ifndef USE_PHYSFS

#include "enums/simpletypes/append.h"
#include "enums/simpletypes/skiperror.h"

#include "utils/stringvector.h"

#include "localconsts.h"

struct VirtDirEntry;
struct VirtFile;
struct VirtFsEntry;
struct VirtFsFuncs;
struct VirtList;

namespace VirtFsDir
{
    VirtDirEntry *searchEntryByRoot(const std::string &restrict root);
    VirtDirEntry *searchEntryByPath(const std::string &restrict path);
    VirtFile *openRead(VirtFsEntry *restrict const entry,
                       const std::string &filename);
    VirtFile *openWrite(VirtFsEntry *restrict const entry,
                        const std::string &filename);
    VirtFile *openAppend(VirtFsEntry *restrict const entry,
                         const std::string &filename);
    VirtFile *openReadDirEntry(VirtDirEntry *const entry,
                               const std::string &filename);
    const char *getBaseDir();
    const char *getUserDir();
    VirtFsFuncs *getFuncs();
    bool addToSearchPath(std::string newDir,
                         const Append append);
    bool addToSearchPathSilent(std::string newDir,
                               const Append append,
                               const SkipError skipError);
    bool removeFromSearchPath(std::string oldDir);
    bool removeFromSearchPathSilent(std::string oldDir);
    void init(const std::string &restrict name);
    void initFuncs(VirtFsFuncs *restrict const ptr);
    void deinit();
    std::vector<VirtDirEntry*> &getEntries();
    bool exists(std::string name);
    bool exists(VirtFsEntry *restrict const entry,
                const std::string &fileName,
                const std::string &dirName);
    void enumerate(VirtFsEntry *restrict const entry,
                   const std::string &dirName,
                   StringVect &names);
    VirtList *enumerateFiles(std::string dirName) RETURNS_NONNULL;
    VirtList *enumerateFiles(const std::string &restrict dirName,
                             VirtList *restrict const list) RETURNS_NONNULL;
    bool isDirectory(VirtFsEntry *restrict const entry,
                     const std::string &dirName,
                     bool &isDirFlag);
    bool isDirectoryInternal(const std::string &restrict dirName);
    bool isSymbolicLink(std::string name);
    void freeList(VirtList *restrict const handle);
    VirtFile *openRead(const std::string &restrict filename);
    VirtFile *openWrite(const std::string &restrict filename);
    VirtFile *openAppend(const std::string &restrict filename);
    bool setWriteDir(std::string newDir);
    std::string getRealDir(std::string filename);
    bool getRealDir(VirtFsEntry *restrict const entry,
                    const std::string &filename,
                    const std::string &dirName,
                    std::string &realDir);
    bool mkdir(std::string dirName);
    bool remove(std::string filename);
    void permitLinks(const bool val);
    const char *getLastError();
    int64_t read(VirtFile *restrict const handle,
                 void *restrict const buffer,
                 const uint32_t objSize,
                 const uint32_t objCount);
    int64_t write(VirtFile *restrict const file,
                  const void *restrict const buffer,
                  const uint32_t objSize,
                  const uint32_t objCount);
    int close(VirtFile *restrict const file);
    int64_t fileLength(VirtFile *restrict const file);
    int64_t tell(VirtFile *restrict const file);
    int seek(VirtFile *restrict const file,
             const uint64_t pos);
    int eof(VirtFile *restrict const file);
}  // namespace VirtFsDir

#endif  // USE_PHYSFS
#endif  // UTILS_VIRTFSDIR_H
