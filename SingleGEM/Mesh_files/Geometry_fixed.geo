// Geometry made on 13/12/18 by replicating the unit cell 4 times to generate one complete hole and four quarter holes at the corners.

// Note that GEM Pitch = 140um and 140 cos(30) = 121.244, 140 sin(30) = 70 as can be seen in the choice of dimensions.
SetFactory("OpenCASCADE");
//+
xmax=242.488;
ymax=140;
//+
Box(1) = {0, 0, 0, 242.488, 140, 50}; //50um thick GEM foil, in which biconal holes are to be cut.
//+
Cone(20) = {121.244, 70, 25, 0, 0, 25, 25, 35, 2*Pi};
//+
Cone(30) = {121.244, 70, 25, 0, 0, -25, 25, 35, 2*Pi};    // Making biconal holes with inner radius 25um and outer radius 35um
//+
Cone(21) = {0, 0, 25, 0, 0, 25, 25, 35, 2*Pi};
//+
Cone(31) = {0, 0, 25, 0, 0, -25, 25, 35, 2*Pi};
//+
Cone(22) = {242.488, 0, 25, 0, 0, 25, 25, 35, 2*Pi};
//+
Cone(32) = {242.488, 0, 25, 0, 0, -25, 25, 35, 2*Pi};
//+
Cone(23) = {242.488, 140, 25, 0, 0, 25, 25, 35, 2*Pi};
//+
Cone(33) = {242.488, 140, 25, 0, 0, -25, 25, 35, 2*Pi};
//+
Cone(24) = {0, 140, 25, 0, 0, 25, 25, 35, 2*Pi};
//+
Cone(34) = {0, 140, 25, 0, 0, -25, 25, 35, 2*Pi};
//+
Box(2) = {0, 0, 0, 242.488, 140, -5};    //Upper coating of copper, 5um thick.
//+
Box(3) = {0, 0, 50, 242.488, 140, 5};    //Lower coating of copper, 5um thick.

//+
Cylinder(25) = {121.244, 70, 50, 0, 0, 5, 40, 2*Pi};
//+
Cylinder(35) = {121.244, 70, 0, 0, 0, -5, 40, 2*Pi};
//+
Cylinder(26) = {0, 0, 50, 0, 0, 5, 40, 2*Pi};
//+
Cylinder(36) = {0, 0, 0, 0, 0, -5, 40, 2*Pi};
//+
Cylinder(27) = {242.488, 0, 50, 0, 0, 5, 40, 2*Pi};
//+
Cylinder(37) = {242.488, 0, 0, 0, 0, -5, 40, 2*Pi};
//+
Cylinder(28) = {242.488, 140, 50, 0, 0, 5, 40, 2*Pi};
//+
Cylinder(38) = {242.488, 140, 0, 0, 0, -5, 40, 2*Pi};   //Generating cylindrical holes in the copper coatings. Radius has been increased to allow a rim of 5um width.
//+
Cylinder(29) = {0, 140, 50, 0, 0, 5, 40, 2*Pi};
//+
Cylinder(39) = {0, 140, 0, 0, 0, -5, 40, 2*Pi};
//+
Box(4) = {0, 0, -2005, 242.488, 140, 5};
//+
Box(5) = {0, 0, 3000, 242.488, 140, 5};
//+
BooleanDifference{ Volume{1}; Delete; }{ Volume{20}; Volume{30}; Volume{31}; Volume{21}; Volume{34}; Volume{24}; Volume{32}; Volume{22}; Volume{33}; Volume{23}; Delete; }   //Cutting out the holes in the GEM foil using boolean difference.
//+
BooleanDifference{ Volume{3}; Delete; }{ Volume{25}; Volume{28}; Volume{27}; Volume{26}; Volume{29}; Delete; }
//+
BooleanDifference{ Volume{2}; Delete; }{ Volume{35}; Volume{36}; Volume{39}; Volume{38}; Volume{37}; Delete; }  //Cutting out the holes in the upper and lower coatings respectively

//+
Box(6) = {0, 0, -2000, 242.488, 140, 5000};  //Enclosing the entire volume in a box of air
//+
Coherence; // same as "BooleanFragments{ Volume{:}; Delete; }{}"

// Specify mesh sizes on middle volumes
p() = PointsOf{ Volume{1,2,3}; };
Characteristic Length {p()} = 5;

// Specify max mesh size
Mesh.CharacteristicLengthMax = 30;

Mesh.Algorithm = 6; // delaunay frontal is nicer here
