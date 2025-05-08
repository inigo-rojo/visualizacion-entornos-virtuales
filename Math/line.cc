#include <cassert>
#include <cstdio>
#include <cstdlib>
#include "line.h"
#include "constants.h"
#include "tools.h"

Line::Line() : m_O(Vector3::ZERO), m_d(Vector3::UNIT_Y) {}
Line::Line(const Vector3 & o, const Vector3 & d) : m_O(o), m_d(d) {}
Line::Line(const Line & line) : m_O(line.m_O), m_d(line.m_d) {}

Line & Line::operator=(const Line & line) {
	if (&line != this) {
		m_O = line.m_O;
		m_d = line.m_d;
	}
	return *this;
}

// @@ TODO: Set line to pass through two points A and B
//
// Note: Check than A and B are not too close!

void Line::setFromAtoB(const Vector3 & A, const Vector3 & B) {
	m_O = A;
	m_d = (B-A)/((B-A).normalize());
}

// @@ TODO: Give the point corresponding to parameter u

Vector3 Line::at(float u) const {
	Vector3 res;

	// p(u) = m_o + u * m_d
	res = m_O + u * m_d;
	return res;
}

// @@ TODO: Calculate the parameter 'u0' of the line point nearest to P
//
// u0 = D*(P-O) / D*D , where * == dot product

float Line::paramDistance(const Vector3 & P) const {
	float res = 0.0f;

	float dot_DD = m_d.dot(m_d);
	Vector3 PmenosO = P - m_O;
	res = m_d.dot(PmenosO/dot_DD);
	return res;
}

// @@ TODO: Calculate the minimum distance 'dist' from line to P
//
// dist = ||P - (O + uD)||
// Where u = paramDistance(P)

float Line::distance(const Vector3 & P) const {
    float u= paramDistance(P);
    Vector3 D = m_d*u;
    D=D+m_O;
    D=P-D;
    return D.length();
}

void Line::print() const {
	printf("O:");
	m_O.print();
	printf(" d:");
	m_d.print();
	printf("\n");
}
