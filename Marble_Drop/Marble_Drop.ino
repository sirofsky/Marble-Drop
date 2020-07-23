//MARBLE DROP
//Giving Gravity a Whirl
//by Jacob Surovsky

//for setup, longpress one blink twice to become "gravity"
//longpress two blinks once to become "buckets"
//put gravity in between the two buckets. 
//The white face on Gravity points in the direction of game gravity

enum blinkRoles {PEG, GRAVITY, BUCKET};
byte blinkRole = PEG;

bool bChangeRole = false;
bool bLongPress = false;

byte tFace;
byte bFace;
byte luFace;
byte ldFace;
byte ruFace;
byte rdFace;

bool detached;
bool listening = false;

enum signalStates {IM_BUCKET, TOP, LEFT_UP, RIGHT_UP, NOTHING, ZIP, ZAP, ZOP};

//PEG
bool gotZip = false;
bool gotDirection = false;

byte buddyFace;

//BUCKET
enum playerRoles {PLAYER1, PLAYER2};
byte playerRole = PLAYER1;
byte bucketColor;
byte score;


//GRAVITY
bool bFace0 = false;
bool bFace1 = false;
bool bFace2 = false;
bool bFace3 = false;
bool bFace4 = false;
bool bFace5 = false;

byte gravityFace;
bool bGravityOn;

bool gotZap;
bool gotZop;
bool primeZip;

#define ZIP_DELAY 500
Timer zipTimer;

void setup() {
  // put your setup code here, to run once:
zipTimer.set(ZIP_DELAY);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (hasWoken()) {
    bLongPress = false;
  }

  if (buttonLongPressed()) {
    bLongPress = true;
  }

  if (buttonReleased()) {
    if (bLongPress) {
      // now change the role
      bChangeRole = true;
      bLongPress = false;
    }

  }
  switch (blinkRole) {
    case PEG:
      pegLoop();
      break;
     case BUCKET:
      bucketLoop();
      break;
    case GRAVITY:
      gravityLoop();
      break;
  }

  if (bLongPress) {
    //transition color
    setColor(WHITE);
  }


}

//PEG -----------------------

void pegLoop(){
if (bChangeRole) {
    blinkRole = BUCKET;
    bChangeRole = false;
  }

  setColor(YELLOW);

  FOREACH_FACE(f){
    if(!isValueReceivedOnFaceExpired(f)){ //I have a neighbor

      if(getLastValueReceivedOnFace(f) == ZIP){
        buddyFace = f;
        gotDirection = false;
        gotZip = true;
        }
   
       if(getLastValueReceivedOnFace(f) == LEFT_UP){
        bFace = (f + 1) % 6;
        gotZip = false;
        gotDirection = true;
        }
        
       else if(getLastValueReceivedOnFace(f) == RIGHT_UP){
        bFace = (f + 5) % 6;
        gotZip = false;
        gotDirection = true;

        }
       else if(getLastValueReceivedOnFace(f) == TOP){ //we're checking for Top last because it's the most important value
        bFace = f;
        gotZip = false;
        gotDirection = true;
        } 
      } 
    }

  ldFace = (bFace + 1) % 6;
  luFace = (bFace + 2) % 6;
  tFace = (bFace + 3) % 6;
  ruFace = (bFace + 4) % 6;
  rdFace = (bFace + 5) % 6;

    

    if(gotZip == true){
         setValueSentOnFace(ZAP, buddyFace);
      }
    if(gotDirection == true){
      setValueSentOnFace(ZOP, buddyFace);
      broadcast();
      }

      setColorOnFace(WHITE, bFace);
      setColorOnFace(GREEN, tFace);
//
//      setValueSentOnAllFaces(NOTHING);
//
//      setValueSentOnFace(LEFT_UP, luFace);
//      setValueSentOnFace(RIGHT_UP, ruFace);
//      setValueSentOnFace(TOP, tFace);
  }

void broadcast(){
  setValueSentOnFace(ZIP, luFace);
  setValueSentOnFace(ZIP, tFace);
  setValueSentOnFace(ZIP, ruFace);

 FOREACH_FACE(f){
    if(!isValueReceivedOnFaceExpired(f)){
      if(getLastValueReceivedOnFace(f) == ZAP)
        gotZop = false;
        gotZap = true;
      }
      if(getLastValueReceivedOnFace(f) == ZOP){

        gotZap = false;
        gotZop = true;
        }
    }

//and these faces will send out information
 
 if(gotZap == true){
  setValueSentOnFace(LEFT_UP, luFace);
  setValueSentOnFace(RIGHT_UP, ruFace);
  setValueSentOnFace(TOP, tFace);
      }

 if(gotZop == true){
//cool cool you can chill
  } 
  
  }


//BUCKET -----------------------  
void bucketLoop(){
if (bChangeRole) {
    blinkRole = GRAVITY;
    bChangeRole = false;
  }

if(buttonSingleClicked()){
  score = score + 1;
  if(score = 6){
    score == 0;
    }
  }

 bucketColor = 0;

 setValueSentOnAllFaces(IM_BUCKET);

setColor(makeColorHSB(bucketColor, 255, 255));

//have the bucket faces glow the number of their score

FOREACH_FACE(f){
  if(f > score){
    setColorOnFace(dim(makeColorHSB(bucketColor, 255, 255),125),f);
    }
  }
  
}






//GRAVITY -----------------------

void gravityLoop(){
if (bChangeRole) {
    blinkRole = PEG;
    bChangeRole = false;
  }

  byte bucketCount = 0;
  bGravityOn = false;
  
//  setColor(CYAN);

  FOREACH_FACE(f){
    if(!isValueReceivedOnFaceExpired(f)){ //I have a neighbor
      if(getLastValueReceivedOnFace(f) == IM_BUCKET){ //and they're a bucket
        bucketCount = bucketCount + 1; // how many buckets am I currently touching?
     
        //which faces are the buckets touching? 
        if(f == 0){
          bFace0 = true;
          }
        if(f == 1){
          bFace1 = true;
          }
        if(f == 2){
          bFace2 = true;
          }
        if(f == 3){
          bFace3 = true;
          }
        if(f == 4){
          bFace4 = true;
          }
        if(f == 5){
          bFace5 = true;
          }
        }
      }
      //and which faces are buckets not touching?
      else if (isValueReceivedOnFaceExpired(f)){
        if(f == 0){
          bFace0 = false;
          }
        if(f == 1){
          bFace1 = false;
          }
        if(f == 2){
          bFace2 = false;
          }
        if(f == 3){
          bFace3 = false;
          }
        if(f == 4){
          bFace4 = false;
          }
        if(f == 5){
          bFace5 = false;
          }
        
        }
    }


if(bucketCount == 2){
declareGravity();
   }
 }  

 void declareGravity(){
  
   //if the buckets are in the correct formation, what is gravity's orientation?
 if(bFace0 == true && bFace2 == true){
      gravityFace = 1;
      bGravityOn = true;
      }
    if(bFace1 == true && bFace3 == true){
      gravityFace = 2;
      bGravityOn = true;
      }
    if(bFace2 == true && bFace4 == true){
      gravityFace = 3;
      bGravityOn = true;
      }
    if(bFace3 == true && bFace5 == true){
      gravityFace = 4;
      bGravityOn = true;
      }
    if(bFace4 == true && bFace0 == true){
      gravityFace = 5;
      bGravityOn = true;
      }
    if(bFace5 == true && bFace1 == true){
      gravityFace = 0;
      bGravityOn = true;
      }

//if the buckets are in the correct orientation, change color to let us know
  if(bGravityOn == true){
  setColor(BLUE);

  //do a bit of math to know what is what
  bFace = gravityFace;
  ldFace = (bFace + 1) % 6;
  luFace = (bFace + 2) % 6;
  tFace = (bFace + 3) % 6;
  ruFace = (bFace + 4) % 6;
  rdFace = (bFace + 5) % 6;  

if(!zipTimer.isExpired()){
  setColor(CYAN);
  } else if (zipTimer.isExpired()){
    primeZip = true;
  }


  
  FOREACH_FACE(f){
    if(!isValueReceivedOnFaceExpired(f)){
      if(getLastValueReceivedOnFace(f) == ZAP)
        primeZip = false;
        gotZop = false;
        gotZap = true;
      }
      if(getLastValueReceivedOnFace(f) == ZOP){
        primeZip = false;
        gotZap = false;
        gotZop = true;
        }
    }

//and these faces will send out information
 if(primeZip == true){
  setValueSentOnAllFaces(ZIP);
  }
 
 if(gotZap == true){
  setValueSentOnFace(LEFT_UP, luFace);
  setValueSentOnFace(RIGHT_UP, ruFace);
  setValueSentOnFace(TOP, tFace);
      }

 if(gotZop == true){
  zipTimer.set(ZIP_DELAY);
  }  



  setColorOnFace(WHITE, bFace);
  }

 }
