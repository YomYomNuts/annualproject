#version 110

attribute vec4 a_Position;

uniform vec4 u_Color;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_WorldMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_RotationMatrix;
uniform float u_DepthHole;
uniform float u_UseTexture;

varying vec4 v_VertexColor;
varying vec2 v_TexCoord;
varying float v_TextureUse;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_WorldMatrix * u_RotationMatrix * a_Position;
	
	v_VertexColor = u_Color;
	v_TextureUse = u_UseTexture;

	float sizeTexture = 900.0;
	float ratio = 8.0;
	float depth = -15.0;

	//v_TexCoord = vec2((a_Position.x + sizeTexture) / (sizeTexture / ratio), (a_Position.x + a_Position.y + a_Position.z + sizeTexture) / (sizeTexture / ratio));

	if (u_UseTexture == 1.0)
	{
		v_TexCoord = vec2((a_Position.x + sizeTexture) / (sizeTexture / ratio), (a_Position.z + sizeTexture) / (sizeTexture / ratio));
		if( a_Position.y >= depth)
			v_TextureUse == 1.0 + a_Position.y / depth;
		if( a_Position.y < depth)
		{
			v_TextureUse = 2.0;
			/*v_VertexColor.x = v_VertexColor.x - (u_DepthHole /v_VertexColor.x / 256.0 / 0.61);
			v_VertexColor.y = v_VertexColor.y - (u_DepthHole /v_VertexColor.y / 256.0 / 0.44);
			v_VertexColor.z = v_VertexColor.z - (u_DepthHole /v_VertexColor.z / 256.0 / 0.43);*/

		
			float black = a_Position.y / (u_DepthHole*2.0);

			v_VertexColor.x += black;
			v_VertexColor.y += black;
			v_VertexColor.z += black;
		}
	}
}