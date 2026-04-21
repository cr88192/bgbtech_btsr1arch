// Simple low-poly chicken (side view-ish)
// units = millimeters

$fn = 6;   // low resolution = fewer facets

translate([0,0,20])
{
    // Body (main oval)
    color("white")
    translate([0, 0, 12])
      scale([1.4, 1.0, 0.9])
        sphere(d = 38, $fn = 12);

    // Head
    color("white")
    translate([22, 0, 22])
      sphere(d = 20, $fn = 6);

    // Beak (two parts: upper + lower)
    color("orange")
    translate([32, 0, 22]) {
      // upper beak
      rotate([0, -25, 0])
        scale([1.8, 1.4, 0.7])
          sphere(d = 12, $fn = 6);
      
      // lower beak
      translate([0, 0, -3])
        rotate([0, 15, 0])
          scale([1.6, 1.3, 0.6])
            sphere(d = 10, $fn = 6);
    }

    // Comb (very simplified)
    color("red")
    translate([18, 0, 34]) {
        translate([-1*3, 0, 0])
          scale([1.0, 0.5, 1.4-abs(-1*0.4)])
            sphere(d = 10, $fn = 6);
        translate([1*3, 0, 0])
          scale([1.0, 0.5, 1.4-abs(1*0.4)])
            sphere(d = 10, $fn = 6);
        translate([0*3, 0, 0])
          scale([1.0, 0.5, 1.4-abs(0*0.4)])
            sphere(d = 10, $fn = 6);

    }

    // Eye (one visible side)
    color("black")
    translate([26, 7, 24])
      sphere(d = 5, $fn = 6);
    color("black")
    translate([26, -7, 24])
      sphere(d = 5, $fn = 6);

    // Tail feathers (simple fan)
//    if(0)
//    color("white")
//    for (a = [-30:15:30]) {
//      rotate([0, a, 0])
//        translate([0, 0, 8])
//          scale([0.7, 2.2, 0.4])
//            sphere(d = 24);
//    }

    // Legs (very basic cylinders + feet)
    color("pink")
    {
      // thigh
      translate([0, -8, -18])
          cylinder(h=19, d1=8, d2=5, $fn = 6);
      // foot (flat oval)
      translate([0, -8, -18])
        rotate([90, 0, 0])
          scale([1.4, 0.4, 1.0])
            sphere(d=12, $fn = 6);

      // thigh
      translate([0, 8, -18])
          cylinder(h=19, d1=8, d2=5, $fn = 6);
      // foot (flat oval)
      translate([0, 8, -18])
        rotate([90, 0, 0])
          scale([1.4, 0.4, 1.0])
            sphere(d=12, $fn = 6);
    }
}