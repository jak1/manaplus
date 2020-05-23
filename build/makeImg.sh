# appimage package
dir=`pwd` # current dir
rel=".." # relative to root dir
appdir="AppDir" # AppDir

wget "https://gitlab.com/mana-launcher/mplusbuilder/-/raw/master/linuxdeploy-x86_64.AppImage"

chmod +x linuxdeploy-x86_64.AppImage

./linuxdeploy-x86_64.AppImage --appimage-extract

if [ ! -d "$rel/$appdir" ]; then
  mkdir "$rel/$appdir"
fi

cp -r "$dir/run/share/games/manaplus/data $rel/$appdir/data"

echo -e '#!/bin/bash
# this file is generated by "build/makeImg.sh" -- all changes get overwriten --
cd $APPDIR
$APPDIR/usr/bin/manaplus $@' > $rel/AppDir/AppRun

chmod -R 755 "$dir/$appdir"

$rel/squashfs-root/AppRun --appdir "$rel/$appdir" \
-d $rel/manaplus.desktop \
-i $rel/data/icons/manaplus.svg \
-e $rel/run/bin/manaplus \
--output appimage >> logs/appimg.log

mv *.AppImage ../binarys/
cp -r run/ binarys/

exit 0