// Geometry made on 13/12/18 by replicating the unit cell 4 times to generate one complete hole and four quarter holes at the corners.

// Note that GEM Pitch = 140um and 140 cos(30) = 121.244, 140 sin(30) = 70 as can be seen in the choice of dimensions.
SetFactory("OpenCASCADE");
//+
xmax=242.488;
ymax=140.0;
thickness=50.0;
zmin=-2000.0;
zmax= 3000.0;
rad_outer=35.0;
rad_inner=25.0;
rad_rim=40.0;
cu_thick=5.0;
//+
Box(1) = {0, 0, 0, xmax, ymax, thickness}; //50um thick GEM foil, in which biconal holes are to be cut.
//+
Cone(20) = {xmax/2, ymax/2, thickness/2, 0, 0, thickness/2, rad_inner, rad_outer, 2*Pi};
//+
Cone(30) = {xmax/2, ymax/2, thickness/2, 0, 0, -thickness/2, rad_inner, rad_outer, 2*Pi};    // Making biconal holes with inner radius 25um and outer radius 35um
//+
Cone(21) = {0, 0, thickness/2, 0, 0, thickness/2, rad_inner, rad_outer, 2*Pi};
//+
Cone(31) = {0, 0, thickness/2, 0, 0, -thickness/2, rad_inner, rad_outer, 2*Pi};
//+
Cone(22) = {xmax, 0, thickness/2, 0, 0, thickness/2, rad_inner, rad_outer, 2*Pi};
//+
Cone(32) = {xmax, 0, thickness/2, 0, 0, -thickness/2, rad_inner, rad_outer, 2*Pi};
//+
Cone(23) = {xmax, ymax, thickness/2, 0, 0, thickness/2, rad_inner, rad_outer, 2*Pi};
//+
Cone(33) = {xmax, ymax, thickness/2, 0, 0, -thickness/2, rad_inner, rad_outer, 2*Pi};
//+
Cone(24) = {0, ymax, thickness/2, 0, 0, thickness/2, rad_inner, rad_outer, 2*Pi};
//+
Cone(34) = {0, ymax, thickness/2, 0, 0, -thickness/2, rad_inner, rad_outer, 2*Pi};
//+
Box(2) = {0, 0, 0, xmax, ymax, -cu_thick};    //Upper coating of copper, 5um thick.
//+
Box(3) = {0, 0, thickness, xmax, ymax, cu_thick};    //Lower coating of copper, 5um thick.

//+
Cylinder(25) = {xmax/2, ymax/2, thickness, 0, 0, cu_thick, rad_rim, 2*Pi};
//+
Cylinder(35) = {xmax/2, ymax/2, 0, 0, 0, -cu_thick, rad_rim, 2*Pi};
//+
Cylinder(26) = {0, 0, thickness, 0, 0, cu_thick, rad_rim, 2*Pi};
//+
Cylinder(36) = {0, 0, 0, 0, 0, -cu_thick, rad_rim, 2*Pi};
//+
Cylinder(27) = {xmax, 0, thickness, 0, 0, cu_thick, rad_rim, 2*Pi};
//+
Cylinder(37) = {xmax, 0, 0, 0, 0, -cu_thick, rad_rim, 2*Pi};
//+
Cylinder(28) = {xmax, ymax, thickness, 0, 0, cu_thick, rad_rim, 2*Pi};
//+
Cylinder(38) = {xmax, ymax, 0, 0, 0, -cu_thick, rad_rim, 2*Pi};   //Generating cylindrical holes in the copper coatings. Radius has been increased to allow a rim of 5um width.
//+
Cylinder(29) = {0, ymax, thickness, 0, 0, cu_thick, rad_rim, 2*Pi};
//+
Cylinder(39) = {0, ymax, 0, 0, 0, -cu_thick, rad_rim, 2*Pi};
//+
Box(4) = {0, 0, zmin-cu_thick, xmax, ymax, cu_thick};
//+
Box(5) = {0, 0, zmax, xmax, ymax, cu_thick};
//+
BooleanDifference{ Volume{1}; Delete; }{ Volume{20}; Volume{30}; Volume{31}; Volume{21}; Volume{34}; Volume{24}; Volume{32}; Volume{22}; Volume{33}; Volume{23}; Delete; }   //Cutting out the holes in the GEM foil using boolean difference.
//+
BooleanDifference{ Volume{3}; Delete; }{ Volume{25}; Volume{28}; Volume{27}; Volume{26}; Volume{29}; Delete; }
//+
BooleanDifference{ Volume{2}; Delete; }{ Volume{35}; Volume{36}; Volume{39}; Volume{38}; Volume{37}; Delete; }  //Cutting out the holes in the upper and lower coatings respectively

//+
Box(6) = {0, 0, zmin, xmax, ymax, zmax-zmin};  //Enclosing the entire volume in a box of air
//+
Coherence; // same as "BooleanFragments{ Volume{:}; Delete; }{}"

// Specify mesh sizes on middle volumes
p() = PointsOf{ Volume{1,2,3}; };
Characteristic Length {p()} = 5;

// Specify max mesh size
Mesh.CharacteristicLengthMax = 30;

Mesh.Algorithm = 6; // delaunay frontal is nicer here
