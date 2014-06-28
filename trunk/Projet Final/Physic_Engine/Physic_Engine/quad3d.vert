#version 110

attribute vec4 a_Position;

uniform vec4 u_Color;

uniform mat4 u_ProjectionMatrix;

varying vec4 v_VertexColor;

uniform mat4 u_WorldMatrix;

uniform mat4 u_ViewMatrix;

uniform mat4 u_RotationMatrix;

//varying vec2 v_TexCoord;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_WorldMatrix * u_RotationMatrix * a_Position;
	
	//v_TexCoord = a_Position.xy;
	//v_TexCoord = vec2(a_Position);

	v_VertexColor = u_Color;
}