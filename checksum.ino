#define cs 1// do you want to add the checksum after the serial messages? 1 for yes, 0 for no

#define ARRAYSIZE 5
#define msg_timeout 1000

int msg_no = 0;
int msg_retr = 0;
unsigned long msg_ms = 0;

String msgQueue[ARRAYSIZE];

void addQueue(PGM_P s, String str = "") {
  char c;
  int total = 0;
  String tmpstr;

  while ((c = pgm_read_byte(s++)) != 0) {
    tmpstr += c;
    total += c;
  }

  for(int i = 0; i < str.length(); i++) {
    tmpstr += str[i];
    total += byte(str[i]);
  }

  if(cs) {
    tmpstr += '(';
    String t1 = String(total%256, HEX);
    t1.toUpperCase();
    tmpstr += t1;
    tmpstr += ')';
  }
  else
    tmpstr += "";

  if(msg_no < ARRAYSIZE){
    msgQueue[msg_no] = tmpstr;
    msg_no++;
  }
  else{
    //sendMes(PSTR("error")); //needs error handeling?
    //linuxReset(1000); //Reset Carambola 
  }
}

void updateQueue(void) {
  // Geen messages in queue
  if(msg_no == 0){
    answer = ' ';
    return;
  }
  
  if(answer == 'a') { //receifed accepted
    for(int i = 0; i < msg_no - 1; i++)
      msgQueue[i] = msgQueue[i+1];
    msg_no--;
    msg_retr = 0;
    msg_ms = 0;
    answer = ' ';
    return;
  }
 
  if((millis() - msg_ms < msg_timeout || (millis() < msg_ms && msg_ms < msg_timeout))) // //millis() - msg_ms < msg_timeout
    return;
  
  if(msg_retr < 3) { // resend (no answer yet)
    Serial.println(msgQueue[0]);
    msg_ms = millis();
    msg_retr += 1;      
  }
  else if(msg_retr == 3){
    //sendMes(PSTR("error")); //needs error handeling?
    msg_ms = millis();
    msg_retr += 1; 
    answer = 'a'; //temporarely to not have to type "accepted" to get next message, remove this line for proper protocol
    //linuxReset(1000); //Reset Carambola 
  }
  else {
    // Error: to many timeouts
  }
}

void sendMes(PGM_P s, String str = "") {
  char c;
  int total = 0;
  while ((c = pgm_read_byte(s++)) != 0) {
    Serial.print(c);
    total += c;
  }

  for(int i = 0; i < str.length(); i++) {
    Serial.print(str[i]);
    total += byte(str[i]);
  }

  if(cs) {
    Serial.print('(');
    Serial.print(total%256, HEX);
    Serial.println(')');
  }
  else
    Serial.println("");
}

String printlnCS(char arr[]){
  String str = arr;

  int total = 0;   
  for(int i=0; i < str.length(); i++) {
    total = (total + byte(arr[i]));
  }

  Serial.print(arr);
  if(cs) {  
    Serial.print("(");
    Serial.print(total%256, HEX);
    Serial.println(")");
  }
  else
    Serial.println();
}

String addcs(char arr[]){
  String str = arr;

  int total = 0;   
  for(int i=0; i < str.length(); i++) {
    total = (total + byte(arr[i]));
    Serial.println(total);
  }

  if(cs) {  
    str += '(';
    str += String(total%256, HEX);
    str += ')';
  }

  return(str);
}

byte CheckSum(byte arr[], int dim_array){
  int total = 0;   
  for(int i=0; i < dim_array; i++)
    total = (total + arr[i]);
  return total%256;
}


