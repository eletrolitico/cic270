#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;
uniform int u_State;
uniform int u_Frame;
uniform bool u_Mirror;
uniform float u_OffsetX;
uniform float u_OffsetY;

void main(){
	gl_Position = u_MVP*position;
	//gl_Position = position;
	if(u_Mirror){
		v_TexCoord = vec2(texCoord.x*-1+u_OffsetX+u_Frame*u_OffsetX,texCoord.y+u_State*u_OffsetY);
	}
	else{
		v_TexCoord = vec2(texCoord.x+u_Frame*u_OffsetX,texCoord.y+u_State*u_OffsetY);
	}
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main() {
	vec4 texColor = texture(u_Texture, v_TexCoord);
	//vec4 texColor = vec4(1,0,0,1);
	color = texColor;
}