#include "stages.h"
#include "stage.h"
const int RANDOM_STAGE = 0, FINALDESTINATION = 1, POKEMONSTADIUM = 2, CASTLESIEGE = 3, CORNERIA = 4, MAX_STAGE = 3;
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
	walls.push_back(Wall(50, 154, 102, "left"));
	walls.push_back(Wall(462, 154, 102, "right"));
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
	leftdeath = -192;
	rightdeath = 704;
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
	walls.push_back(Wall(48, 222, 100, "left"));
	walls.push_back(Wall(442, 222, 100, "right"));
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
	spawnx.push_back(160);
	spawnx.push_back(288);
	spawnx.push_back(96);
	spawnx.push_back(224);
	spawny.push_back(2);
	spawny.push_back(2);
	spawny.push_back(2);
	spawny.push_back(2);
	floors.push_back(Floor(30, 74, 60, 15, false));
	floors[0].rightneighbor = 1;
	floors.push_back(Floor(90, 59, 124, -12, false));
	floors[1].leftneighbor = 0;
	floors[1].rightneighbor = 2;
	floors.push_back(Floor(214, 71, 46, 10, false));
	floors[2].leftneighbor = 1;
	floors[2].rightneighbor = 3;
	floors.push_back(Floor(260, 61, 35, 30, false));
	floors[3].leftneighbor = 2;
	floors[3].rightneighbor = 4;
	floors.push_back(Floor(295, 31, 74, 30, false));
	floors[4].leftneighbor = 3;
	floors.push_back(Floor(362, 85, 46, 0, false));
	floors[5].rightneighbor = 6;
	floors.push_back(Floor(408, 85, 33, 7, false));
	floors[6].leftneighbor = 5;
	floors[6].rightneighbor = 7;
	floors.push_back(Floor(441, 78, 44, -17, false));
	floors[7].leftneighbor = 6;
	songnames.push_back("Main Theme");
	songnames.push_back("Fortuna");
	songartists.push_back("Star Fox");
	songartists.push_back("Star Fox Assault");
	songs.push_back("SSBDS_Files/music/stages/corneria1.mp3");
	songs.push_back("SSBDS_Files/music/stages/corneria2.mp3");
} // initializes Corneria
