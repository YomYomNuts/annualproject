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

	if( a_Position.y < 0)
	{
		//float black = a_Position.y / 30;

		//v_VertexColor.x += black;
		//v_VertexColor.y += black;
		//v_VertexColor.z += black;

		//v_VertexColor.x = v_VertexColor.x - (1/v_VertexColor.x / 0.61);
		//v_VertexColor.y = v_VertexColor.y - (1/v_VertexColor.y / 0.44);
		//v_VertexColor.z = v_VertexColor.z - (1/v_VertexColor.z / 0.13);

		//v_VertexColor.x = v_VertexColor.x - (19/v_VertexColor.x / 256 / 0.91);
		//v_VertexColor.y = v_VertexColor.y - (19/v_VertexColor.y / 256 / 0.79);
		//v_VertexColor.z = v_VertexColor.z - (19/v_VertexColor.z / 256 / 0.58);

		//////////////v_VertexColor.x = v_VertexColor.x - (19/v_VertexColor.x / 256 / 0.61);
		//////////////v_VertexColor.y = v_VertexColor.y - (19/v_VertexColor.y / 256 / 0.44);
		//////////////v_VertexColor.z = v_VertexColor.z - (19/v_VertexColor.z / 256 / 0.13);

		//v_VertexColor.x = v_VertexColor.x - (19/v_VertexColor.x / 256 / 0.84);
		//v_VertexColor.y = v_VertexColor.y - (19/v_VertexColor.y / 256 / 0.79);
		//v_VertexColor.z = v_VertexColor.z - (19/v_VertexColor.z / 256 / 0.69);

		v_VertexColor.x = v_VertexColor.x - (19/v_VertexColor.x / 256 / 0.61);
		v_VertexColor.y = v_VertexColor.y - (19/v_VertexColor.y / 256 / 0.44);
		v_VertexColor.z = v_VertexColor.z - (19/v_VertexColor.z / 256 / 0.43);

		float black = a_Position.y / 38;

		v_VertexColor.x += black;
		v_VertexColor.y += black;
		v_VertexColor.z += black;
	}
	/*
	else if( a_Position.y < -4)
	{
		float black = a_Position.y / 19;

		v_VertexColor.x += black;
		v_VertexColor.y += black;
		v_VertexColor.z += black;
	}
	*/
}