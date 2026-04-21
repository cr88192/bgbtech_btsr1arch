module leg()
{
	translate([0,0,16])
		cube([3,3,16]);
	translate([0,0,0])
		cube([3,3,16]);
	translate([0,0,0])
		cube([3,8,3]);
}

module arm()
{
	color("white")
	{
		translate([0,0,0])
			cube([16,3,3]);
		translate([16,0,0])
			cube([16,3,3]);
	}
	color("#AA5500")
	{
		translate([32,-1,1])
		{
			cube([8,5,2]);
			translate([1,3.2,0])
			rotate([0,0,25])
				cube([6,2,2]);
		}
	}
}

module neck(jawrot)
{
	color("pink")
	{
		translate([-1,0.5,4])
			cube([2,2,10]);	
	}
}

module head(jawrot)
{
	color("pink")
	{
//		translate([-2,4,10])
//			rotate([-jawrot,0,0])
//			translate([0,0,-2])
//				cube([4,4,2]);	

//		translate([-3,0,8])
//			cube([1,7,2]);	
//		translate([2,0,8])
//			cube([1,7,2]);	

//		translate([-3,0,10])
//			cube([6,8,6]);	

//		translate([-2.5,4,8])
//			cube([5,4,8]);	

		translate([0,5.5,8])
		rotate([0,0,45])
			cylinder(h=8,r1=2,r2=3.5,center=false,$fn=4);

//		translate([-0.4,8,11])
//			cube([1,1,2]);	
		translate([0,7.45,11])
			rotate([1,0,0])
		{
//			cube([1,1,4]);
			cylinder(h=4,r1=0.5,r2=0.25,center=false,$fn=4);
		}
	}

	color("black")
	{
		translate([-1,7,10-jawrot*0.08])
			cube([2,0.25,jawrot*0.1]);

		translate([-1.75,7.8,14])
			cube([0.75,0.25,0.75]);
		translate([1.0,7.8,14])
			cube([0.75,0.25,0.75]);
	}

	color("brown")
	{
		translate([-4,-1,14])
		{
//			cube([8,8,5]);
		}
		translate([0,5,16])
		{
			rotate([0,0,45])
			cylinder(h=4,r1=5,r2=3,center=false,$fn=4);
		}

	}
}

scale(2.54)
{
	color("gray")
	{
	translate([-5,0,0])
		leg();
	translate([2,0,0])
		leg();
	translate([-6,-1,32])
		cube([12,5,6]);

	translate([-4,-0.5,33])
		cube([1,4,19.5]);
	translate([3,-0.5,33])
		cube([1,4,19.5]);
	}

	color("violet")
	{
	translate([-5.25,-0.25,42])
		cube([10.5,3.5,1]);

	translate([-5.25,-0.25,45])
		cube([10.5,3.5,1]);
	}
	
	color("white")
	{
//		translate([-6,0,38])
//			cube([12,4,14]);
		translate([-5,0,38])
			cube([10,3,14]);
	}

	translate([5,0,48])
		arm();

	translate([-5,0,48])
		scale([-1, 1, 1])
			arm();

	translate([0,0,48])
	{
		neck(0);
		translate([0,0,2])
		rotate([15,0,0])
			translate([0,0,-4])
			head(15);
	}
}
