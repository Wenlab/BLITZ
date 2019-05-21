#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D textureID;

//rotation angle
uniform float theta;

//patch center
uniform float centerX;
uniform float centerY;

void main()
{

	vec2 v = TexCoord.xy;
	v.x = centerX + (TexCoord.x - centerX) * cos(theta) + (TexCoord.y - centerY) * sin(theta);
	v.y = centerY + (TexCoord.x - centerX) * -sin(theta) + (TexCoord.y - centerY) * cos(theta);
	vec4 texColor =  texture(textureID, v);

	FragColor = texColor;
		
}