//-----------------------------------------------------------------------------
#define OLC_PGE3_APPLICATION
#include <olcPixelGameEngine3.h>

//-- BEWARE: olcPGEX3_Miniaudio.h is not a full interface, as it uses
//   types declared in miniaudio.h, so the code using the olc extension
//   still needs to include miniaudio.h to compile. The #define below basically
//   includes the miniaudio.h file, so it¡s ok here. Howerver, when using this
//   static library on our game, we still need to include both the extension and
//   the minmiaudio header (which not only includes the declarations but the
//   function bodies as wel... duplicated code??)
#define OLC_PGEX3_MINIAUDIO
#include <olcPGEX3_Miniaudio.h>
//-----------------------------------------------------------------------------
