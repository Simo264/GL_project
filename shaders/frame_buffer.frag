#version 330 core

in 	vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;

#define EFFECT_INVERSION

vec4 inversionEffect();
vec4 grayscaleEffect();
vec4 kernelEffect();
vec4 blurEffect();

void main()
{ 
#ifdef EFFECT_INVERSION
	FragColor = inversionEffect();

#elif defined EFFECT_GRAYSCALE
	FragColor = grayscaleEffect();

#elif defined EFFECT_KERNEL
	FragColor = kernelEffect();

#elif defined EFFECT_BLUR
	FragColor = blurEffect();

#else
	FragColor = texture(screenTexture, TexCoords);

#endif

}


vec4 inversionEffect()
{
	return vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
}

vec4 grayscaleEffect()
{
	vec4 tex = texture(screenTexture, TexCoords);
	float average = 0.2126 * tex.r + 0.7152 * tex.g + 0.0722 * tex.b;
	return vec4(average, average, average, 1.0);
}

vec4 kernelEffect()
{
	const float offset = 1.0 / 300.0;  
	vec2 offsets[9] = vec2[](
		vec2(-offset,  offset), // top-left
		vec2( 0.0f,    offset), // top-center
		vec2( offset,  offset), // top-right
		vec2(-offset,  0.0f),   // center-left
		vec2( 0.0f,    0.0f),   // center-center
		vec2( offset,  0.0f),   // center-right
		vec2(-offset, -offset), // bottom-left
		vec2( 0.0f,   -offset), // bottom-center
		vec2( offset, -offset)  // bottom-right    
	);
	float kernel[9] = float[](
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	);
	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel[i];

	return vec4(col, 1.0);
}

vec4 blurEffect()
{
	const float offset = 1.0 / 300.0;  
	vec2 offsets[9] = vec2[](
		vec2(-offset,  offset), // top-left
		vec2( 0.0f,    offset), // top-center
		vec2( offset,  offset), // top-right
		vec2(-offset,  0.0f),   // center-left
		vec2( 0.0f,    0.0f),   // center-center
		vec2( offset,  0.0f),   // center-right
		vec2(-offset, -offset), // bottom-left
		vec2( 0.0f,   -offset), // bottom-center
		vec2( offset, -offset)  // bottom-right    
	);
	float kernel[9] = float[](
    1.0/16, 2.0/16, 1.0/16,
    2.0/16, 4.0/16, 2.0/16,
    1.0/16, 2.0/16, 1.0/16  
	);

	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	}
	
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel[i];

	return vec4(col, 1.0);

}

