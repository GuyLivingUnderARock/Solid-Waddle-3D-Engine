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

void InitSDL(App& app) {
	app.windowFlags = SDL_WINDOW_SHOWN;
	app.rendererFlags = SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	app.window = SDL_CreateWindow(app.windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, app.windowFlags);
	if (!app.window) {
		printf("Failed to open %d x %d window: %s\n", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	app.renderer = SDL_CreateRenderer(app.window, -1, app.rendererFlags);
	if (!app.renderer) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}
}

void SetRenderDrawColour(SDL_Renderer* renderer, Colour colour) {
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.alpha);
}

void EventHandle(App& app) {
	SDL_WaitEvent(&event);

	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			Point3D place_pos = Point3D{ float(event.motion.x), float(event.motion.y), 1 }; // Placement position @ (X, Y, Z)
			Point3D size = Point3D{ 1, 1, 1 } * 100;

			//place_pos = Point3D{ 0, 0, 1 } * size;

			app.meshes.emplace_back(Cubeoid(app, place_pos, size));

			if (currentMesh == nullptr) {
				meshIndex = app.meshes.size() - 1;
			}

			currentMesh = &app.meshes[meshIndex];
			break;
		}
		break;
	case SDL_KEYUP:
		switch (event.key.keysym.sym) {
		case SDLK_0:
			if (app.meshes.size() > 0) {
				if (meshIndex + 1 < app.meshes.size()) {
					meshIndex++;
				}
				else {
					meshIndex = 0;
				}
				currentMesh = &app.meshes[meshIndex];
			}
			break;
		}
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_w:
			if (currentMesh != nullptr) app.cam.Move(Point3D{ 0, 0, 1 } * app.cam.move_scale);
			break;
		case SDLK_s:
			if (currentMesh != nullptr) app.cam.Move(Point3D{ 0, 0, -1 } * app.cam.move_scale);
			break;
		case SDLK_d:
			if (currentMesh != nullptr) app.cam.Move(Point3D{ 1, 0, 0 } * app.cam.move_scale);
			break;
		case SDLK_a:
			if (currentMesh != nullptr) app.cam.Move(Point3D{ -1, 0, 0 } * app.cam.move_scale);
			break;
		case SDLK_e:
			if (currentMesh != nullptr) app.cam.Move(Point3D{ 0, 1, 0 } * app.cam.move_scale);
			break;
		case SDLK_q:
			if (currentMesh != nullptr) app.cam.Move(Point3D{ 0, -1, 0 } * app.cam.move_scale);
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

void Draw(App& app) {
	SetRenderDrawColour(app.renderer, app.clearColour);
	SDL_RenderClear(app.renderer);

	for (auto& mesh: app.meshes) {
		Mesh2D mesh2D = mesh.ConvertTo2DMesh(app.cam, app.renderScale);

		SetRenderDrawColour(app.renderer, mesh.colour);
		if (currentMesh == &mesh) {
			SetRenderDrawColour(app.renderer, Colour(Colours::RED));
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

			SDL_RenderDrawLinesF(app.renderer, point_arr, arr_size);
			
			//std::vector<SDL_Vertex> verts = triangle.ConvertToSDL_Vertex();

			//SDL_RenderGeometry(app.renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
		}
	}

	SDL_RenderPresent(app.renderer);
}

int main(int argc, char** argv) {
	App app = App(); // Holds the SDL_Window and SDL_Renderer

	InitSDL(app);
	SDL_RenderSetScale(app.renderer, app.renderScale, app.renderScale);

	running = true;
	while (running) {
		EventHandle(app);

		Update();

		Draw(app);
	}

	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);

	SDL_Quit();
	exit(0);
}
