#include <iostream>

#include "Defs.h"
#include "Structs.h"

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

SDL_FPoint Point3D::ConvertToSDL_FPoint(float focalDistance, float renderScale, Point3D camPosition, Point3D camRotation) {
	SDL_FPoint point;

	float h_x = std::sqrt(std::pow(z + camPosition.z + focalDistance, 2) + std::pow(x - camPosition.x, 2));
	float h_y = std::sqrt(std::pow(z + camPosition.z + focalDistance, 2) + std::pow(y + camPosition.y, 2));

	float angle_x = std::asin((x - camPosition.x) / h_x);
	float angle_y = std::asin((y + camPosition.y) / h_y);

	point.x = ((h_x * (z + camPosition.z + focalDistance) / focalDistance) * std::sin(angle_x)) + ((WINDOW_WIDTH/renderScale)/2);
	point.y = ((h_y * (z + camPosition.z + focalDistance) / focalDistance) * std::sin(angle_y)) + ((WINDOW_HEIGHT/renderScale)/2);

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

float ViewCam::CalcFocalDist() {
	return std::sqrt(std::powf(((WINDOW_WIDTH / 2) * std::sin(90)) / (std::sin(fov / 2)), 2) + std::powf(WINDOW_WIDTH / 2, 2));
}

void ViewCam::Move(Point3D moveBy) {
	position += moveBy;
}

Mesh2D Mesh3D::ConvertTo2DMesh(ViewCam cam, float renderScale) {
	Mesh2D mesh;
	mesh.colour = colour;

	for (int t = 0; t < triangles.size(); t++) {
		mesh.triangles.emplace_back();

		for (int p = 0; p < triangles[t].points.size(); p++) {
			mesh.points.emplace_back(triangles[t].points[p].ConvertToSDL_FPoint(cam.CalcFocalDist(), renderScale, cam.position, cam.rotation));
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

Cubeoid::Cubeoid(App& app, Point3D centre = Point3D{ 0, 0, 0 }, Point3D size_ = Point3D{ 1, 1, 1 }) {
	Point3D size = size_;

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
