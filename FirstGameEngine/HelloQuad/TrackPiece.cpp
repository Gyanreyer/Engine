#include "TrackPiece.h"


TrackPiece::TrackPiece(vec3 loc, vec3 rot, vec3 size, const string texFile, TrackType type)
	: Object(loc, rot, size, texFile, "models/quad.obj")
{
	trackType = type;

	setUpTrackColliders();
}

TrackPiece::TrackPiece(vec3 loc, vec3 rot, const string texFile, TrackType type)
	: TrackPiece(loc, rot, vec3(3, 3, 0), texFile, type)
{}

TrackPiece::~TrackPiece()
{
}

void TrackPiece::setUpTrackColliders()
{
	if (trackType == TrackType::straight)
	{
		if (transform.rotation.y == 0)
		{
			colliders[0] = Transform(transform.location + vec3(-2.9f,0,0), vec3(0), vec3(0.05f,1.f,3.f));
			colliders[1] = Transform(transform.location + vec3(2.9f, 0, 0), vec3(0), vec3(0.05f, 1.f, 3.f));
		}
		else
		{
			colliders[0] = Transform(transform.location + vec3(0, 0, -2.9f), vec3(0), vec3(3.f, 1.f, 0.05f));
			colliders[1] = Transform(transform.location + vec3(0, 0, 2.9f), vec3(0), vec3(3.f, 1.f, 0.05f));
		}
	}
	else
	{
		colliders[0] = Transform(transform.location, vec3(0), vec3(0.05f,1.f,3.f));
		colliders[1] = Transform(transform.location, vec3(0), vec3(3.f, 1.f, 0.05f));

		if (transform.rotation.y < 180.f)
		{
			colliders[0].location.x += 2.9f;
		}
		else
		{
			colliders[0].location.x -= 2.9f;
		}

		if (transform.rotation.y == 0 || transform.rotation.y == 270.f)
		{
			colliders[1].location.z += 2.9f;
		}
		else
		{
			colliders[1].location.z -= 2.9f;
		}


	}
}


