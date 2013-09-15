#include "Mesh.h"

Mesh::Mesh()
{
    isOn = true;

    gridSize = 8;
    gridWidth = (mapWidth - 1)/ gridSize + 2;
    gridHeight = (mapHeight - 1) / gridSize + 2;

    for(int i = 0; i < gridHeight; i += 1)
		for(int j = 0; j < gridWidth; j += 1)
		{
			if(i == 0 || i == gridHeight - 1 || j == 0 || j == gridWidth - 1)
				points.push_back(new Anchor(Vect(j * gridSize, i * gridSize)));
			else
                points.push_back(new Point(Vect(j * gridSize, i * gridSize), 0.5));
		}

	for(int i = 1; i < gridHeight; i += 1)
		for(int j = 1; j < gridWidth; j += 1)
		{
			constraints.push_back(new Constraint(points[i * gridWidth + (j - 1)],
                                        points [i * gridWidth + j], 2, 4, {0.2f, 0.1f, 0.6f, 1.0f}));
			constraints.push_back(new Constraint(points[(i - 1) * gridWidth + j],
                                        points [i * gridWidth + j], 2, 4, {0.2f, 0.1f, 0.6f, 1.0f}));
		} // end for (i)
} // end constructor

Mesh::~Mesh()
{
    for(std::vector <Point*>::iterator i = points.begin(); i < points.end(); i += 1)
        delete *i;
    for(std::vector <Constraint*>::iterator i = constraints.begin(); i < constraints.end(); i += 1)
        delete *i;
} // end destructor

void Mesh::turnOn()
{
    isOn = true;
} // end turnOff method

void Mesh::turnOff()
{
    isOn = false;
} // end turnOn method

void Mesh::draw()
{
    for(std::vector<Constraint*>::iterator i = constraints.begin(); i < constraints.end(); i += 1)
        (*i)->draw();
} // end draw method

void Mesh::step()
{
    if (isOn) {
        float speed = 0.1; /** Temporary **/
        for(unsigned int i = 0; i < constraints.size(); i += 1) {
            constraints[i]->step(speed); /** Make update independant of timestep **/
        }
        for(unsigned int i = 0; i < points.size(); i += 1) {
            //points[i]->addForce(VECTOR(0, points[i]->getMass() * 98.1));
            points[i]->step(speed); /** Make update independant of timestep **/
        } // end for (i)
    }
    /** Cool Effect
    Vect pivot(mapWidth / 2, mapHeight / 2);
    for (unsigned int i = 0; i < points.size(); i += 1)
    {
        Vect pos = points[i]->getPosition();
        float angle = radtoDegree(atan2((pivot.y - pos.y), (pos.x - pivot.x)));
        pos.x += cos(degreetoRad(angle + 90)) * 0.1;
        pos.y -= sin(degreetoRad(angle + 90)) * 0.1;
        points[i]->setPosition(pos);
    } // end for (i)
    **/
} // end step method

void Mesh::applyForce(Vect mPos, int force, int startAngle, int endAngle) /** Add force parameter **/
{
    if (isOn) {
        float angle, length;

        for(unsigned int i = 0; i < points.size(); i += 1)
        {
            Vect pos = points[i]->getPosition();
            angle = radtoDegree(atan2((mPos.y - pos.y), (pos.x - mPos.x)));
            if (angle >= startAngle && angle <= endAngle) {
                Vect res(pos.x - mPos.x, pos.y - mPos.y);
                length = pow(res.x, 2) + pow(res.y, 2);
                if(length <= 500){ /** Mess around with distance and force **/
                    length = sqrt(length);
                    Vect dirForce((res.x / length) * force * 20,
                                  (res.y / length) * force * 20);
                    points[i]->addForce(Vect(force * dirForce.x,
                                             force * dirForce.y));
                }
            }
        } // end for (i)
    }
} // end applyForce method
