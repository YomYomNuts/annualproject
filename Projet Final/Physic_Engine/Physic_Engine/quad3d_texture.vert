#version 110

attribute vec4 a_Position;

uniform vec4 u_Color;

uniform mat4 u_ProjectionMatrix;

varying vec4 v_VertexColor;

uniform mat4 u_WorldMatrix;

varying vec2 v_TexCoord;

uniform mat4 u_ViewMatrix;

uniform mat4 u_RotationMatrix;

uniform float u_Scale;

uniform int u_Face;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_WorldMatrix * u_RotationMatrix * a_Position;
	
	if(u_Face == 0)
		v_TexCoord = vec2(1.0 - (u_Scale/2.0 + a_Position.z) / (u_Scale), (u_Scale/2.0 + a_Position.y) / (u_Scale));
	if(u_Face == 1)
		v_TexCoord = vec2((u_Scale/2.0 + a_Position.x) / (u_Scale), (u_Scale/2.0 + a_Position.y) / (u_Scale));
	if(u_Face == 2)
		v_TexCoord = vec2((u_Scale/2.0 + a_Position.z) / (u_Scale), (u_Scale/2.0 + a_Position.y) / (u_Scale));
	if(u_Face == 3)
		v_TexCoord = vec2(1.0 - (u_Scale/2.0 + a_Position.x) / (u_Scale), (u_Scale/2.0 + a_Position.y) / (u_Scale));
	if(u_Face == 4)
		v_TexCoord = vec2((u_Scale/2.0 + a_Position.x) / (u_Scale), (u_Scale/2.0 + a_Position.z) / (u_Scale));
	if(u_Face == 5)
		v_TexCoord = vec2((u_Scale/2.0 + a_Position.x) / (u_Scale), (u_Scale/2.0 + a_Position.z) / (u_Scale));
	
	//gl_Position = u_ProjectionMatrix * u_WorldMatrix * u_RotationMatrix * a_Position;
	//gl_Position = ftransform();
	
	//v_TexCoord = gl_MultiTexCoord0.xy;

	v_VertexColor = u_Color;
}