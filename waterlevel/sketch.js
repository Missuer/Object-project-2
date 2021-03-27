
//Written by William Luk
// posts data to an Adafuit.io feed
let url = 'https://io.adafruit.com/api/v2/Missuer/feeds/waterlevel/data';
var counter=0;
let wave1=null;
let wave2=null;
let nowLevel=0;
let toLevel=0;
let popWindow=null;
let msgDiv=null;
function getData(){
 
  httpGet(url, 'json', function(result){
	  console.log(result);
    	   if(result!=null&&result[0]!=null){
    		   toLevel=parseFloat(result[0].value);
    	   }
  });
}
function setup() {
    theta=random(10)
    createCanvas(windowWidth, windowHeight);
    frameRate(30); 
    colorMode(HSB);
    wave1=new Wave({
        canvasWidth: windowWidth, 
        canvasHeight:windowHeight*0.9,
        waveWidth: 0.055, 
        waveHeight: 4,
        color: '#476aff',
        xOffset: 0,
        yOffset:windowHeight*0.1,
        speed: 0.2, 
      });
    wave2=new Wave({
        canvasWidth: windowWidth, 
        canvasHeight:windowHeight*0.9, 
        waveWidth: 0.055, 
        waveHeight: 4, 
        color: 'rgba(71, 106, 255, 0.48)', 
        xOffset: 2, 
        yOffset:windowHeight*0.1,
        speed: 0.1,
      });
    popWindow=document.getElementById("popwindow");
    msgDiv=document.getElementById("msg");
    document.getElementById("button").onclick=()=>{
    		hidePopWindow();
    };
    
}
function draw() {
	background("white");
	if(counter%80==0)
		getData();
	counter++;
    if(toLevel>nowLevel)
    	{
    		nowLevel+=1;
    		drawPopWindow(nowLevel);
    	}
    else if(nowLevel>toLevel)
    	{
    		nowLevel-=1;
    	}
    wave1.update(nowLevel);
    wave1.draw();
    wave2.update(nowLevel);
    wave2.draw();
   
}

function Wave({
    canvasWidth, 
    canvasHeight,
    waveWidth = 0.055,
    waveHeight = 6,
    xOffset = 0,
    yOffset=0,
    speed = 0.04,
    color = '#DBB77A',
  }) {
    this.points = [];
    this.startX = 0;
    this.canvasWidth = canvasWidth;
    this.canvasHeight = canvasHeight;
    this.waveWidth = waveWidth;
    this.waveHeight = waveHeight;
    this.xOffset = xOffset;
    this.speed = speed;
    this.color = color;
    this.update=function(nowRange){
    	 	this.points = [];
    	    const {
    	      startX, waveHeight, waveWidth, canvasWidth, canvasHeight, xOffset,
    	    } = this;
    	    for (let x = startX; x < startX + canvasWidth; x += 20 / canvasWidth) {
    	      const y = Math.sin(((startX + x) * waveWidth) + xOffset);
    	      const dY = canvasHeight * (1 - (nowRange / 100));
    	      this.points.push([x,yOffset+ dY + (y * waveHeight)]);
    	    }
    	    this.xOffset += this.speed;
    	    
    }
    this.draw=function(){
    	 	noStroke(0) 
    	    fill(this.color);  
    	    beginShape() 
    	    vertex(width, height);
    	    vertex(0,height);
    	  
    	    for (let x = 0; x < this.points.length; x++) {
    	    		let p=this.points[x];
    	        vertex(p[0],p[1]);
    	    }
    	    endShape()
    }
    
  }
function drawPopWindow(p){
	if(p>=30&&p<50)
	{
		if(this.last!=1)
		{
			popWindow.style.display="flex";
			msgDiv.innerText="The air conditioner starts to run.";
			this.last=1;
		}
	}
	else if(p>=50&&p<80){
		if(this.last!=2){
			popWindow.style.display="flex";
			msgDiv.innerText="Lights is turning on.";
			this.last=2;
		}
	}
	else if(p>=80){
		if(this.last!=3){
			popWindow.style.display="flex";
			msgDiv.innerText="The bathtub has been filled with water.";
			this.last=3;
		}
	}
	else
		this.last=0;
}
function hidePopWindow(){
	popWindow.style.display="none";
}
  


