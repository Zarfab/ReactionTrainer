class PaletteItem {
  public boolean selected;
  public color c;
  
  private int x, y, w, h;
  
  public PaletteItem(int x, int y, int w, int h) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
  }
  
  public void draw() {
    pushMatrix();
    pushStyle();
    translate(x, y);
    rectMode(CORNER);
    stroke(255);
    fill(c);
    strokeWeight(selected? 5 : 1);
    rect(0, 0, w, h);
    popMatrix();
    popStyle();
  }
  
  public boolean mouseOver() {
    return mouseX >= x && mouseY >= y && mouseX <= x + w && mouseY <= y + h;
  }
}
