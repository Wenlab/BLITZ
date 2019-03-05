#version 330 core
// this shader is designed to render texture on the screen
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture0;
uniform int patternIdx;
//uniform int yDivide;

//rotation angle
uniform float theta;
//x-distance & y-distance
uniform float xDis;
uniform float yDis;

void main()
{
	vec4 texColor =  texture(texture0,TexCoord);
	vec4 black = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 white = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 gray = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	vec4 blue = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	vec4 topColor, bottomColor;
	vec2 v = gl_FragCoord.xy;
	v.x += xDis;
	v.y += yDis;


	if (patternIdx == 0) // texture on the top
	{
		topColor = texColor;
		bottomColor = gray;//gray;
	}else if(patternIdx == 1)
	{
		topColor = gray;
		bottomColor = texColor;
	}else if(patternIdx == 2)
	{
		topColor = black;
		bottomColor = black;
	}	


	FragColor = bottomColor;
	if (mod(v.x * cos(theta) - v.y * sin(theta) , 300.0) < 150.0) {
        if (mod( v.x * sin(theta) + v.y * cos(theta) , 300.0) > 150.0)
            FragColor = topColor;
    } else {
        if (mod( v.x * sin(theta) + v.y * cos(theta) , 300.0) < 150.0)
            FragColor = topColor;
    }
}
