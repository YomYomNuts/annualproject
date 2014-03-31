
#ifdef GL_ES
varying lowp vec4 v_VertexColor;
#else
varying vec4 v_VertexColor;
#endif

void main()
{
	gl_FragColor = v_VertexColor;
}