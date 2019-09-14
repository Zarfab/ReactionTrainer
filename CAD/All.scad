use<components.scad>
use<RoundBox.scad>
use<TopPlate.scad>
use<InnerPlate.scad>
use<BottomPlate.scad>


difference() {
    union() {
        roundBox();
        //translate([0, 0, 30.4]) components();
        //#color([0.2, 1, 0.4]) translate([0, 0, 0.6]) batteryHolder();
        //#translate([0, 0, 32.4]) TopPlate();
        //color([0.9, 0.9, 0.9]) translate([0, 0, 27.8]) InnerPlate();
        translate([0, 0, -3]) BottomPlate();
    }
    rotate(60) translate([0, -50, -5]) cube([100, 100, 60]);
}