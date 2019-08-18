use<components.scad>
use<TopPlate.scad>
use<RoundedRect.scad>

$fn = 120;

module InnerPlate() {
    difference() {
        union() {
            cylinder(d=61.6, h=4.15);
            translate([0,0,-1.8]) difference() {
                cylinder(d=61.6, h=4.8, center=true);
                translate([0, 0, 0]) cube([62, 32, 4.85], center=true);
                translate([20, 20, 0]) cube([16, 18, 4.85], center=true);
            }
            translate([0, 32.9, -1.8]) cylinder(d=12, h = 4.8, center=true);
           translate([0, 30, -1.8]) cube([12, 6, 4.8], center=true);
        }
        translate([0, 0, 3]) cylinder(d=36, h=2);
        translate([31-1.8, 0, -0.2]) roundedRect(size=[6.4, 5.2, 4.4], radius=1.6);
        translate([-31+1.8, 0, -0.2]) roundedRect(size=[6.4, 5.2, 4.4], radius=1.6);
        translate([0, 0, 2.6]) components(tolerance=[0.4, 0.4, 0.2]);
        // space for wires
        translate([0, -9, 1]) cube([18, 4, 2], center=true);
        translate([10, -10.5, 1]) cylinder(d=6, h=2, center=true, $fn=3);
        translate([-15, -6, 1]) cylinder(d=18, h=2.1, center=true, $fn=6);
        translate([-15, 6, 1]) cylinder(d=18, h=2.1, center=true, $fn=6);
        translate([-8, 0, 1]) cube([4, 22, 2.1], center=true);
        // space for buzzer
        translate([0, 32.9, 0]) cylinder(d=12.8, h=4.4);
        translate([-3.2, 32.9, -4.9]) cylinder(d=1.2, h=5);
        translate([3.2, 32.9, -4.9]) cylinder(d=1.2, h=5);
        translate([0, 23, -4.0]) rotate([90, 0, 0]) cylinder(d=3, h=20, center=true);
        translate([0, 32.9, -4.0]) rotate([0, 90, 0]) cylinder(d=3, h=8, center=true);
        translate([0, -23, -4.0]) rotate([90, 0, 0]) cylinder(d=3, h=20, center=true);
    }
}

InnerPlate();
//#translate([0,0,4.6]) TopPlate();
