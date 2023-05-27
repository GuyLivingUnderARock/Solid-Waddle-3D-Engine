#include <iostream>

#include "Defs.h"
#include "Structs.h"


#pragma region LTimer
LTimer::LTimer() {
	//Initialize the variables
	mStartTicks = 0;
	mPausedTicks = 0;

	mPaused = false;
	mStarted = false;
}

void LTimer::start() {
	//Start the timer
	mStarted = true;

	//Unpause the timer
	mPaused = false;

	//Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop() {
	//Stop the timer
	mStarted = false;

	//Unpause the timer
	mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause() {
	//If the timer is running and isn't already paused
	if (mStarted && !mPaused)
	{
		//Pause the timer
		mPaused = true;

		//Calculate the paused ticks
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause()
{
	//If the timer is running and paused
	if (mStarted && mPaused)
	{
		//Unpause the timer
		mPaused = false;

		//Reset the starting ticks
		mStartTicks = SDL_GetTicks() - mPausedTicks;

		//Reset the paused ticks
		mPausedTicks = 0;
	}
}

void LTimer::restart() {
	stop();
	start();
}

Uint32 LTimer::getTicks() {
	//The actual timer time
	Uint32 time = 0;

	//If the timer is running
	if (mStarted) {
		//If the timer is paused
		if (mPaused) {
			//Return the number of ticks when the timer was paused
			time = mPausedTicks;
		}
		else {
			//Return the current time minus the start time
			time = SDL_GetTicks() - mStartTicks;
		}
	}

	return time;
}

bool LTimer::isStarted() {
	//Timer is running and paused or unpaused
	return mStarted;
}

bool LTimer::isPaused() {
	//Timer is running and paused
	return mPaused && mStarted;
}
#pragma endregion

SDL_Colour Colour(Colours colour) {
	SDL_Colour sdl_colour = SDL_Colour();

	switch (colour) {
	case Colours::WHITE:
		sdl_colour.r = 255;
		sdl_colour.g = 255;
		sdl_colour.b = 255;
		sdl_colour.a = 255;
		break;
	case Colours::BLACK:
		sdl_colour.r = 0;
		sdl_colour.g = 0;
		sdl_colour.b = 0;
		sdl_colour.a = 255;
		break;
	case Colours::RED:
		sdl_colour.r = 255;
		sdl_colour.g = 0;
		sdl_colour.b = 0;
		sdl_colour.a = 255;
		break;
	case Colours::GREEN:
		sdl_colour.r = 0;
		sdl_colour.g = 255;
		sdl_colour.b = 0;
		sdl_colour.a = 255;
		break;
	case Colours::BLUE:
		sdl_colour.r = 0;
		sdl_colour.g = 0;
		sdl_colour.b = 255;
		sdl_colour.a = 255;
		break;
	}

	return sdl_colour;
}

#pragma region Point3D Operator Overides
Point3D& Point3D::operator +(float a) {
	x += a;
	y += a;
	z += a;

	return *this;
}
Point3D& Point3D::operator +=(float a) {
	return *this + a;
}

Point3D& Point3D::operator +(Point3D& point) {
	x += point.x;
	y += point.y;
	z += point.z;

	return *this;
}
Point3D& Point3D::operator +=(Point3D& point) {
	return *this + point;
}

Point3D& Point3D::operator -(float a) {
	x -= a;
	y -= a;
	z -= a;

	return *this;
}
Point3D& Point3D::operator -=(float a) {
	return *this - a;
}

Point3D& Point3D::operator -(Point3D& point) {
	x -= point.x;
	y -= point.y;
	z -= point.z;

	return *this;
}
Point3D& Point3D::operator -=(Point3D& point) {
	return *this - point;
}

Point3D& Point3D::operator *(float a) {
	x *= a;
	y *= a;
	z *= a;

	return *this;
}
Point3D& Point3D::operator *=(float a) {
	return *this * a;
}

Point3D& Point3D::operator *(Point3D& point) {
	x *= point.x;
	y *= point.y;
	z *= point.z;

	return *this;
}
Point3D& Point3D::operator *=(Point3D& point) {
	return *this * point;
}

Point3D& Point3D::operator /(float a) {
	x /= a;
	y /= a;
	z /= a;

	return *this;
}
Point3D& Point3D::operator /=(float a) {
	return *this / a;
}

Point3D& Point3D::operator /(Point3D& point) {
	x /= point.x;
	y /= point.y;
	z /= point.z;

	return *this;
}
Point3D& Point3D::operator /=(Point3D& point) {
	return *this / point;
}
#pragma endregion

SDL_FPoint Point3D::ConvertToSDL_FPoint(float focalDistance, Point3D camPosition, Point3D camRotation) {
	SDL_FPoint point;

	float h_x = std::sqrt(std::pow(z + camPosition.z + focalDistance, 2) + std::pow(x - camPosition.x, 2));
	float h_y = std::sqrt(std::pow(z + camPosition.z + focalDistance, 2) + std::pow(y + camPosition.y, 2));

	float angle_x = std::asin((x - camPosition.x) / h_x);
	float angle_y = std::asin((y + camPosition.y) / h_y);

	point.x = h_x * std::sin(angle_x) * (z + camPosition.z + focalDistance) / focalDistance + WINDOW_WIDTH/2;
	point.y = h_y * std::sin(angle_y) * (z + camPosition.z + focalDistance) / focalDistance + WINDOW_HEIGHT/2;

	return point;
}

std::vector<SDL_Vertex> Triangle2D::ConvertToSDL_Vertex() {
	std::vector<SDL_Vertex> vertexes;

	for (auto& point: points) {
		vertexes.emplace_back();
		vertexes[vertexes.size() - 1].position = point;
	}

	return vertexes;
}

#pragma region ViewCam
float ViewCam::CalcFocalDist() {
	return std::sqrt(std::pow((WINDOW_WIDTH / 2) / (std::sin( (fov*PI/180) / 2)), 2) + std::pow(WINDOW_WIDTH / 2, 2));
}

void ViewCam::MoveBy(Point3D moveBy) {
	position += moveBy * move_scale;
}
#pragma endregion

#pragma region Mesh2D
Mesh2D Mesh3D::ConvertTo2DMesh(ViewCam cam) {
	Mesh2D mesh;
	mesh.colour = colour;

	for (int t = 0; t < triangles.size(); t++) {
		mesh.triangles.emplace_back();

		for (int p = 0; p < triangles[t].points.size(); p++) {
			mesh.points.emplace_back(triangles[t].points[p].ConvertToSDL_FPoint(cam.CalcFocalDist(), cam.position, cam.rotation));
			mesh.triangles[t].points.emplace_back(mesh.points[mesh.points.size() - 1]);
		}
	}

	return mesh;
}

Point3D Mesh3D::AverageCentre() {
	Point3D avgPoint;
	int numPoints = 0;

	for (auto& triangle: triangles) {
		for (auto& point : triangle.points) {
			avgPoint += point;
			numPoints++;
		}
	}

	return avgPoint / numPoints;
}

void Mesh3D::Move(Point3D moveBy) {
	for (auto& triangle: triangles) {
		for (auto& point : triangle.points) {
			point += moveBy;
		}
	}
}
#pragma endregion

#pragma region Engine3D
void Engine3D::InitSDL() {
	windowFlags = SDL_WINDOW_SHOWN;
	rendererFlags = SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, windowFlags);
	if (!window) {
		printf("Failed to open %d x %d window: %s\n", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	renderer = SDL_CreateRenderer(window, -1, rendererFlags);
	if (!renderer) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_RenderSetVSync(renderer, 1);
}

void Engine3D::SetRenderDrawColour(SDL_Colour colour) {
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
}

void Engine3D::Draw() {
	SetRenderDrawColour(clearColour);
	SDL_RenderClear(renderer);

	for (auto& mesh : meshes) {
		Mesh2D mesh2D = mesh.ConvertTo2DMesh(cam);

		SetRenderDrawColour(mesh.colour);
		
		for (auto& triangle : mesh2D.triangles) {
			#pragma region Draw Wireframes
			SDL_FPoint point_arr[4];
			int arr_size = std::end(point_arr) - std::begin(point_arr);

			for (int i = 0; i < arr_size; i++) {
				if (i == 3) {
					point_arr[i] = triangle.points[0];
				}
				else {
					point_arr[i] = triangle.points[i];
				}
			}

			SDL_RenderDrawLinesF(renderer, point_arr, arr_size);
			#pragma endregion

			//std::vector<SDL_Vertex> verts = triangle.ConvertToSDL_Vertex();
			//SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
		}
	}

	SDL_RenderPresent(renderer);
}
#pragma endregion

#pragma region Primatives
Cubeoid::Cubeoid(Engine3D& engine3D, Point3D centre_, Point3D size, SDL_Colour colour_) {
	colour = colour_;
	Point3D centre { -centre_.x, -centre_.y, centre_.z };

	Point3D point_A { -0.5, -0.5, -0.5 }; // Point A on a Cube
	point_A *= size;
	point_A += centre;
	points.emplace_back(point_A);

	Point3D point_B { 0.5, -0.5, -0.5 }; // Point B on a Cube
	point_B *= size;
	point_B += centre;
	points.emplace_back(point_B);

	Point3D point_C { 0.5, -0.5, 0.5 }; // Point C on a Cube
	point_C *= size;
	point_C += centre;
	points.emplace_back(point_C);

	Point3D point_D { -0.5, -0.5, 0.5 }; // Point D on a Cube
	point_D *= size;
	point_D += centre;
	points.emplace_back(point_D);

	Point3D point_E { -0.5, 0.5, -0.5 }; // Point E on a Cube
	point_E *= size;
	point_E += centre;
	points.emplace_back(point_E);

	Point3D point_F { 0.5, 0.5, -0.5 }; // Point F on a Cube
	point_F *= size;
	point_F += centre;
	points.emplace_back(point_F);

	Point3D point_G { 0.5, 0.5, 0.5 }; // Point G on a Cube
	point_G *= size;
	point_G += centre;
	points.emplace_back(point_G);

	Point3D point_H { -0.5, 0.5, 0.5 }; // Point H on a Cube
	point_H *= size;
	point_H += centre;
	points.emplace_back(point_H);

	std::vector<Point3D> triangle_points_vector;
	
	triangle_points_vector = { points[0], points[1], points[5] };
	Triangle3D triangle_front1{ triangle_points_vector };
	triangles.emplace_back(triangle_front1);

	triangle_points_vector = { points[0], points[4], points[5] };
	Triangle3D triangle_front2{ triangle_points_vector };
	triangles.emplace_back(triangle_front2);

	triangle_points_vector = { points[1], points[2], points[3] };
	Triangle3D triangle_bottom1{ triangle_points_vector };
	triangles.emplace_back(triangle_bottom1);

	triangle_points_vector = { points[1], points[0], points[3] };
	Triangle3D triangle_bottom2{ triangle_points_vector };
	triangles.emplace_back(triangle_bottom2);

	triangle_points_vector = { points[4], points[5], points[6] };
	Triangle3D triangle_top1{ triangle_points_vector };
	triangles.emplace_back(triangle_top1);

	triangle_points_vector = { points[4], points[7], points[6] };
	Triangle3D triangle_top2{ triangle_points_vector };
	triangles.emplace_back(triangle_top2);

	triangle_points_vector = { points[4], points[0], points[3] };
	Triangle3D triangle_left1{ triangle_points_vector };
	triangles.emplace_back(triangle_left1);

	triangle_points_vector = { points[3], points[7], points[3] };
	Triangle3D triangle_left2{ triangle_points_vector };
	triangles.emplace_back(triangle_left2);

	triangle_points_vector = { points[1], points[2], points[6] };
	Triangle3D triangle_right1{ triangle_points_vector };
	triangles.emplace_back(triangle_right1);

	triangle_points_vector = { points[1], points[5], points[6] };
	Triangle3D triangle_right2{ triangle_points_vector };
	triangles.emplace_back(triangle_right2);

	triangle_points_vector = { points[7], points[6], points[2] };
	Triangle3D triangle_back1{ triangle_points_vector };
	triangles.emplace_back(triangle_back1);

	triangle_points_vector = { points[7], points[3], points[2] };
	Triangle3D triangle_back2{ triangle_points_vector };
	triangles.emplace_back(triangle_back2);
}
#pragma endregion
