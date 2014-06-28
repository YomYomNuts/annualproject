/*
#ifdef GL_ES
varying lowp vec4 v_VertexColor;
#else
varying vec4 v_VertexColor;
#endif

void main()
{
	gl_FragColor = v_VertexColor;
}*/



/*#ifdef GL_ES
varying lowp vec4 v_VertexColor;
#else
varying vec4 v_VertexColor;
#endif*/
varying vec4 v_VertexColor;
//uniform sampler2D u_Texture;

//varying vec2 v_TexCoord;

void main()
{
	//gl_FragColor = v_VertexColor + texture2D(u_Texture, v_TexCoord);
	gl_FragColor = v_VertexColor;
}