module roundedRect(size, radius)
{
    x = size[0]-radius;
    y = size[1]-radius;
    z = size[2];

    linear_extrude(height=z)
    hull()
    {
        // place 4 circles in the corners, with the given radius
        translate([(-x/2)+(radius/2), (-y/2)+(radius/2), 0])
        circle(r=radius);

        translate([(x/2)-(radius/2), (-y/2)+(radius/2), 0])
        circle(r=radius);

        translate([(-x/2)+(radius/2), (y/2)-(radius/2), 0])
        circle(r=radius);

        translate([(x/2)-(radius/2), (y/2)-(radius/2), 0])
        circle(r=radius);
    }
}



module tore(mainRadius, circleRadius, fn1 = $fn, fn2 = $fn)
{
    rotate_extrude(convexity = 10, $fn = fn1)
    translate([mainRadius, 0, 0])
    circle(r = circleRadius, $fn = fn2);
}

tore(10, 2, fn1 = 120, fn2 = 12);