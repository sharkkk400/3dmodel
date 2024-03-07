#include "transAxis.h"

transAxis::transAxis()
{
	isDraw = false;
	item = -1;
	components.clear();
}

transAxis::~transAxis()
{
}

void transAxis::init()
{
	for (auto mesh : components) {
		mesh->alpha = 0.3;
		mesh->Topping = true;
		mesh->bind();
	}
}

void transAxis::draw()
{
	for (auto mesh : components) {
		mesh->draw(base);
	}
}

void transAxis::setdefaultStatus()
{
	for (auto mesh : components) {
		mesh->alpha = 0.3;
	}
}

void transAxis::setcheckeditem(int item)
{
	components[item]->alpha = 1.0;
}

bool transAxis::checkinitem(QVector2D viewXY)
{	
	setdefaultStatus();
	item = -1;
	for (int i = 0; i < components.size(); i++) {
		if (components[i]->checkSelect(viewXY)) {
			setcheckeditem(i);
			if (unititem.find(i) != unititem.end()) {
				setcheckeditem(unititem[i]);
			}
			item = i;
			return true;
		}
	}
	return false;
}
