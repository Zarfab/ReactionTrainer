use<components.scad>
use<RoundedRect.scad>

$fn = 120;

module TopPlate() {
    difference() {
        union() {
            cylinder(d=61.6, h=1.6);
            translate([0, 0, -1.6]) cylinder(d=35.7, h=1.6);
        }
        translate([31-1.8, 0, -0.2]) roundedRect(size=[6.4, 5.2, 2], radius=1.6);
        translate([-31+1.8, 0, -0.2]) roundedRect(size=[6.4, 5.2, 2], radius=1.6);
        translate([0, 0, -2]) components(tolerance=[0.3, 0.2, 0.1]);
        translate([0, 33, -0.2]) cylinder(d=12.8, h=2);
        translate([0.15, 1.35, 0]) roundedRect(size=[3.3, 5.5, 2], radius=0.8);
    }
}

TopPlate();