
#define NEW_OLD_SCHOOL

#ifndef NEW_OLD_SCHOOL
attribute vec4 a_Position;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelViewMatrix;

uniform vec3 u_DiffuseColor;
#endif

varying vec4 v_VertexColor;

void main()
{
/*	
#ifdef NEW_OLD_SCHOOL
*/
	v_VertexColor = gl_Color;
	gl_Position = ftransform();
	//gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	//gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;	
/*
#else

	v_VertexColor = vec4(u_DiffuseColor, 1.0);
	gl_Position = u_ProjectionMatrix * u_ModelViewMatrix * a_Position;
	*/
//#endif
}

