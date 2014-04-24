import processing.serial.*;     // import the Processing serial library
Serial bluetoothPort;
char val;
boolean alert;
boolean alertDrawFlag;
int alertCount;

void alertDraw()
{
  if (alert == false)
  {
    return;
  }
  
  fill(240, 240, 240);
  textSize(36);
  text("Close your bag", 170, 310);
  ++alertCount;
  if (alertCount > 120)
  {
    alertCount = 0;
    alert = false;
  }
}

void setup()
{
  size(600, 600);
  frameRate(60);
  noStroke();
  
  alert = false;
  alertDrawFlag = false;
  alertCount = 0;
  bluetoothPort = new Serial(this, "/dev/tty.AdafruitEZ-Link3f5a-SPP", 9600);

  textFont(createFont("NanumGothic-48", 48));
}

void draw()
{
  background(20, 20, 20);
  val = (char)bluetoothPort.read();
  
  if (val == 'O')
  {
    alert = true;
  }
  
  alertDraw();
}
