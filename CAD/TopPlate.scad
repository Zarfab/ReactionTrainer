use<components.scad>
use<RoundedRect.scad>

$fn = 120;

module TopPlate() {
    difference() {
        union() {
            cylinder(d=61.6, h=1.6);
            translate([0, 0, -1.6]) cylinder(d=35.2, h=1.6);
        }
        translate([31-1.8, 0, -0.2]) roundedRect(size=[6.4, 5.2, 2], radius=1.6);
        translate([-31+1.8, 0, -0.2]) roundedRect(size=[6.4, 5.2, 2], radius=1.6);
        translate([0, 0, -2]) components(tolerance=[0.4, 0.4, 0.2]);
        translate([0, 33, -0.2]) cylinder(d=12.8, h=2);
        translate([0.15, 1.30, -0.3]) roundedRect(size=[4, 6.5, 3], radius=0.8);
    }
}

TopPlate();