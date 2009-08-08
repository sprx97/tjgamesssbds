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

extern const int finaldestination_Info[3]; // BgMode, Width, Height
extern const unsigned short finaldestination_Map[8192] __attribute__ ((aligned (4))) ;  // Pal : finaldestination_Pal
extern const unsigned char finaldestination_Tiles[37248] __attribute__ ((aligned (4))) ;  // Pal : finaldestination_Pal
extern PAGfx_struct finaldestination; // background pointer
extern const unsigned short finaldestination_Pal[192] __attribute__ ((aligned (4))) ;

extern const int pokemonstadium_Info[3]; // BgMode, Width, Height
extern const unsigned short pokemonstadium_Map[8192] __attribute__ ((aligned (4))) ;  // Pal : pokemonstadium_Pal
extern const unsigned char pokemonstadium_Tiles[34496] __attribute__ ((aligned (4))) ;  // Pal : pokemonstadium_Pal
extern PAGfx_struct pokemonstadium; // background pointer
extern const unsigned short pokemonstadium_Pal[215] __attribute__ ((aligned (4))) ;

extern const int castlesiege_Info[3]; // BgMode, Width, Height
extern const unsigned int castlesiege_Map[8192] __attribute__ ((aligned (4))) ;
extern const unsigned char castlesiege_Tiles[92480] __attribute__ ((aligned (4))) ;
extern PAGfx_struct castlesiege; // background pointer
extern const unsigned short castlesiege_Pal[201] __attribute__ ((aligned (4))) ;

extern const int corneria_Info[3]; // BgMode, Width, Height
extern const unsigned short corneria_Map[8192] __attribute__ ((aligned (4))) ;
extern const unsigned char corneria_Tiles[35328] __attribute__ ((aligned (4))) ;
extern PAGfx_struct corneria; // background pointer
extern const unsigned short corneria_Pal[166] __attribute__ ((aligned (4))) ;

#endif

