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


// Sprite files : 
extern const unsigned char damageicons1_Sprite[40960] __attribute__ ((aligned (4))) ;  // Pal : damageicons1_Pal
extern const unsigned char damageicons2_Sprite[40960] __attribute__ ((aligned (4))) ;  // Pal : damageicons2_Pal
extern const unsigned char damageicons3_Sprite[40960] __attribute__ ((aligned (4))) ;  // Pal : damageicons3_Pal
extern const unsigned char damageicons4_Sprite[40960] __attribute__ ((aligned (4))) ;  // Pal : damageicons4_Pal
extern const unsigned char damageiconsC_Sprite[40960] __attribute__ ((aligned (4))) ;  // Pal : damageiconsC_Pal

// Palette files : 
extern const unsigned short damageicons1_Pal[251] __attribute__ ((aligned (4))) ;
extern const unsigned short damageicons2_Pal[251] __attribute__ ((aligned (4))) ;
extern const unsigned short damageicons3_Pal[251] __attribute__ ((aligned (4))) ;
extern const unsigned short damageicons4_Pal[253] __attribute__ ((aligned (4))) ;
extern const unsigned short damageiconsC_Pal[248] __attribute__ ((aligned (4))) ;


#endif

