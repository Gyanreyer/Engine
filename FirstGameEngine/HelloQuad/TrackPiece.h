#pragma once
#include "Object.h"
class TrackPiece :
	public Object
{
public:
	enum TrackType {
		straight,
		corner
	};

	TrackPiece(vec3 loc, vec3 rot, vec3 size, const string texFile, TrackType type);
	TrackPiece(vec3 loc, vec3 rot, const string texFile, TrackType type);
	~TrackPiece();

	TrackType trackType;
	Transform colliders[2];

private:
	void setUpTrackColliders();
};

