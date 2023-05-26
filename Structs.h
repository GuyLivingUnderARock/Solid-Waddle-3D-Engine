#pragma once
#include <vector>
#include <cmath>

#include "SDL.h"


#pragma region Colours
enum Colours {
	WHITE,
	BLACK,
	RED,
	GREEN,
	BLUE
};

struct Colour {
public:
	unsigned int r = 255;
	unsigned int g = 255;
	unsigned int b = 255;
	unsigned int alpha = 255;

	Colour(Colours colour) {
		switch (colour) {
		case Colours::WHITE:
			r = 255;
			g = 255;
			b = 255;
			alpha = 255;
			break;
		case Colours::BLACK:
			r = 0;
			g = 0;
			b = 0;
			alpha = 255;
			break;
		case Colours::RED:
			r = 255;
			g = 0;
			b = 0;
			alpha = 255;
			break;
		case Colours::GREEN:
			r = 0;
			g = 255;
			b = 0;
			alpha = 255;
			break;
		case Colours::BLUE:
			r = 0;
			g = 0;
			b = 255;
			alpha = 255;
			break;
		}
	}
};
#pragma endregion

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
	SDL_FPoint ConvertToSDL_FPoint(float focalDistance, Point3D camPosition, Point3D camRotation);
};

struct ViewCam {
	Point3D position;
	Point3D rotation;

	float fov = 60;
	float move_scale = 10;

	// Returns the distance the virtua camera is from the screen using the fov
	float CalcFocalDist();
	// Additively changes the position of the view port in the world space
	void Move(Point3D moveBy);
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
	Colour colour = Colour(Colours::BLUE);

	std::vector<SDL_FPoint> points;
	std::vector<Triangle2D> triangles;
};

struct Mesh3D {
public:
	Colour colour = Colour(Colours::BLUE);

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
	Colour clearColour = Colour(Colours::WHITE); // Colour that the renderer fills with to clear the screen

	bool fullscreen = false; // Unimplemented
	int frameCount, timerFPS, lastFrame, fps; // Unimplemented

	ViewCam cam; // The view port in the 3D engine

	std::vector<Mesh3D> meshes;

	// Initalizes SDL components i.e. The window and renderer
	void InitSDL();
	// Sets the colour the renderer is drawing or clearing with
	void SetRenderDrawColour(Colour clearColour);
	// Draws all meshes in the 3D engine
	void Draw();
};

#pragma region PrimitiveMeshes
struct Cubeoid : Mesh3D {
public:
	Cubeoid(Engine3D& engine3D, Point3D centre, Point3D size);
};

#pragma endregion