#include "Edge.h"


Edge::Edge(void)
{
	this->indexVertex1 = 0;
	this->indexVertex2 = 0;
}

Edge::Edge(unsigned short indexVertex1)
{
	this->indexVertex1 = indexVertex1;
	this->indexVertex2 = 0;
}

Edge::Edge(unsigned short indexVertex1, unsigned short indexVertex2)
{
	this->indexVertex1 = indexVertex1;
	this->indexVertex2 = indexVertex2;
}

Edge::Edge(const Edge * edge)
{
	this->indexVertex1 = edge->indexVertex1;
	this->indexVertex2 = edge->indexVertex2;
}

Edge::~Edge(void)
{
}

bool Edge::operator==(const Edge & edge)
{
	return this->indexVertex1 == edge.indexVertex1 && this->indexVertex2 == edge.indexVertex2;
}