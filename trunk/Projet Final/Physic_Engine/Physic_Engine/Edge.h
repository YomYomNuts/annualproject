#ifndef __EDGE__
#define __EDGE__

#include "../../EsgiGL/Common/vector.h"

class Edge
{
	// Attributs
public:
	unsigned short indexVertex1;
	unsigned short indexVertex2;
protected:
private:
	
	//Methods
public:
	Edge(void);
	Edge(unsigned short indexVertex1);
	Edge(unsigned short indexVertex1, unsigned short indexVertex2);
	Edge(const Edge * edge);
	~Edge(void);
	bool operator==(const Edge & edge);
protected:
private:
};

#endif