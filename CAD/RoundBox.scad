use<components.scad>
use<RoundedRect.scad>
use<TopPlate.scad>
use<InnerPlate.scad>
use<BottomPlate.scad>

$fn = 120;

module tore(radius_ring, radius_wire) {
     rotate_extrude () translate([radius_ring,0,0]) circle (r = radius_wire);
}


module roundBox() {
    difference() {
        union() {
            cylinder(d=86, h=32);
            translate([0, 0, 32]) difference() {
                cylinder(d1=86, d2=82, h=2);
                translate([0, 0, 1])cylinder(d1=49, d2=55, h=3, center=true);
            }
        }
        translate([0, 0, -0.05]) cylinder(d=62, h=32.1);
        translate([0, 0, 10.6]) cube([21.5, 77.9, 21.5], center=true);
        translate([-4, 39-12, 21]) cube([8, 12, 5]);
        // space for switch
        translate([0, -34.5, 24]) cube([20, 12, 6], center=true);
        translate([0, -38, 24]) cube([8, 12, 4], center=true);
        for(i=[0:3]) {
            rotate([0, 0, 45 + i*90]) translate([0, 37, -0.1]) {
                cylinder(d=7.8, h=0.5);
                cylinder(d1=6, d2=4.8, h=9.5);
                translate([0, 0, 9.4])cylinder(d1=4.6, d2=4.4, h=10);
            }
        }
        translate([0, 32.9, 20]) cylinder(d=12.2, h=16);
        translate([-6.1, 24, 20]) cube([12.2, 12, 6]);
    }
    
    translate([31-1.8, 0, 0]) roundedRect(size=[6, 4.8, 32], radius=1.6);
    translate([-31+1.8, 0, 0]) roundedRect(size=[6, 4.8, 32], radius=1.6);
}

roundBox();
//#translate([0, 0, 28.4]) components();
//#translate([0, 0, 30.4]) TopPlate();
//#translate([0, 0, 25.8]) InnerPlate();
//#translate([0, 0, -3]) BottomPlate();