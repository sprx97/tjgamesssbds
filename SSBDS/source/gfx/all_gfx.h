//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H

#ifndef PAGfx_struct
    typedef struct{
    void *Map;
    int MapSize;
    void *Tiles;
    int TileSize;
    void *Palette;
    int *Info;
} PAGfx_struct;
#endif

extern const unsigned short shield_Pal[90] __attribute__ ((aligned (4))) ;
extern const unsigned char shield_Sprite[4096] __attribute__ ((aligned (4))) ;

extern const unsigned char charprev[40960] __attribute__ ((aligned (4))) ;

extern const int finaldestination_Info[3]; // BgMode, Width, Height
extern const unsigned short finaldestination_Map[8192] __attribute__ ((aligned (4))) ;
extern const unsigned char finaldestination_Tiles[40384] __attribute__ ((aligned (4))) ;
extern PAGfx_struct finaldestination; // background pointer

extern const int pokemonstadium_Info[3]; // BgMode, Width, Height
extern const unsigned short pokemonstadium_Map[8192] __attribute__ ((aligned (4))) ;
extern const unsigned char pokemonstadium_Tiles[35072] __attribute__ ((aligned (4))) ;
extern PAGfx_struct pokemonstadium; // background pointer

extern const int castleseige_Info[3]; // BgMode, Width, Height
extern const unsigned int castleseige_Map[8192] __attribute__ ((aligned (4))) ;
extern const unsigned char castleseige_Tiles[92480] __attribute__ ((aligned (4))) ;
extern PAGfx_struct castleseige; // background pointer

extern const int corneria_Info[3]; // BgMode, Width, Height
extern const unsigned short corneria_Map[8192] __attribute__ ((aligned (4))) ;
extern const unsigned char corneria_Tiles[35328] __attribute__ ((aligned (4))) ;
extern PAGfx_struct corneria; // background pointer

extern const int finaldestinationminimap_Info[3]; // BgMode, Width, Height
extern const unsigned short finaldestinationminimap_Map[768] __attribute__ ((aligned (4))) ;  // Pal : finaldestinationminimap_Pal
extern const unsigned char finaldestinationminimap_Tiles[3520] __attribute__ ((aligned (4))) ;  // Pal : finaldestinationminimap_Pal
extern PAGfx_struct finaldestinationminimap; // background pointer

extern const int pokemonstadiumminimap_Info[3]; // BgMode, Width, Height
extern const unsigned short pokemonstadiumminimap_Map[768] __attribute__ ((aligned (4))) ;  // Pal : pokemonstadiumminimap_Pal
extern const unsigned char pokemonstadiumminimap_Tiles[4096] __attribute__ ((aligned (4))) ;  // Pal : pokemonstadiumminimap_Pal
extern PAGfx_struct pokemonstadiumminimap; // background pointer

extern const unsigned short charprev_Pal[250] __attribute__ ((aligned (4))) ;

extern const unsigned short corneria_Pal[166] __attribute__ ((aligned (4))) ;
extern const unsigned short finaldestination_Pal[237] __attribute__ ((aligned (4))) ;
extern const unsigned short pokemonstadium_Pal[215] __attribute__ ((aligned (4))) ;
extern const unsigned short castleseige_Pal[201] __attribute__ ((aligned (4))) ;

extern const unsigned short finaldestinationminimap_Pal[226] __attribute__ ((aligned (4))) ;
extern const unsigned short pokemonstadiumminimap_Pal[233] __attribute__ ((aligned (4))) ;
#endif

