To use features like mp3 playback or liblobby you have to select an arm7 core for your project.
You can do this in the "Makefile" - open it with an editor and read the first few lines.

There are the following features:
---------------------------------
Sound / Music:
mp3      - mp3 file playback using Noda's AS_Lib (Examples\Sound\ASLib_General) - you can always use AS_Lib for sounds, but mp3s only with this option.
mod      - mod file playback using hitchhikr's modplayer (Examples\Sound\ModPlayback)
mikmod   - module playback (mod, s3m, xm, etc.) using Stonebone's libmikmod (Examples\Sound\MikMod)

Networking:
dsWifi   - dsWifi for DS<->AccessPoint connections (Examples\Wifi\dsWifi)
libLobby - liblobby for DS<->DS connections (Examples\Wifi\libLobby) - if you choose this, you must initialize libLobby at the beginning of your program like in the liblobby example, else the arm7 will be stuck

File structure:
efs      - EFS_Lib to append a file system to the nds (Examples\FAT\EFS_Lib) - if you choose this, copy over efsroot and the patcher from the example
---------------------------------

Important note:
Your project will only work on the G6 and M3 Real cards if you choose an arm7 core which does not use much space.
Currently this is only the arm7_mp3 core and the "basic" core (which is the same as arm7_mp3, left for backward compatibility).

You should also replace the project description with something more appropriate. The default is:
TEXT1 		:= PAlib Project -
TEXT2 		:= Change this text
TEXT3 		:= for your project!

As the last step, consider creating a custom logo.bmp for your project even if your card doesn't show this icon.
Its size has to be 32x32 and it can have up to 16 colors (color index 0 is transparent).
If your card doesn't show the internal logo from NDS ROMs, you can use DSOrganize to view it on the DS and confirm that it looks right.
