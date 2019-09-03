use <BasicShapes.scad>

module ledRing(nbLeds=12, diamOut=52, diamIn=36, tolerance=[0, 0, 0]) {
    union() {
        // PCB
        difference() {
            cylinder(
                    d=diamOut+tolerance[0], 
                    h=1.5+tolerance[2], 
                    center=true, 
                    $fn=60);
            cylinder(
                    d=diamIn-tolerance[0], 
                    h=1.6+tolerance[2], 
                    center=true, 
                    $fn=60);
            for(i=[0:3]) {
                rotate([0, 0, 45+i*90]) translate([48.6/2, 0, 0]) cylinder(d=2 - tolerance[1], h=1.8 + tolerance[2], center=true, $fn=20);
            }
        }
        // LEDs and capacitors
        for(i=[0:nbLeds-1]) {
            rotate([0, 0, i*360/nbLeds]) {
                translate([0, (diamOut-6.6)/2, 1.6+tolerance[2]]) cube([6.4, 5.4, 1.6]+tolerance, center=true);
                translate([0, (diamOut-13.6)/2, 1.3+tolerance[2]]) cube([3.2, 2.2, 1]+tolerance, center=true);
            }
        }
        // space for wires
        translate([0, diamOut*0.42, -2.5]) cube([12, 6, 5.05], center=true);
    }
}


module VL6180X(tolerance=[0, 0, 0]) {
    difference() {
        union() {
            roundedRect([20.5, 17.8, 1.6] + tolerance, 1.5, $fn = 30);
            translate([-1.5, -1.3, 0]) cube([3, 5.15, 3]+ tolerance);
            translate([0, 0, 1.25]) cube([18.7, 8.1, 2.5]+ tolerance, center=true);
            translate([4, 4.45, 1.25]) cube([3.6, 2, 2.5]+ tolerance, center=true);
            translate([0, -6.32, 1.25]) cube([17.8, 2.6, 2.5]+ tolerance, center=true);
            // space for wires
            translate([0, -6.32, -2.5]) cube([18, 3, 5.05], center=true);
        }
        translate([-7.6, 6.25, -0.1]) cylinder(d=2.6-tolerance[0], h=2, $fn=30);
        translate([7.6, 6.25, -0.1]) cylinder(d=2.6-tolerance[0], h=2, $fn=30);
        /*translate([0, -6.32, -0.1]) for(i=[0:6]) {
            translate([(i-3) * 2.54, 0, 0]) cylinder(d=1, h=2, $fn=20);
        }*/
    }
}





module d1Mini32(tolerance=[0, 0, 0]) { // whole size 31.5 x 39
    x = tolerance[0];
    y = tolerance[1];
    translate([-15.75, -19.5, 0]) linear_extrude(1.5) polygon(points=[[-x/2,-y/2],[-x/2,36.5],[2.5,39+y/2],[29,39+y/2], [31.5+x/2, 36.5], [31.5+x/2, 6.5], [29, 6.5], [28.5, 6], [28.5, -y/2]]);
    // esp32
    translate([0, 6.7, -0.4]) cube([18, 25.6, 0.8]+tolerance, center=true);
    translate([0, 4, -0.9]) cube([15.7, 17.5, 4.7]+tolerance, center=true);
    translate([0, -17.5, -0.7]) cube([7.7, 5.7, 4.1]+tolerance, center=true);
    translate([0, -13, -0.59]) cube([20, 12, 1.2]+tolerance, center=true);
    translate([10, -19, -3.25]) cube([3.5, 4.5, 3.3]+tolerance);
    //translate([13.15, 0, 2.4]) cube([5.2, 26, 1.82]+tolerance, center=true);
    translate([-13.15, 0, 2.4]) cube([5.2, 26, 3.82]+tolerance, center=true);
}



module buzzer() {
    union() {
        difference() {
            cylinder(d=12, h=8.5, $fn=60);
            translate([0, 0, 4.6]) cylinder(d=3, h=4, $fn=20);
        }
        translate([3.25, 0, -5.5]) cylinder(d=0.5, h=6);
        translate([-3.25, 0, -5.5]) cylinder(d=0.5, h=6);
    }
}


module batteryHolder() {
    translate([0, 0, 11])
    difference() {
        cube([21.4, 77.9, 22], center=true);
        translate([0, 0, -4]) cube([22, 67, 18], center=true);
    }
    translate([0, -36, 23]) cube([3, 5, 3], center=true);
    translate([0, 36, 23]) cube([3, 5, 3], center=true);
    rotate([90, 0, 0]) translate([0, 10, 0]) cylinder(d=18, h=65.2, center=true);
}



module components(tolerance=[0.4, 0.4, 0.2]) {
    color([0.4,0.4,0.4]) translate([0, 0, 0.8]) rotate([0, 0, 90]) ledRing(tolerance=tolerance);
    color([0.8,0.4,0.8])translate([0, 34, -2.5]) buzzer();
    color([0.4,1,0.1])VL6180X(tolerance=tolerance);
    color([0.0,0.4,1])translate([0, 0, -4]) rotate([0, 0, 90])d1Mini32(tolerance=tolerance);
}
//d1Mini32();
//components();

batteryHolder();