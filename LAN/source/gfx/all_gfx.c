//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifdef __cplusplus
extern "C" {
#endif

#include "all_gfx.h"

#include "shield.c"
#include "shield.pal.c"

#include "paused.c"
#include "paused.pal.c"

#include "corneria.c"
#include "finaldestination.c"
#include "pokemonstadium.c"
#include "castlesiege.c"

#include "corneria.pal.c"
#include "pokemonstadium.pal.c"
#include "castlesiege.pal.c"
#include "finaldestination.pal.c"

#include "finaldestinationminimap.c"
#include "finaldestinationminimap.pal.c"

#include "pokemonstadiumminimap.c"
#include "pokemonstadiumminimap.pal.c"

PAGfx_struct finaldestination = {(void*)finaldestination_Map, 8192, (void*)finaldestination_Tiles, 40384, (void*)finaldestination_Pal, (int*)finaldestination_Info };
PAGfx_struct pokemonstadium = {(void*)pokemonstadium_Map, 8192, (void*)pokemonstadium_Tiles, 35072, (void*)pokemonstadium_Pal, (int*)pokemonstadium_Info };
PAGfx_struct castlesiege = {(void*)castlesiege_Map, 8192, (void*)castlesiege_Tiles, 92480, (void*)castlesiege_Pal, (int*)castlesiege_Info };
PAGfx_struct corneria = {(void*)corneria_Map, 8192, (void*)corneria_Tiles, 35328, (void*)corneria_Pal, (int*)corneria_Info };
PAGfx_struct finaldestinationminimap = {(void*)finaldestinationminimap_Map, 768, (void*)finaldestinationminimap_Tiles, 3520, (void*)finaldestinationminimap_Pal, (int*)finaldestinationminimap_Info };
PAGfx_struct pokemonstadiumminimap = {(void*)pokemonstadiumminimap_Map, 768, (void*)pokemonstadiumminimap_Tiles, 3968, (void*)pokemonstadiumminimap_Pal, (int*)pokemonstadiumminimap_Info };
PAGfx_struct paused = {(void*)paused_Map, 768, (void*)paused_Tiles, 23552, (void*)paused_Pal, (int*)paused_Info };

#ifdef __cplusplus
}
#endif

