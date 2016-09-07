#version 430

uniform sampler2D myTexture;
in vec2 fragUV;

out vec4 fragColor;

in vec4 lightLoc;
in vec4 camLoc;
in vec4 fragLoc;
in vec4 fragNorm;

uniform vec3 ambCoeff = 0.4 * vec3(1,1,1);
uniform vec3 diffCoeff = 0.8 * vec3(1,1,1);
uniform vec3 specCoeff = 0.7 * vec3(1,1,1);
float specPower = 16.0;

void main()
{
	vec4 L = normalize(lightLoc - fragLoc);
	vec4 E = normalize(camLoc - fragLoc);
	vec4 H = normalize((L + E));

	vec3 ambient = ambCoeff;
	vec3 diffuse = max(dot(fragNorm,L), 0.0) * diffCoeff;
	vec3 specular = pow(max(dot(fragNorm,H), 0.0), specPower) * specCoeff;

	vec4 brightness = vec4(ambient + diffuse + specular, 1.0);

	fragColor = texture(myTexture, fragUV) * brightness;

}