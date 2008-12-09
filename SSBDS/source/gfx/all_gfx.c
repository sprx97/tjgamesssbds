//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifdef __cplusplus
extern "C" {
#endif

#include "all_gfx.h"

#include "charsel.c"
#include "charsel.pal.c"
#include "stagesel.c"
#include "stagesel.pal.c"

#include "menusolo.c"
#include "menulan.c"
#include "menuoptions.c"
#include "menuextras.c"
#include "menusolo.pal.c"
#include "menulan.pal.c"
#include "menuoptions.pal.c"
#include "menuextras.pal.c"

#include "Other/SpecialFX.c"
	
#include "Palettes/specialFX.pal.c"
	
#include "charprev.c"
#include "charprev.pal.c"

#include "projectilesprites.c"
#include "projectilesprites.pal.c"

#include "Other/ArrowLeft.c"
#include "Other/ArrowRight.c"
#include "Other/ArrowUp.c"
#include "Other/ArrowDown.c"

#include "Minimap/p1minimap.c"
#include "Minimap/p2minimap.c"
#include "Minimap/p3minimap.c"
#include "Minimap/p4minimap.c"
#include "Minimap/visibleminimap.c"

#include "Stages/corneria.c"
#include "Stages/finaldestination.c"
#include "Stages/pokemonstadium.c"
#include "Stages/castleseige.c"
#include "Minimap/finaldestinationminimap.c"

#include "Palettes/corneria.pal.c"
#include "Palettes/pokemonstadium.pal.c"
#include "Palettes/castleseige.pal.c"
#include "Palettes/finaldestination.pal.c"
#include "Palettes/ArrowLeft.pal.c"
#include "Palettes/ArrowRight.pal.c"
#include "Palettes/ArrowUp.pal.c"
#include "Palettes/ArrowDown.pal.c"
#include "Palettes/p1minimap.pal.c"
#include "Palettes/p2minimap.pal.c"
#include "Palettes/p3minimap.pal.c"
#include "Palettes/p4minimap.pal.c"
#include "Palettes/visibleminimap.pal.c"
#include "Palettes/finaldestinationminimap.pal.c"

PAGfx_struct finaldestination = {(void*)finaldestination_Map, 8192, (void*)finaldestination_Tiles, 40384, (void*)finaldestination_Pal, (int*)finaldestination_Info };
PAGfx_struct finaldestinationminimap = {(void*)finaldestinationminimap_Map, 512, (void*)finaldestinationminimap_Tiles, 960, (void*)finaldestinationminimap_Pal, (int*)finaldestinationminimap_Info };
PAGfx_struct pokemonstadium = {(void*)pokemonstadium_Map, 8192, (void*)pokemonstadium_Tiles, 35072, (void*)pokemonstadium_Pal, (int*)pokemonstadium_Info };
PAGfx_struct castleseige = {(void*)castleseige_Map, 8192, (void*)castleseige_Tiles, 92480, (void*)castleseige_Pal, (int*)castleseige_Info };
PAGfx_struct corneria = {(void*)corneria_Map, 8192, (void*)corneria_Tiles, 35328, (void*)corneria_Pal, (int*)corneria_Info };

#ifdef __cplusplus
}
#endif

