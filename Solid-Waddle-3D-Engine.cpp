#include <iostream>
#include <vector>
#include <thread>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_render.h"

#include "Defs.h"
#include "Structs.h"


bool running; // Determines if the main while loop should continue
SDL_Event event;

// Handles SDL events such as keyboard and mouse input
void EventHandle(Engine3D& engine3D) {
	//SDL_WaitEvent(&event); // Pauses program and waits
	SDL_PollEvent(&event); // Uses much more CPU

	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			// Rember to convert float(event.motion.x), float(event.motion.y) to a 3D point properly
			Point3D place_pos = Point3D{ float(event.motion.x), float(event.motion.y), 1 }; // Placement position @ (X, Y, Z)
			Point3D size = Point3D{ 1, 1, 1 } * 100;

			place_pos = Point3D{ 0, 0, 1 } * size;
			engine3D.meshes.emplace_back(Cubeoid(engine3D, place_pos, size, Colour(BLACK)));

			place_pos = Point3D{ -2, 0, 1 } * size;
			engine3D.meshes.emplace_back(Cubeoid(engine3D, place_pos, size, Colour(RED)));

			place_pos = Point3D{ 2, 0, 1 } *size;
			engine3D.meshes.emplace_back(Cubeoid(engine3D, place_pos, size, Colour(RED)));

			place_pos = Point3D{ 0, 2, 1 } * size;
			engine3D.meshes.emplace_back(Cubeoid(engine3D, place_pos, size, Colour(GREEN)));

			place_pos = Point3D{ 0, -2, 1 } * size;
			engine3D.meshes.emplace_back(Cubeoid(engine3D, place_pos, size, Colour(GREEN)));

			break;
		}
		break;
	case SDL_KEYUP:
		switch (event.key.keysym.sym) {
		case SDLK_0:
			engine3D.cam.position = Point3D{ 0, 0, 0 };
			break;
		}
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_w:
			if (engine3D.meshes.size() != 0) engine3D.cam.MoveBy(Point3D{ 0, 0, 1 } * engine3D.cam.move_scale);
			break;
		case SDLK_s:
			if (engine3D.meshes.size() != 0) engine3D.cam.MoveBy(Point3D{ 0, 0, -1 } * engine3D.cam.move_scale);
			break;
		case SDLK_d:
			if (engine3D.meshes.size() != 0) engine3D.cam.MoveBy(Point3D{ 1, 0, 0 } * engine3D.cam.move_scale);
			break;
		case SDLK_a:
			if (engine3D.meshes.size() != 0) engine3D.cam.MoveBy(Point3D{ -1, 0, 0 } * engine3D.cam.move_scale);
			break;
		case SDLK_e:
			if (engine3D.meshes.size() != 0) engine3D.cam.MoveBy(Point3D{ 0, 1, 0 } * engine3D.cam.move_scale);
			break;
		case SDLK_q:
			if (engine3D.meshes.size() != 0) engine3D.cam.MoveBy(Point3D{ 0, -1, 0 } * engine3D.cam.move_scale);
			break;
		}
		break;
	case SDL_QUIT:
		running = false;
		break;
	}
}

// Updates game logic
void Update() {
	
}

int main(int argc, char** argv) {
	Engine3D engine3D = Engine3D(); // Holds the SDL_Window and SDL_Renderer

	engine3D.InitSDL();

	running = true;
	while (running) {
		EventHandle(engine3D);

		Update();
		
		engine3D.Draw();
	}

	SDL_DestroyRenderer(engine3D.renderer);
	SDL_DestroyWindow(engine3D.window);

	SDL_Quit();
	exit(0);
}
