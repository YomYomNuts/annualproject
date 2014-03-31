#version 110

attribute vec4 a_Position;

uniform vec4 u_Color;
uniform mat4 u_WorldMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

varying vec4 v_VertexColor;

void main()
{
	v_VertexColor = u_Color;
	gl_PointSize = 4.0;
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_WorldMatrix * a_Position;
}

