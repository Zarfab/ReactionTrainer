class PaletteItem {
  public boolean selected;
  public color c;
  
  public PaletteItem() {
    
  }
  
  public void draw(int x, int y, int w, int h) {
    pushMatrix();
    pushStyle();
    translate(x, y);
    rectMode(CORNER);
    stroke(255);
    fill(c);
    if(selected) {
      strokeWeight(3);
      rect(0, 0, w, h);
      noFill();
      stroke(0);
      rect(0, 0, w, h);
    }
    else {
      strokeWeight(1);
      rect(0, 0, w, h);
    }
    popMatrix();
    popStyle();
  }
}
