#include "VoronoiGenerator.h"
#include "DelaunayGenerator.h"
#include "voro++.h"

#include <algorithm>


VoronoiGenerator::VoronoiGenerator(void)
{
}

VoronoiGenerator::~VoronoiGenerator(void)
{
}

// This function returns a random double between 0 and 1
double rnd()
{
	return double(rand())/RAND_MAX;
}

void VoronoiGenerator::GenerateVoronoi(GameObjectManager & gom, const GameObject * baseGameObject, const vec3 & positionCenterRotation, const vec3 & positionLookAt, const vec3 & positionTranlate, float radius, float height, float scale, const int particles)
{
	// Set up constants for the container geometry
	vec3 containerVec(radius * scale, radius * scale, (height/2) * scale);

	// Set up the number of blocks that the container is divided into
	const int n_x = 7, n_y = 7, n_z = 7;

	// Set the number of particles that are going to be randomly introduced
	container con(-containerVec.x, containerVec.x, -containerVec.y, containerVec.y, -containerVec.z, containerVec.z, n_x, n_y, n_z, false, false, false, 8);

	// Add four plane walls to the container to make a tetrahedron
	for (unsigned int i = 0; i < baseGameObject->listFaces->size(); ++i)
	{
		Face * face = baseGameObject->listFaces->at(i);
		Edge * edge1 = face->edge1, * edge2 = face->edge2;
		vec3 * p0 = &baseGameObject->listVertices->at(edge1->indexVertex1);
		vec3 * p1 = &baseGameObject->listVertices->at(edge1->indexVertex2);
		vec3 * p2 = &baseGameObject->listVertices->at(edge2->indexVertex1);
		vec3 * p3 = &baseGameObject->listVertices->at(edge2->indexVertex2);
		vec3 size = *p0 + *p1;
		vec3 cross;
		
		if (!face->normalInverse)
		{
			if (edge1->indexVertex1 == edge2->indexVertex1)
			{
				cross = (*p0 - *p1).Cross(*p3 - *p1);
				size += *p3;
			}
			else if (edge1->indexVertex1 == edge2->indexVertex2)
			{
				cross = (*p0 - *p1).Cross(*p2 - *p1);
				size += *p2;
			}
			else if (edge1->indexVertex2 == edge2->indexVertex2)
			{
				cross = (*p0 - *p1).Cross(*p2 - *p1);
				size += *p2;
			}
			else
			{
				cross = (*p0 - *p1).Cross(*p3 - *p1);
				size += *p3;
			}
		}
		else
		{
			if (edge1->indexVertex1 == edge2->indexVertex1)
			{
				cross = (*p1 - *p0).Cross(*p3 - *p0);
				size += *p3;
			}
			else if (edge1->indexVertex1 == edge2->indexVertex2)
			{
				cross = (*p1 - *p0).Cross(*p2 - *p0);
				size += *p2;
			}
			else if (edge1->indexVertex2 == edge2->indexVertex2)
			{
				cross = (*p1 - *p0).Cross(*p2 - *p0);
				size += *p2;
			}
			else
			{
				cross = (*p1 - *p0).Cross(*p3 - *p0);
				size += *p3;
			}
		}

		size /= 3;
		cross.Normalize();
		con.add_wall(*(new wall_plane(cross.x, cross.y, cross.z, size.Length())));
	}
	float angle = radius / (height * 2);
	wall_cone cone(0, 0, -height, 0, 0, 1, atan(angle));
	con.add_wall(cone);

	// Randomly insert particles into the container, checking that they lie
	// inside the tetrahedron
	int i = 0;
	double x, y, z;
	while(i < particles)
	{
		x = rnd() * (containerVec.x * 2) - containerVec.x;
		y = rnd() * (containerVec.y * 2) - containerVec.y;
		z = rnd() * (containerVec.z * 2) - containerVec.z;
		if (con.point_inside(x, y, z))
		{
			con.put(i, x, y, z);
			i++;
		}
	}

	// Generate the Shrapnels
	voronoicell c;double *pp;
	c_loop_all vl(con);
	if(vl.start())
	{
		do
		{
			if(con.compute_cell(c, vl))
			{
				GameObject * cell = new GameObject();
				gom.listObjects->push_back(cell);

				pp = con.p[vl.ijk] + con.ps*vl.q;

				// Get the vertices and the edges
				double xx = *pp, yy = pp[1], zz = pp[2];
				double *ptsp = c.pts;
				for(int ii = 0; ii < c.p; ii++, ptsp += 3)
				{
					cell->listVertices->push_back(vec3(xx + *ptsp * 0.5, yy + ptsp[1] * 0.5, zz + ptsp[2] * 0.5));
				}

				// Get the faces
				std::vector<int> v;
				std::vector<double> normals;
				c.face_vertices(v);
				c.normals(normals);
				int j, k = 0, l = 0;
				while((unsigned int) k < v.size())
				{
					j = k + v[k++];
					std::vector<unsigned short> * listIndexVertices = new std::vector<unsigned short>();
					while(k < j)
					{
						listIndexVertices->push_back(v[k++]);
					}
					DelaunayGenerator::LaunchTriangulationGeneration(cell, listIndexVertices, vec3(normals[3 * l], normals[3 * l + 1], normals[3 * l + 2]));
					++l;
				}
				cell->TranslateAndLookAtAndScale(positionCenterRotation, positionLookAt, positionTranlate, 1.0f);
				cell->CalculateIndexes();
			}
		} while(vl.inc());
	}
}