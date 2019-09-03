use<components.scad>
use<BasicShapes.scad>

$fn = 120;

module TopPlate() {
    difference() {
        union() {
            cylinder(d=60.8, h=1.6);
            translate([0, 0, -1.6]) cylinder(d=35.2, h=1.6);
        }
        translate([31-1.8, 0, -0.2]) roundedRect(size=[7.2, 6, 2], radius=1.6);
        translate([-31+1.8, 0, -0.2]) roundedRect(size=[7.2, 6, 2], radius=1.6);
        translate([0, 0, -2.1]) components(tolerance=[0.6, 0.4, 0.2]);
        translate([0, 34, -0.2]) cylinder(d=12.8, h=2);
        translate([0.15, 1.30, -0.3]) roundedRect(size=[4, 6.5, 3], radius=0.8);
    }
}

TopPlate();