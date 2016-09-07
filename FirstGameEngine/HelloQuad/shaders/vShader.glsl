#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 norm;
layout (location = 3) uniform mat4 modelMatrix;
layout (location = 4) uniform mat4 cameraMatrix;
layout (location = 5) uniform vec3 cameraPos;
layout (location = 6) uniform vec3 lightPos;

out vec2 fragUV;

out vec4 lightLoc;
out vec4 camLoc;
out vec4 fragLoc;
out vec4 fragNorm;

void main()
{
	gl_Position = cameraMatrix * modelMatrix * vec4(position,1);
	fragUV = uv;

	fragLoc = vec4(position,1) * modelMatrix;
	lightLoc = vec4(lightPos,1);
	fragNorm = normalize(vec4(norm,0) * transpose(inverse(modelMatrix)));
	camLoc = vec4(cameraPos,1) * cameraMatrix;

}

