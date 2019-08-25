#ifndef CGALALIASES_H
#define CGALALIASES_H

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Aff_transformation_3.h>
#include <CGAL/Kernel_d/Aff_transformation_d.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/Direction_3.h>
#include <CGAL/Line_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/squared_distance_3.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef CGAL::Aff_transformation_3<Kernel> Aff_transformation;
typedef CGAL::Bbox_3 Bbox;
typedef Kernel::Point_3 Point;
typedef std::vector<Point> Points;
typedef Kernel::Vector_3 Vector;
typedef Kernel::Direction_3 Direction;



#endif //CGALALIASES_H
