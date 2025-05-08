#include <cmath>
#include "intersect.h"
#include "constants.h"
#include "tools.h"

/* | algo           | difficulty | */
/* |----------------+------------| */
/* | BSPherePlane   |          1 | */
/* | BBoxBBox       |          2 | */
/* | BBoxPlane      |          4 | */

// @@ TODO: test if a BSpheres intersects a plane.
//! Returns :
//   +IREJECT outside
//   -IREJECT inside
//    IINTERSECT intersect

int BSpherePlaneIntersect(const BSphere *bs, Plane *pl) {
    Vector3 centro= bs->getPosition();
    float radio=bs->getRadius();
    float distancia= pl->distance(centro);
    if(radio*radio > distancia*distancia){
        return IINTERSECT;
    }
    else{
        if ((pl->whichSide(centro))>0)
        {
            return +IREJECT;
        }else{
            return -IREJECT;
        }
    }
}


// @@ TODO: test if two BBoxes intersect.
//! Returns :
//    IINTERSECT intersect
//    IREJECT don't intersect

int  BBoxBBoxIntersect(const BBox *bba, const BBox *bbb ) {
    Vector3 mina= bba->m_min;
    Vector3 maxa= bba->m_max;
    Vector3 minb= bbb->m_min;
    Vector3 maxb= bbb->m_max;
    if((mina[0] <= maxb[0] and minb[0]<=maxa[0])or(minb[0] <= maxa[0] and mina[0]<=maxb[0]))
    {
        //colisiona en el plano x
        if((mina[1] <= maxb[1] and minb[1]<=maxa[1])or(minb[1] <= maxa[1] and mina[1]<=maxb[1]))
        {
            //colisionan en el plano y
            if((mina[2] <= maxb[2] and minb[2]<=maxa[2])or(minb[2] <= maxa[2] and mina[2]<=maxb[2]))
            {
                //colisionan en el plano z
                return IINTERSECT;
            }
        }
    }
    return IREJECT;
}

// @@ TODO: test if a BBox and a plane intersect.
//! Returns :
//   +IREJECT outside
//   -IREJECT inside
//    IINTERSECT intersect

int  BBoxPlaneIntersect (const BBox *theBBox, Plane *thePlane) {
    Vector3 p0= theBBox->m_min;
    Vector3 p7= theBBox->m_max;
    Vector3 p1= Vector3(p7[0],p0[1],p0[2]);
    Vector3 p2= Vector3(p7[0],p7[1],p0[2]);
    Vector3 p3= Vector3(p0[0],p7[1],p0[2]);
    Vector3 p4= Vector3(p7[0],p0[1],p7[2]);
    Vector3 p5= Vector3(p0[0],p0[1],p7[2]);
    Vector3 p6= Vector3(p0[0],p7[1],p7[2]);
    //comparar los puntos con el plano haciendo conteo de en que direccion del plano estan(si hay alguno en ambos lados o en el plano hay colision) hacer for en un array con which side
    Vector3 puntos[8]={p0,p1,p2,p3,p4,p5,p6,p7};
    bool dentro=false,fuera=false,plano=false;
    for(int i=0;i<8;i++)
    {
        int donde=thePlane->whichSide(puntos[i]);
        if(donde==0)
            plano=true;
        else if(donde==-1)
            dentro=true;
        else
            fuera=true;
    }
    if(plano==true or (fuera==true and dentro==true))
        return IINTERSECT;
    else if(fuera==true)
        return +IREJECT;
    else
        return -IREJECT;
    
}

// Test if two BSpheres intersect.
//! Returns :
//    IREJECT don't intersect
//    IINTERSECT intersect

int BSphereBSphereIntersect(const BSphere *bsa, const BSphere *bsb ) {

	Vector3 v;
	v = bsa->m_centre - bsb->m_centre;
	float ls = v.dot(v);
	float rs = bsa->m_radius + bsb->m_radius;
	if (ls > (rs * rs)) return IREJECT; // Disjoint
	return IINTERSECT; // Intersect
}


// Test if a BSpheres intersect a BBox.
//! Returns :
//    IREJECT don't intersect
//    IINTERSECT intersect

int BSphereBBoxIntersect(const BSphere *sphere, const BBox *box) {

	float d;
	float aux;
	float r;

	r = sphere->m_radius;
	d = 0;

	aux = sphere->m_centre[0] - box->m_min[0];
	if (aux < 0) {
		if (aux < -r) return IREJECT;
		d += aux*aux;
	} else {
		aux = sphere->m_centre[0] - box->m_max[0];
		if (aux > 0) {
			if (aux > r) return IREJECT;
			d += aux*aux;
		}
	}

	aux = (sphere->m_centre[1] - box->m_min[1]);
	if (aux < 0) {
		if (aux < -r) return IREJECT;
		d += aux*aux;
	} else {
		aux = sphere->m_centre[1] - box->m_max[1];
		if (aux > 0) {
			if (aux > r) return IREJECT;
			d += aux*aux;
		}
	}

	aux = sphere->m_centre[2] - box->m_min[2];
	if (aux < 0) {
		if (aux < -r) return IREJECT;
		d += aux*aux;
	} else {
		aux = sphere->m_centre[2] - box->m_max[2];
		if (aux > 0) {
			if (aux > r) return IREJECT;
			d += aux*aux;
		}
	}
	if (d > r * r) return IREJECT;
	return IINTERSECT;
}

// Test if a Triangle intersects a ray.
//! Returns :
//    IREJECT don't intersect
//    IINTERSECT intersect

int IntersectTriangleRay(const Vector3 & P0,
						 const Vector3 & P1,
						 const Vector3 & P2,
						 const Line *l,
						 Vector3 & uvw) {
	Vector3 e1(P1 - P0);
	Vector3 e2(P2 - P0);
	Vector3 p(crossVectors(l->m_d, e2));
	float a = e1.dot(p);
	if (fabs(a) < Constants::distance_epsilon) return IREJECT;
	float f = 1.0f / a;
	// s = l->o - P0
	Vector3 s(l->m_O - P0);
	float lu = f * s.dot(p);
	if (lu < 0.0 || lu > 1.0) return IREJECT;
	Vector3 q(crossVectors(s, e1));
	float lv = f * q.dot(l->m_d);
	if (lv < 0.0 || lv > 1.0) return IREJECT;
	uvw[0] = lu;
	uvw[1] = lv;
	uvw[2] = f * e2.dot(q);
	return IINTERSECT;
}

/* IREJECT 1 */
/* IINTERSECT 0 */

const char *intersect_string(int intersect) {

	static const char *iint = "IINTERSECT";
	static const char *prej = "IREJECT";
	static const char *mrej = "-IREJECT";
	static const char *error = "IERROR";

	const char *result = error;

	switch (intersect) {
	case IINTERSECT:
		result = iint;
		break;
	case +IREJECT:
		result = prej;
		break;
	case -IREJECT:
		result = mrej;
		break;
	}
	return result;
}
