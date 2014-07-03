
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

varying vec4 v_VertexColor;
varying vec2 v_TexCoord;
varying float v_TextureUse;

void main()
{
	if (v_TextureUse == 0.0)
		gl_FragColor = v_VertexColor * texture2D(u_Texture2, v_TexCoord);
	if (v_TextureUse != 0.0)
		gl_FragColor = v_VertexColor * mix(texture2D(u_Texture1, v_TexCoord), texture2D(u_Texture2, v_TexCoord), v_TextureUse - 1.0);
}