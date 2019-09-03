use<components.scad>
use<TopPlate.scad>
use<BasicShapes.scad>

$fn = 120;

module InnerPlate() {
    difference() {
        union() {
            cylinder(d=60.8, h=4.15);
            translate([0,0,-1.8]) difference() {
                cylinder(d=60.8, h=4.8, center=true);
                translate([-40, -16, -3]) cube([60.4, 32, 4.85]);
                translate([25, 0, 0]) cube([20, 12, 4.85], center=true);
                translate([20, 22, 0]) cube([16, 18, 4.85], center=true);
            }
            translate([0, 32.9, -1.8]) cylinder(d=11.2, h = 4.8, center=true);
           translate([0, 30, -1.8]) cube([11.2, 6, 4.8], center=true);
        }
        translate([0, 0, 3]) cylinder(d=36, h=2);
        translate([31-1.8, 0, -0.2]) roundedRect(size=[7.2, 6, 4.4], radius=1.6);
        translate([-31+1.8, 0, -0.2]) roundedRect(size=[7.2, 6, 4.4], radius=1.6);
        translate([0, 0, 2.6]) components(tolerance=[1, 0.8, 0.2]);
        // space for wires
        translate([0, -9, 1]) cube([18, 4, 5], center=true);
        translate([10, -10.5, 1]) cylinder(d=6, h=5, center=true, $fn=3);
        translate([-10, -10.5, 1]) rotate(180) cylinder(d=6, h=5, center=true, $fn=3);
        // space for buzzer
        translate([0, 34, 0]) cylinder(d=13.2, h=4.4);
        translate([-3.2, 34, -4.9]) cylinder(d=1.6, h=5);
        translate([3.2, 34, -4.9]) cylinder(d=1.6, h=5);
        translate([0, 25, -4.0]) rotate([90, 0, 0]) cylinder(d=5, h=30, center=true);
        translate([0, 32.9, -4.0]) rotate([0, 90, 0]) cylinder(d=4, h=8, center=true);
        translate([0, -23, -4.0]) rotate([90, 0, 0]) cylinder(d=5, h=20, center=true);
    }
}

InnerPlate();
//#translate([0,0,4.6]) TopPlate();
