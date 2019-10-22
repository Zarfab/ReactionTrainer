use<components.scad>
use<BasicShapes.scad>
use<TopPlate.scad>
use<InnerPlate.scad>
use<BottomPlate.scad>

$fn = 120;

module roundBox() {
    difference() {
        union() {
            cylinder(d=86, h=34);
            translate([0, 0, 34]) difference() {
                cylinder(d1=86, d2=82, h=2);
                translate([0, 0, 1])cylinder(d1=50, d2=56, h=3, center=true);
            }
        }
        // make the box hollow
        translate([0, 0, -0.05]) cylinder(d=62, h=34.1);
        // make space for bottom plate clip insert
        translate([0, 0, 2]) tore(31, 0.6);
        // space for key to help opening
        for(a=[0,180]) { 
            rotate(a) translate([32.5, -4.8, -0.1]) cube([12, 9.6, 3.8]);
        }
        // add space for battery holder (18650)
        translate([0, 0, 10.6]) cube([21.8, 78.3, 22.4], center=true);
        translate([0, 0, 22]) cube([6, 78.4, 6], center=true);
        // space for switch
        translate([0, -33.8, 24]) cube([20, 12, 10], center=true);
        translate([0, -38, 26]) cube([8, 12, 4], center=true);
        // m4 inserts
        /*for(i=[0:3]) {
            rotate([0, 0, 45 + i*90]) translate([0, 37, -0.1]) {
                cylinder(d=7.8, h=0.5);
                cylinder(d1=6, d2=4.8, h=9.5);
                translate([0, 0, 9.4])cylinder(d1=4.6, d2=4.4, h=10);
            }
        }*/
        for(i=[0:3]) {
            rotate([0, 0, 45 + 90*i]) translate([0, 37.2, -0.1]) #cylinder(d=10.5, h=2.3);
        }
        translate([0, 34, -0.5]) cylinder(d=12.8, h=38);
        translate([-6.4, 21, 20]) cube([12.8, 12, 14]);
        /*translate([0,0,-0.1])difference() {
            cylinder(d=88, h=3);
            cylinder(d=83, h=3);
        }*/
    }
    
    translate([31-1.8, 0, 0]) roundedRect(size=[6, 4.8, 34], radius=1.6);
    translate([-31+1.8, 0, 0]) roundedRect(size=[6, 4.8, 34], radius=1.6);
}

roundBox();