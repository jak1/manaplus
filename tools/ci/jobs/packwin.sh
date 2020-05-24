#!/bin/bash

cp_dll(){
	if [[ -f "/usr/lib/mxe/usr/${CROSS}/bin/${1}" ]]; then
		cp "/usr/lib/mxe/usr/${CROSS}/bin/${1}" "../../libs/dll/${1}"
	else
		echo "NOT FOUND: /usr/lib/mxe/usr/${CROSS}/bin/${1}"
		exit 1
	fi
}

cp_dlls(){
	mkdir -p ../../libs/dll
	cp_dll "SDL.dll"
	cp_dll "SDL_image.dll"
	cp_dll "SDL_mixer.dll"
	cp_dll "SDL_net.dll"
	cp_dll "SDL_ttf.dll"
	cp_dll "libSDL_gfx-16.dll"
	cp_dll "libcurl-4.dll"
	#cp_dll "libfluidsynth-2.dll" # not needed!
	cp_dll "libfreetype-6.dll"
	cp_dll "libiconv-2.dll"
	cp_dll "libintl-8.dll"
	cp_dll "libjpeg-9.dll"
	cp_dll "libogg-0.dll"
	cp_dll "libpng16-16.dll"
	cp_dll "libstdc++-6.dll"
	cp_dll "libvorbis-0.dll"
	cp_dll "libvorbisfile-3.dll"
	cp_dll "libxml2-2.dll"
	cp_dll "libwinpthread-1.dll"
	cp_dll "zlib1.dll"
	cp_dll "gdb.exe"
	cp_dll "libdl.dll"
	cp_dll "libexpat-1.dll"
	#cp_dll "libreadline6.dll" # hmm update?
	#cp_dll "libreadline7.dll" # ^
	cp_dll "libreadline8.dll"
	cp_dll "libtermcap.dll"
}

cp_dlls
