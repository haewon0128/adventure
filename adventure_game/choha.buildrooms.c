//HAEWON CHO


#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

int obc[7][7]; //to save conntions in array

int IsGraphFull()      // see of obc is full.
{


int count = 0;			//count how many connections each room has 
int i, j;
for(i=0; i<7; i++)
{
for(j=0; j<7; j++)
{
if(obc[i][j] == 1)		//if the the roomi and room j is connected count ++
{
count++;
}
}
if(count < 3)
{
return 0;			//if the connection is less than 3 then return 0
} 
else
{
count=0;				//if the connection is more than 3 then initaialize count and go to next
}
}

return 1;
}





int getRandomRoom()
{

//time_t t;
//srand ((unsigned)time(&t));
int num = (rand()%7);				//get random number between 0-6

return num;



}

int CanAddConnection(int x)
{
int i;
int count = 0;
for(i=0; i<7; i++)			//from room0 -room6 check if roomx can be connected with more room
{
if(obc[x][i]==1)
{
count++;
}
}
if(count>=6)				//if connection is more than 6 then return 0 because it can't have more connection
return 0;

else
return 1;				//if connection is less than 6 then it can have more connection

}


int ConnectionAlreadyExists(int x, int y)		//see if chosen rooms are already connected
{
if(obc[x][y]==1)			//check chosen two numbers
return 1;			//if they are connected the return 1

else
return 0;


}

void ConnectRoom(int x, int y)			//if they fulfill the condition to be connected, then connect the room
{
obc[x][y] = 1;					//conenct room x and room y
obc[y][x] = 1;					//connect both way

}

int IsSameRoom(int x, int y)			//check if the chosen numbers are same number
{
if(x == y)
return 1;					//if they are same then return 1

else 
return 0;

}



void AddRandomConnection()			//with above functions, connect randomly chosen rooms
{
int a;
int b;


do{
a = getRandomRoom();				//a is random number
}

while (CanAddConnection(a) == 0);			//while the connections condition is fulfilled


do
{
b = getRandomRoom();				//get another randome number
}
while (CanAddConnection(b) == 0||IsSameRoom(a,b) == 1 || ConnectionAlreadyExists(a,b)==1); //while valid random number

ConnectRoom(a,b);     // if the two numbers fulfill the condition then connect the rooms

}


int main(void)
{


// put room names in the array
time_t t;
char roomnames[10][8];
strcpy(roomnames[0],"Korea");
strcpy(roomnames[1],"Denmark");
strcpy(roomnames[2],"Taiwan");
strcpy(roomnames[3],"Russia");
strcpy(roomnames[4],"Spain");
strcpy(roomnames[5],"China");
strcpy(roomnames[6],"France");
strcpy(roomnames[7],"Italy");
strcpy(roomnames[8],"India");
strcpy(roomnames[9],"Germany");




srand((unsigned) time(&t));				// to get random numbers
char pRooms[7][8];					//to save picked random rooms
int i=0;
int r;
char* p= "picked";					// mark picked room



for(i=0; i<7; i++)
{
int num=(rand()%10);					// get random number
r= strcmp(roomnames[num],p);				// compare random room and picked
if (r == 0)						// if the random room is alrady picked then pick another random room
{
i--;
}
else if(r != 0){					// if the random room is not picked yet then put the name in picked room and 
strcpy(pRooms[i],roomnames[num]);			// write picked in the room position
strcpy(roomnames[num],p);
}
}



char buffer [50];		
int s=getpid();							//get pid to add at the name of directory
sprintf(buffer, "choha.rooms.%d",s);				//save choha.rooms.pid into buffer



int roomdir=mkdir(buffer,0755);					//make room directory with the name in buffer and give permission 0755


char pathfile[50];						
sprintf(pathfile, "%s/room0",buffer);				//save pathfile choha.rooms.pid/room0
int fd0 = open(pathfile, O_RDWR | O_CREAT |  O_TRUNC, S_IRUSR | S_IWUSR);	//make file with path in buffer, read and write,
										// create new file, write newly

sprintf(pathfile, "%s/room1",buffer);						// same with room0
int fd1 = open(pathfile, O_RDWR | O_CREAT |  O_TRUNC, S_IRUSR | S_IWUSR);

sprintf(pathfile, "%s/room2",buffer);
int fd2 = open(pathfile, O_RDWR | O_CREAT |  O_TRUNC, S_IRUSR | S_IWUSR);

sprintf(pathfile, "%s/room3",buffer);
int fd3 = open(pathfile, O_RDWR | O_CREAT |  O_TRUNC, S_IRUSR | S_IWUSR);


sprintf(pathfile, "%s/room4",buffer);
int fd4 = open(pathfile, O_RDWR | O_CREAT |  O_TRUNC, S_IRUSR | S_IWUSR);


sprintf(pathfile, "%s/room5",buffer);
int fd5 = open(pathfile, O_RDWR | O_CREAT |  O_TRUNC, S_IRUSR | S_IWUSR);


sprintf(pathfile, "%s/room6",buffer);
int fd6 = open(pathfile, O_RDWR | O_CREAT |  O_TRUNC, S_IRUSR | S_IWUSR);

//make file room0~room6


while(IsGraphFull()==0)			//connect rooms until the graph is full
{
AddRandomConnection();
}



char *end = "ROOM TYPE:END ROOM";	// save strings to use in the file
char *str = "ROOM TYPE:START ROOM";
char *r_n = "ROOM NAME: ";
char *mid = "ROOM TYPE:MID ROOM";





char *nl = "\n";
char cnt[50];
int ccount;
int b;


// fill files room0~room6


//fd0
write(fd0,r_n, strlen(r_n)*sizeof(char));			// write roomname: in room0
write(fd0,pRooms[0],strlen(pRooms[0])*sizeof(char));		//name from picked room[0]
write(fd0,nl,strlen(nl)*sizeof(char));				//add new line
ccount =0;
for(b=0;b<7;b++)
{
if(obc[0][b]==1)						//using obc array to check if the room is connected room0~6
{
ccount++;							// if it's connected then count ++
sprintf(cnt, "CONNECTION %d:",ccount);				// save connection 1~n: into cnt
write(fd0,cnt,strlen(cnt)*sizeof(char));			//write connection1~n in the file
write(fd0,pRooms[b],strlen(pRooms[b])*sizeof(char));		//write connected room
write(fd0,nl,strlen(nl)*sizeof(char));				//add newline
}
}

write(fd0,str,strlen(str)*sizeof(char));			//write room type : start room because it's fd0
write(fd0,nl,strlen(nl)*sizeof(char));


//same with fd0
//fd1
write(fd1,r_n, strlen(r_n)*sizeof(char));
write(fd1,pRooms[1],strlen(pRooms[1])*sizeof(char));
write(fd1,nl,strlen(nl)*sizeof(char));
ccount =0;
for(b=0;b<7;b++)
{
if(obc[1][b]==1)
{
ccount++;
sprintf(cnt, "CONNECTION %d:",ccount);
write(fd1,cnt,strlen(cnt)*sizeof(char));
write(fd1,pRooms[b],strlen(pRooms[b])*sizeof(char));
write(fd1,nl,strlen(nl)*sizeof(char));
}
}

write(fd1,mid,strlen(mid)*sizeof(char));			//write room type midroom because it's fd1
write(fd1,nl,strlen(nl)*sizeof(char));


//same with fd0
//fd2
write(fd2,r_n, strlen(r_n)*sizeof(char));
write(fd2,pRooms[2],strlen(pRooms[2])*sizeof(char));
write(fd2,nl,strlen(nl)*sizeof(char));
ccount =0;
for(b=0;b<7;b++)
{
if(obc[2][b]==1)
{
ccount++;
sprintf(cnt, "CONNECTION %d:",ccount);
write(fd2,cnt,strlen(cnt)*sizeof(char));
write(fd2,pRooms[b],strlen(pRooms[b])*sizeof(char));
write(fd2,nl,strlen(nl)*sizeof(char));
}
}

write(fd2,mid,strlen(mid)*sizeof(char));
write(fd2,nl,strlen(nl)*sizeof(char));


//fd3
write(fd3,r_n, strlen(r_n)*sizeof(char));
write(fd3,pRooms[3],strlen(pRooms[3])*sizeof(char));
write(fd3,nl,strlen(nl)*sizeof(char));
ccount =0;
for(b=0;b<7;b++)
{
if(obc[3][b]==1)
{
ccount++;
sprintf(cnt, "CONNECTION %d:",ccount);
write(fd3,cnt,strlen(cnt)*sizeof(char));
write(fd3,pRooms[b],strlen(pRooms[b])*sizeof(char));
write(fd3,nl,strlen(nl)*sizeof(char));
}
}

write(fd3,mid,strlen(mid)*sizeof(char));
write(fd3,nl,strlen(nl)*sizeof(char));


//fd4
write(fd4,r_n, strlen(r_n)*sizeof(char));
write(fd4,pRooms[4],strlen(pRooms[4])*sizeof(char));
write(fd4,nl,strlen(nl)*sizeof(char));
ccount =0;
for(b=0;b<7;b++)
{
if(obc[4][b]==1)
{
ccount++;
sprintf(cnt, "CONNECTION %d:",ccount);
write(fd4,cnt,strlen(cnt)*sizeof(char));
write(fd4,pRooms[b],strlen(pRooms[b])*sizeof(char));
write(fd4,nl,strlen(nl)*sizeof(char));
}
}

write(fd4,mid,strlen(mid)*sizeof(char));
write(fd4,nl,strlen(nl)*sizeof(char));




//fd5
write(fd5,r_n, strlen(r_n)*sizeof(char));
write(fd5,pRooms[5],strlen(pRooms[5])*sizeof(char));
write(fd5,nl,strlen(nl)*sizeof(char));
ccount =0;
for(b=0;b<7;b++)
{
if(obc[5][b]==1)
{
ccount++;
sprintf(cnt, "CONNECTION %d:",ccount);
write(fd5,cnt,strlen(cnt)*sizeof(char));
write(fd5,pRooms[b],strlen(pRooms[b])*sizeof(char));
write(fd5,nl,strlen(nl)*sizeof(char));
}
}

write(fd5,mid,strlen(mid)*sizeof(char));
write(fd5,nl,strlen(nl)*sizeof(char));


//fd6
write(fd6,r_n, strlen(r_n)*sizeof(char));
write(fd6,pRooms[6],strlen(pRooms[6])*sizeof(char));
write(fd6,nl,strlen(nl)*sizeof(char));
ccount =0;
for(b=0;b<7;b++)
{
if(obc[6][b]==1)
{
ccount++;
sprintf(cnt, "CONNECTION %d:",ccount);
write(fd6,cnt,strlen(cnt)*sizeof(char));
write(fd6,pRooms[b],strlen(pRooms[b])*sizeof(char));
write(fd6,nl,strlen(nl)*sizeof(char));
}
}

write(fd6,end,strlen(mid)*sizeof(char));			//write room type: end room because it's the last room
write(fd6,nl,strlen(nl)*sizeof(char));







return 0;

}














