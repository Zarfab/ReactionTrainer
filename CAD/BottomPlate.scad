$fn = 120;

module BottomPlate() {
    difference() {
        cylinder(d1=84, d2=86, h=3);
        for(i=[0:3]) {
            rotate([0, 0, 45 + i*90]) translate([0, 37, 0.1]) {
                translate([0, 0, -0.2]) cylinder(d=8.2, h=0.3);
                cylinder(d1=8.2, d2=4.4, h=2.4);
                cylinder(d=4.4, h=3.2);
            }
        }
    }
}

BottomPlate();