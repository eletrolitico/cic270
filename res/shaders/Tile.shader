#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;
out vec2 pos;

uniform mat4 u_MVP;

void main(){
	gl_Position = u_MVP*position;
	//gl_Position = position;
	v_TexCoord = texCoord;
	pos = position.xy;
}

#shader fragment
#version 330 core

#define MAX_LIGHTS 20

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec2 pos;

uniform sampler2D u_Texture;
uniform float u_AmbientLight;
uniform vec2 lightPos[MAX_LIGHTS];

void main() {
	vec4 tC = texture(u_Texture, v_TexCoord);
	float l = 0;
	for(int i=0;i<MAX_LIGHTS;i++){
		if(lightPos[i] != vec2(0,0)){
			l = min(min(u_AmbientLight + max(1 - (dot(pos-lightPos[i],pos-lightPos[i])/10),0), 1) + l, 1);
		}
	}
	tC = vec4(l*tC.xyz,tC.w);
	color = tC;
}