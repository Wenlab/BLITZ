#version 330 core
// this shader is designed to render texture on the screen
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D textureID;
uniform int idxCase;
uniform int yDivide;

void main()
{
	vec4 texColor =  texture(textureID,TexCoord);
	vec4 black = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 white = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 gray = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	vec4 blue = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	vec4 topColor, bottomColor;
	if (idxCase == 0) // texture on the top
	{
		topColor = texColor;
		bottomColor = black;
	}else if(idxCase == 1)
	{
		topColor = black;
		bottomColor = texColor;
	}else if(idxCase == 2)
	{
		topColor = black;
		bottomColor = black;
	}	

	if (gl_FragCoord.y < yDivide)
		FragColor = topColor;
	else
		FragColor = bottomColor;

}
