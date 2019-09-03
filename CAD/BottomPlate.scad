use <BasicShapes.scad>

$fn = 120;

module BottomPlate() {
    difference() {
        union() {
            cylinder(d1=84, d2=86, h=3);
            //translate([0,0,2.9]) cylinder(d=86, h=3.1);
            translate([0,0,3]) difference() {
                union() {
                    cylinder(d=61.6, h=3);
                    translate([0,0,2]) tore(mainRadius=30.8, circleRadius=0.55);
                }
                cylinder(d=56.8, h=3.1);
                translate([31-1.8, 0, 0]) roundedRect(size=[7.2, 6, 4.4], radius=1.6);
        translate([-31+1.8, 0, 0]) roundedRect(size=[7.2, 6, 4.4], radius=1.6);
                translate([0, 0, 3]) cube([26, 78, 6], center=true);
            }
        }
        //translate([0,0,2.9]) cylinder(d=84, h=3.2);
        /*for(i=[0:3]) {
            rotate([0, 0, 45 + i*90]) translate([0, 37, 0.1]) {
                translate([0, 0, -0.2]) cylinder(d=8.2, h=0.3);
                cylinder(d1=8.2, d2=4.4, h=2.4);
                cylinder(d=4.4, h=4.2);
            }
        }*/
    }
}

BottomPlate();