#pragma once
#include "Transform.h"
#include "Model.h"
#include <string>

using std::string;

enum colliderType {
	sphere,
	axisAligned,
	none
};

class Object
{
public:
	Object(vec3 loc, vec3 rot, vec3 size, const string texFile, const string modFile);
	Object(vec3 loc, vec3 rot, vec3 size, const string texFile, const string modFile, colliderType coll);
	~Object();

	Transform transform;

	string textureFile;
	string modelFile;

	Model model;

	colliderType collider;

	bool bufferModel();
	
};

