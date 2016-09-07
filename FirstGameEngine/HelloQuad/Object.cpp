#include "Object.h"


Object::Object(vec3 loc, vec3 rot, vec3 size, const string texFile, const string modFile) : Object(loc, rot, size, texFile, modFile, colliderType::none)
{
}

Object::Object(vec3 loc, vec3 rot, vec3 size, const string texFile, const string modFile, colliderType coll)
{
	transform = Transform(loc, rot, size);
	textureFile = texFile;
	modelFile = modFile;
	collider = coll;

}

Object::~Object()
{
}

bool Object::bufferModel()
{
	return model.buffer(modelFile);
}

