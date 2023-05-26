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
Mesh3D* currentMesh = nullptr;
int meshIndex = -1;

void EventHandle(Engine3D& engine3D) {
	SDL_WaitEvent(&event);

	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			Point3D place_pos = Point3D{ float(event.motion.x), float(event.motion.y), 1 }; // Placement position @ (X, Y, Z)
			Point3D size = Point3D{ 1, 1, 1 } * 100;

			//place_pos = Point3D{ 0, 0, 1 } * size;

			engine3D.meshes.emplace_back(Cubeoid(engine3D, place_pos, size));

			if (currentMesh == nullptr) {
				meshIndex = engine3D.meshes.size() - 1;
			}

			currentMesh = &engine3D.meshes[meshIndex];
			break;
		}
		break;
	case SDL_KEYUP:
		switch (event.key.keysym.sym) {
		case SDLK_0:
			if (engine3D.meshes.size() > 0) {
				if (meshIndex + 1 < engine3D.meshes.size()) {
					meshIndex++;
				}
				else {
					meshIndex = 0;
				}
				currentMesh = &engine3D.meshes[meshIndex];
			}
			break;
		}
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_w:
			if (currentMesh != nullptr) engine3D.cam.Move(Point3D{ 0, 0, 1 } * engine3D.cam.move_scale);
			break;
		case SDLK_s:
			if (currentMesh != nullptr) engine3D.cam.Move(Point3D{ 0, 0, -1 } * engine3D.cam.move_scale);
			break;
		case SDLK_d:
			if (currentMesh != nullptr) engine3D.cam.Move(Point3D{ 1, 0, 0 } * engine3D.cam.move_scale);
			break;
		case SDLK_a:
			if (currentMesh != nullptr) engine3D.cam.Move(Point3D{ -1, 0, 0 } * engine3D.cam.move_scale);
			break;
		case SDLK_e:
			if (currentMesh != nullptr) engine3D.cam.Move(Point3D{ 0, 1, 0 } * engine3D.cam.move_scale);
			break;
		case SDLK_q:
			if (currentMesh != nullptr) engine3D.cam.Move(Point3D{ 0, -1, 0 } * engine3D.cam.move_scale);
			break;
		}
		break;
	case SDL_QUIT:
		running = false;
		break;
	}
}

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
