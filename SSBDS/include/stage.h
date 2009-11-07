#ifndef STAGE_H
#define STAGE_H
#include <vector>
#include <string>
using std::vector;
using std::string;

/// a ledge on the stage
class Ledge {
	public:
		int x, y; // x and y position of the ledge
		string direction;
		Ledge(int xpos, int ypos, string dir);
};
/// a floor on the stage
class Floor {
	public:
		int x, y; // x and y of far left of stage
		int length; ///< total run of the floor
		int rise; ///<how far up the stage slants
		bool isplatform; ///< is this a platform or not?
		Floor(int xpos, int ypos, int l, int r, bool plat);
		bool isPlatform(); ///< returns isplatform
		Floor* rightneighbor;
		Floor* leftneighbor;
};
/// a ceiling on the stage
class Ceiling {
	public:
		int x, y; // x and y position of ceiling
		int length; ///< length of ceiling
		Ceiling(int xpos, int ypos, int l); ///< creates a new ceiling
};
/// a wall on the stage
class Wall {
	public:
		int x, y; // x and y position of the wall
		int length; ///< how long the wall is
		string direction; ///< what direction it is facing
		Wall(int xpos, int ypos, int l, string d); ///< creates a new wall
};

/// A collection of Floors, Walls, Ceilings, and Ledges along with some miscelaneous data make up a Stage

/// All stages extend this class
class Stage {
	public:
		// variables
		int width, height; // width and height of the image of the stage
		int leftdeath, rightdeath, topdeath, bottomdeath;
		// where a person dies of the edges of the stage
		string name; ///< name of the stage
		vector<Floor> floors; ///< floors of the stage
		vector<Wall> walls; ///< walls of the stage
		vector<Ceiling> ceilings; ///< ceilings of the stage
		vector<Ledge> ledges; ///< ledges of the stage
		vector<int> spawnx; ///< where the x coords of the spawn points are
		vector<int> spawny; ///< where the y coords of the spawn points are
		vector<const char*> songnames;
		vector<const char*> songartists;
		vector<const char*> songs;
		// methods
		Stage();
		virtual void Initialize(); ///< each subclass has it's own initialization code
		string getName();
		vector<Floor> getFloors();
		vector<Wall> getWalls();
		vector<Ceiling> getCeilings();
		vector<Ledge> getLedges();
		// gets stuff about the stage
		//destructor
		virtual ~Stage();
};
#endif
