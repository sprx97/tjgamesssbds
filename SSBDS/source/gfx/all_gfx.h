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

extern const unsigned char menusolo[4096] __attribute__ ((aligned (4))) ;
extern const unsigned char menulan[4096] __attribute__ ((aligned (4))) ;
extern const unsigned char menuextras[4096] __attribute__ ((aligned (4))) ;
extern const unsigned char menuoptions[4096] __attribute__ ((aligned (4))) ;

extern const unsigned short menusolo_Pal[203];
extern const unsigned short menulan_Pal[201];
extern const unsigned short menuoptions_Pal[201];
extern const unsigned short menuextras_Pal[196];

// Minimap Icons :
extern const unsigned char ArrowLeft_Sprite[12288] __attribute__ ((aligned (4))) ;  // Pal : ArrowLeft_Pal
extern const unsigned char ArrowRight_Sprite[12288] __attribute__ ((aligned (4))) ;  // Pal : ArrowRight_Pal
extern const unsigned char ArrowUp_Sprite[12288] __attribute__ ((aligned (4))) ;  // Pal : ArrowUp_Pal
extern const unsigned char ArrowDown_Sprite[12288] __attribute__ ((aligned (4))) ; // Pal : ArrowDown_Pal

extern const unsigned char p1minimap_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : p1minimap_Pal
extern const unsigned char p2minimap_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : p2minimap_Pal
extern const unsigned char p3minimap_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : p2minimap_Pal
extern const unsigned char p4minimap_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : p2minimap_Pal
extern const unsigned char visibleminimap_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : visibleminimap_Pal

extern const unsigned char projectilesprites[122880] __attribute__ ((aligned (4))) ;
extern const unsigned char charprev[40960] __attribute__ ((aligned (4))) ;
extern const unsigned char charsel[40960] __attribute__ ((aligned (4))) ;
extern const unsigned char stagesel[40960] __attribute__ ((aligned (4))) ;
extern const unsigned char specialFX[73728] __attribute__ ((aligned (4))) ;

// Background files : 
extern const int finaldestination_Info[3]; // BgMode, Width, Height
extern const unsigned short finaldestination_Map[8192] __attribute__ ((aligned (4))) ;  // Pal : finaldestination_Pal
extern const unsigned char finaldestination_Tiles[40384] __attribute__ ((aligned (4))) ;  // Pal : finaldestination_Pal
extern PAGfx_struct finaldestination; // background pointer

extern const int finaldestinationminimap_Info[3]; // BgMode, Width, Height
extern const unsigned short finaldestinationminimap_Map[512] __attribute__ ((aligned (4))) ;  // Pal : finaldestinationminimap_Pal
extern const unsigned char finaldestinationminimap_Tiles[960] __attribute__ ((aligned (4))) ;  // Pal : finaldestinationminimap_Pal
extern PAGfx_struct finaldestinationminimap; // background pointer

extern const int pokemonstadium_Info[3]; // BgMode, Width, Height
extern const unsigned short pokemonstadium_Map[8192] __attribute__ ((aligned (4))) ;  // Pal : pokemonstadium_Pal
extern const unsigned char pokemonstadium_Tiles[35072] __attribute__ ((aligned (4))) ;  // Pal : pokemonstadium_Pal
extern PAGfx_struct pokemonstadium; // background pointer

extern const int castleseige_Info[3]; // BgMode, Width, Height
extern const unsigned int castleseige_Map[8192] __attribute__ ((aligned (4))) ;  // Pal : castleseige_Pal
extern const unsigned char castleseige_Tiles[92480] __attribute__ ((aligned (4))) ;  // Pal : castleseige_Pal
extern PAGfx_struct castleseige; // background pointer

extern const int corneria_Info[3]; // BgMode, Width, Height
extern const unsigned short corneria_Map[8192] __attribute__ ((aligned (4))) ;
extern const unsigned char corneria_Tiles[35328] __attribute__ ((aligned (4))) ;
extern PAGfx_struct corneria;

extern const unsigned short projectilesprites_Pal[19] __attribute__ ((aligned (4))) ;
extern const unsigned short charsel_Pal[235] __attribute__ ((aligned (4))) ;
extern const unsigned short stagesel_Pal[245] __attribute__ ((aligned (4))) ;
extern const unsigned short specialFX_Pal[242] __attribute__ ((aligned (4))) ;
extern const unsigned short charprev_Pal[250] __attribute__ ((aligned (4))) ;

extern const unsigned short corneria_Pal[166] __attribute__ ((aligned (4))) ;
extern const unsigned short finaldestination_Pal[237] __attribute__ ((aligned (4))) ;
extern const unsigned short pokemonstadium_Pal[215] __attribute__ ((aligned (4))) ;
extern const unsigned short castleseige_Pal[201] __attribute__ ((aligned (4))) ;
extern const unsigned short ArrowLeft_Pal[84] __attribute__ ((aligned (4))) ;
extern const unsigned short ArrowRight_Pal[84] __attribute__ ((aligned (4))) ;
extern const unsigned short ArrowUp_Pal[50] __attribute__ ((aligned (4))) ;
extern const unsigned short ArrowDown_Pal[50] __attribute__ ((aligned (4))) ;
extern const unsigned short p1minimap_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short p2minimap_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short p3minimap_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short p4minimap_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short visibleminimap_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short finaldestinationminimap_Pal[208] __attribute__ ((aligned (4))) ;
#endif

