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

void InitSDL(Engine3D& engine3D) {
	engine3D.windowFlags = SDL_WINDOW_SHOWN;
	engine3D.rendererFlags = SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	engine3D.window = SDL_CreateWindow(engine3D.windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, engine3D.windowFlags);
	if (!engine3D.window) {
		printf("Failed to open %d x %d window: %s\n", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	engine3D.renderer = SDL_CreateRenderer(engine3D.window, -1, engine3D.rendererFlags);
	if (!engine3D.renderer) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}
}

void SetRenderDrawColour(SDL_Renderer* renderer, Colour colour) {
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.alpha);
}

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

void Draw(Engine3D& engine3D) {
	SetRenderDrawColour(engine3D.renderer, engine3D.clearColour);
	SDL_RenderClear(engine3D.renderer);

	for (auto& mesh: engine3D.meshes) {
		Mesh2D mesh2D = mesh.ConvertTo2DMesh(engine3D.cam, engine3D.renderScale);

		SetRenderDrawColour(engine3D.renderer, mesh.colour);
		if (currentMesh == &mesh) {
			SetRenderDrawColour(engine3D.renderer, Colour(Colours::RED));
		}

		for (auto& triangle: mesh2D.triangles) {
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

			SDL_RenderDrawLinesF(engine3D.renderer, point_arr, arr_size);
			
			//std::vector<SDL_Vertex> verts = triangle.ConvertToSDL_Vertex();

			//SDL_RenderGeometry(engine3D.renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
		}
	}

	SDL_RenderPresent(engine3D.renderer);
}

int main(int argc, char** argv) {
	Engine3D engine3D = Engine3D(); // Holds the SDL_Window and SDL_Renderer

	InitSDL(engine3D);
	SDL_RenderSetScale(engine3D.renderer, engine3D.renderScale, engine3D.renderScale);

	running = true;
	while (running) {
		EventHandle(engine3D);

		Update();

		Draw(engine3D);
	}

	SDL_DestroyRenderer(engine3D.renderer);
	SDL_DestroyWindow(engine3D.window);

	SDL_Quit();
	exit(0);
}
