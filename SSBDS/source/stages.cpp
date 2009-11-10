#include "stages.h"
#include "stage.h"
const int RANDOM_STAGE = 0, FINALDESTINATION = 1, POKEMONSTADIUM = 2, CASTLESIEGE = 3, CORNERIA = 4, MAX_STAGE = 5;
// stage shortcuts just like character shortcuts

FinalDestination::~FinalDestination() {}
FinalDestination::FinalDestination() {
	Initialize();
}
void FinalDestination::Initialize() {
	name = "finaldestination";
	width = 512;
	height = 256;
	topdeath = -320;
	leftdeath = -192;
	rightdeath = 704;
	bottomdeath = 256;
	minhorizscroll = -128;
	maxhorizscroll = 384;
	minvertscroll = -192;
	maxvertscroll = 64;
	spawnx.push_back(160);
	spawnx.push_back(288);
	spawnx.push_back(96);
	spawnx.push_back(352);
	spawny.push_back(4);
	spawny.push_back(4);
	spawny.push_back(4);
	spawny.push_back(4);
	floors.push_back(Floor(64, 132, 384, 0, false));
	walls.push_back(Wall(96, 132, 116, "left"));
	walls.push_back(Wall(416, 132, 116, "right"));
	walls.push_back(Wall(65, 132, 1, "left"));
	walls.push_back(Wall(447, 132, 1, "right"));
	ceilings.push_back(Ceiling(64, 132, 384));
	ledges.push_back(Ledge(64, 132, "left"));
	ledges.push_back(Ledge(448, 132, "right"));
	songnames.push_back("Final Destination");
	songnames.push_back("Final Destination");
	songartists.push_back("Super Smash Bros. Brawl");
	songartists.push_back("Super Smash Bros. Melee");
	songs.push_back("SSBDS_Files/music/stages/finaldestination1.mp3");
	songs.push_back("SSBDS_Files/music/stages/finaldestination2.mp3");
} // initializes all the variables for final destination
PokemonStadium::~PokemonStadium() {}
PokemonStadium::PokemonStadium() {
	Initialize();
}
void PokemonStadium::Initialize() {
	name = "pokemonstadium";
	width = 512;
	height = 256;
	topdeath = -320;
	bottomdeath = 256;
	leftdeath = -192;
	rightdeath = 704;
	minhorizscroll = -128;
	maxhorizscroll = 384;
	minvertscroll = -192;
	maxvertscroll = 64;
	spawnx.push_back(160);
	spawnx.push_back(288);
	spawnx.push_back(96);
	spawnx.push_back(352);
	spawny.push_back(26);
	spawny.push_back(26);
	spawny.push_back(26);
	spawny.push_back(26);
	floors.push_back(Floor(42, 154, 428, 0, false));
	floors.push_back(Floor(128, 110, 62, 0, true));
	floors.push_back(Floor(324, 110, 62, 0, true));
	walls.push_back(Wall(43, 154, 102, "left"));
	walls.push_back(Wall(469, 154, 102, "right"));
	ceilings.push_back(Ceiling(42, 216, 428));
	ledges.push_back(Ledge(42, 154, "left"));
	ledges.push_back(Ledge(470, 154, "right"));
	songnames.push_back("Pokemon Gym / Evolution");
	songnames.push_back("Battle Theme");
	songnames.push_back("Lugia's Song");
	songartists.push_back("Pokemon Red/Blue/Yellow");
	songartists.push_back("Pokemon Gold/Silver/Crystal");
	songartists.push_back("Pokemon 2000");
	songs.push_back("SSBDS_Files/music/stages/pokemonstadium1.mp3");
	songs.push_back("SSBDS_Files/music/stages/pokemonstadium2.mp3");
	songs.push_back("SSBDS_Files/music/stages/pokemonstadium3.mp3");
} // initializes the pokemon stadium stage
CastleSiege::~CastleSiege() {}
CastleSiege::CastleSiege() {
	Initialize();
}
void CastleSiege::Initialize() {
	name = "castlesiege";
	width = 512;
	height = 384;
	topdeath = -320;
	bottomdeath = 384;
	leftdeath = -128;
	rightdeath = 576;
	minhorizscroll = -64;
	maxhorizscroll = 256;
	minvertscroll = -192;
	maxvertscroll = 64;
	spawnx.push_back(160);
	spawnx.push_back(288);
	spawnx.push_back(96);
	spawnx.push_back(352);
	spawny.push_back(88);
	spawny.push_back(88);
	spawny.push_back(88);
	spawny.push_back(88);
	floors.push_back(Floor(40, 216, 161, 0, false));
	floors.push_back(Floor(202, 216, 98, 25, false));
	floors[0].rightneighbor = 1;
	floors[1].leftneighbor = 0;
	floors[1].rightneighbor = 2;
	floors.push_back(Floor(300, 191, 150, 0, false));
	floors[2].leftneighbor = 1;
	floors.push_back(Floor(84, 160, 110, 0, true));
	floors.push_back(Floor(302, 142, 104, 0, true));
	ledges.push_back(Ledge(40, 216, "left"));
	ledges.push_back(Ledge(450, 191, "right"));
	walls.push_back(Wall(41, 216, 40, "left"));
	walls.push_back(Wall(449, 191, 40, "right"));
	ceilings.push_back(Ceiling(40, 256, 100));
	ceilings.push_back(Ceiling(349, 231, 100));
	walls.push_back(Wall(55, 256, 200, "left"));
	walls.push_back(Wall(435, 231, 200, "right"));
	songnames.push_back("Ike's Theme");
	songnames.push_back("Victory is Near");
	songartists.push_back("Fire Emblem 10: Radiant Dawn");
	songartists.push_back("Fire Emblem 9: Path of Radiance");
	songs.push_back("SSBDS_Files/music/stages/castlesiege1.mp3");
	songs.push_back("SSBDS_Files/music/stages/castlesiege2.mp3");
} // initializes Castle Siege
Corneria::~Corneria() {}
Corneria::Corneria() {
	Initialize();
}
void Corneria::Initialize() {
	name = "corneria";
	width = 512;
	height = 256;
	topdeath = -320;
	bottomdeath = 256;
	leftdeath = -192;
	rightdeath = 704;
	minhorizscroll = -128;
	maxhorizscroll = 384;
	minvertscroll = -192;
	maxvertscroll = 64;
	spawnx.push_back(160);
	spawnx.push_back(288);
	spawnx.push_back(96);
	spawnx.push_back(224);
	spawny.push_back(-128);
	spawny.push_back(-128);
	spawny.push_back(-128);
	spawny.push_back(-128);
	floors.push_back(Floor(-14, 29, 64, 16, false));
	floors[0].rightneighbor = 1;
	floors.push_back(Floor(50, 13, 146, -12, false));
	floors[1].leftneighbor = 0;
	floors[1].rightneighbor = 2;
	floors.push_back(Floor(196, 25, 66, 10, false));
	floors[2].leftneighbor = 1;
	floors[2].rightneighbor = 3;
	floors.push_back(Floor(262, 15, 37, 31, false));
	floors[3].leftneighbor = 2;
	floors[3].rightneighbor = 4;
	floors.push_back(Floor(299, -16, 91, 31, false));
	floors[4].leftneighbor = 3;
	floors.push_back(Floor(384, 46, 55, 0, false));
	floors[5].rightneighbor = 6;
	floors.push_back(Floor(439, 46, 38, 8, false));
	floors[6].leftneighbor = 5;
	floors[6].rightneighbor = 7;
	floors.push_back(Floor(477, 38, 54, -23, false));
	floors[7].leftneighbor = 6;
	floors.push_back(Floor(78, 104, 35, 0, false));
	ledges.push_back(Ledge(-14, 29, "left"));
	ledges.push_back(Ledge(390, -47, "right"));
	ledges.push_back(Ledge(531, 61, "right"));
	songnames.push_back("Main Theme");
	songnames.push_back("Fortuna");
	songartists.push_back("Star Fox");
	songartists.push_back("Star Fox Assault");
	songs.push_back("SSBDS_Files/music/stages/corneria1.mp3");
	songs.push_back("SSBDS_Files/music/stages/corneria2.mp3");
} // initializes Corneria
