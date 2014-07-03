
#ifdef GL_ES
varying lowp vec4 v_VertexColor;
#else
varying vec4 v_VertexColor;
#endif
varying vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	gl_FragColor = texture2D(u_Texture, v_TexCoord);
}