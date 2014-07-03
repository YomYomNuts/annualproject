#version 110

attribute vec4 a_Position;

uniform vec4 u_Color;

uniform mat4 u_ProjectionMatrix;

varying vec4 v_VertexColor;

uniform mat4 u_WorldMatrix;

uniform mat4 u_ViewMatrix;

uniform mat4 u_RotationMatrix;

uniform float u_DepthHall;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_WorldMatrix * u_RotationMatrix * a_Position;
	
	v_VertexColor = u_Color;

	if( a_Position.y < -5.0)
	{
		v_VertexColor.x = v_VertexColor.x - (u_DepthHall /v_VertexColor.x / 256.0 / 0.61);
		v_VertexColor.y = v_VertexColor.y - (u_DepthHall /v_VertexColor.y / 256.0 / 0.44);
		v_VertexColor.z = v_VertexColor.z - (u_DepthHall /v_VertexColor.z / 256.0 / 0.43);

		
		float black = a_Position.y / (u_DepthHall*2.5);

		v_VertexColor.x += black;
		v_VertexColor.y += black;
		v_VertexColor.z += black;
		
	}
}