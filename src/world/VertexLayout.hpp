#pragma once
#ifndef WORLD_VERTEXLAYOUT_HPP
#define WORLD_VERTEXLAYOUT_HPP
namespace ofxPlanet {
class VertexLayout {
public:
	static int VERTEX_POSITION;
	static int TEXCOORD_POSITION;
	static int POSITION_POSITION;
private:
	VertexLayout() = delete;
	~VertexLayout() = delete;
};
}
#endif