#pragma once
#include <vector>
#include <cmath>

#include "SDL.h"


//The application time based timer
struct LTimer {
public:
	//Initializes variables
	LTimer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();
	void restart();

	//Gets the timer's time
	Uint32 getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The clock time when the timer started
	Uint32 mStartTicks;

	//The ticks stored when the timer was paused
	Uint32 mPausedTicks;

	//The timer status
	bool mPaused;
	bool mStarted;
};

enum Colours {
	WHITE,
	BLACK,
	RED,
	GREEN,
	BLUE
};

SDL_Colour Colour(Colours colour);

struct Point3D {
public:
	float x = 0;
	float y = 0;
	float z = 0;

	Point3D& operator +(float a);
	Point3D& operator +=(float a);

	Point3D& operator +(Point3D& point);
	Point3D& operator +=(Point3D& point);

	Point3D& operator -(float a);
	Point3D& operator -=(float a);

	Point3D& operator -(Point3D& point);
	Point3D& operator -=(Point3D& point);

	Point3D& operator *(float a);
	Point3D& operator *=(float a);

	Point3D& operator *(Point3D& point);
	Point3D& operator *=(Point3D& point);

	Point3D& operator /(float a);
	Point3D& operator /=(float a);

	Point3D& operator /(Point3D& point);
	Point3D& operator /=(Point3D& point);

	// Returns a 2D point conversion of this 3D point
	SDL_FPoint ConvertToPoint2D(float focalDistance, Point3D camPosition, Point3D camRotation);
	// Returns a 3D point conversion of this 2D point
	static Point3D ConvertToPoint3D(SDL_FPoint point2D, float z, float focalDistance, Point3D camPosition, Point3D camRotation);
	float Distance(Point3D point);
};

struct ViewCam {
	Point3D position;
	Point3D rotation;

	float fov = 90;
	float move_scale = 500;

	// Returns the distance the virtua camera is from the screen using the fov
	float CalcFocalDist();
	// Additively changes the position of the view port in the world space
	void MoveBy(Point3D moveBy);
};

#pragma region Triangles
struct Triangle2D
{
public:
	// For some DUMB FUCKING REASON I don't know why,
	// but I can't use a vector of SDL_FPoint references, reference_wrappers, or even pointers
	// like, WTF why does it explode the Process memory from 3 or 4 MB to 16 or 17 GB!!!!
	// YTAF??
	std::vector<SDL_FPoint> points;

	// Returns the 2D points of the triangle as SDL vertexes
	std::vector<SDL_Vertex> ConvertToSDL_Vertex();
};

struct Triangle3D {
public:
	// Same dumb shit here.
	std::vector<Point3D> points;
};
#pragma endregion

#pragma region Meshes
struct Mesh2D
{
public:
	SDL_Colour colour = Colour(Colours::BLUE);

	std::vector<SDL_FPoint> points;
	std::vector<Triangle2D> triangles;
};

struct Mesh3D {
public:
	SDL_Colour colour = Colour(Colours::BLUE);

	std::vector<Point3D> points;
	std::vector<Triangle3D> triangles;

	// Reterns a 2D projection of the 3D mesh
	Mesh2D ConvertTo2DMesh(ViewCam cam);
	// Returns the average x, y, z of all points in the mesh as a Point3D
	Point3D AverageCentre();
	// Additively changes the position of all points in the mesh
	void Move(Point3D moveBy);
};
#pragma endregion

struct Engine3D {
public:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr; // Used to render to the SDL_Window using the GPU

	Uint32 rendererFlags = 0;
	Uint32 windowFlags = 0;

	const char* windowName = "Window 01";
	SDL_Colour clearColour = Colour(Colours::BLACK); // Colour that the renderer fills with to clear the screen

	bool fullscreen = false; // Unimplemented

	float frameStep = 1.5f;

	int frameCount; // Frames loaded since last frame step
	LTimer fpsTimer; // Time since start
	float fps; // 

	LTimer deltaTimer;
	float deltaTime; // Time taken for the last frame

	ViewCam cam; // The view port in the 3D engine

	std::vector<Mesh3D> meshes;

	// Initalizes SDL components i.e. The window and renderer
	void InitSDL();
	// Sets the colour the renderer is drawing or clearing with
	void SetRenderDrawColour(SDL_Colour clearColour);
	// Draws all meshes in the 3D engine
	void Draw();

	void FrameCalcs();
};

#pragma region PrimitiveMeshes
struct Cubeoid : Mesh3D {
public:
	Cubeoid(Engine3D& engine3D, Point3D centre_ = Point3D{ 0, 0, 0 }, Point3D size = Point3D{ 1, 1, 1 },
		SDL_Colour colour_ = Colour(Colours::BLUE));
};

#pragma endregion