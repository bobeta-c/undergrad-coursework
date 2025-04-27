#include "GamerMap.h"
#include <iostream>

GamerMap::GamerMap()
{}

bool GamerMap::addGamer(std::string name)
{
	return (m_map.insert(name, 0));
}

double GamerMap::hoursSpent(std::string name) const
{
	double r_val;
	if(m_map.get(name, r_val)){
		return r_val;
	}
	return -1;
}

bool GamerMap::play(std::string name, double hours)
{
	if(!m_map.contains(name) || hours < 0){
		return false;
	}
	double value;
	m_map.get(name, value);
	m_map.update(name, value + hours);
	return true;
}

int GamerMap::numGamers() const
{
	return m_map.size();
}

void GamerMap::print() const
{
	for(int i = 0; i < numGamers(); i++){
		std::string name;
		double hours;
		if(!m_map.get(i,name,hours)) std::cerr << "error line 44\n";
		std::cout << name << ' ' << hours << '\n';
	}
}
