#version 330 core
// this shader is designed to render texture on the screen
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
//uniform sampler2D texture0[0];
//uniform sampler2D texture0[1];
//uniform sampler2D texture0[2];
uniform sampler2D texture0[3];
uniform int cIdx;
uniform int patternIdx;
uniform int yDivide;

void main()
{
    vec4 texColor;
	if(cIdx == 0)
	{
	    texColor =  texture(texture0[0],TexCoord);
	}
	if(cIdx == 1)
	{
		texColor =  texture(texture0[1],TexCoord);
	}
	if(cIdx == 2)
	{
		texColor =  texture(texture0[2],TexCoord);
	}
	vec4 black = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 white = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 gray = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	vec4 topColor, bottomColor;
	if (patternIdx == 0) 
	{
		topColor = texColor;
		bottomColor = gray;
	}else if(patternIdx == 1)
	{
		topColor = gray;
		bottomColor = texColor;
	}else if(patternIdx == 2)
	{
		topColor = black;
		bottomColor = black;
	}	

	if (gl_FragCoord.y < yDivide)
		FragColor = topColor;
	else
		FragColor = bottomColor;

}
