#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<time.h>
#include<dirent.h>
#include<fcntl.h>

struct rooms
{
	char roomname[9];
	char conts[6][9];
	char rtype[15];	
	int numRoom;

};

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;		//have a myMutex


//to save room names and room type as a struct
void getFileStruct(FILE *fp, struct rooms arr_rooms[], int x)
{
	char roomnames[10][8];				//save names into roomname array
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
	char roomtypes[3][12];					//save room types into roomtype array
	strcpy(roomtypes[0],"START ROOM");
	strcpy(roomtypes[1],"MID ROOM");
	strcpy(roomtypes[2],"END ROOM");
	
	char temptype[12];
	char *typeret;
	char tempname[15];
	char *check;
	char *line=NULL;
	size_t size=0;
	ssize_t ret;
	char *nameret;
	int j=0;
	int i=0;
	
	
	memset(arr_rooms[x].conts,'\0',sizeof(arr_rooms[x].conts));			//empty struct array contections
	arr_rooms[x].numRoom =0;							//too count number of connections
	while((ret = getline(&line, &size, fp))!=-1)					//until finish to read each line in the file
	{
	
		i=0;	
		while(1)
		{
			
			if(check=strstr(line,"ROOM NAME"))				//if the line contains "room name"
			{
				nameret=strstr(line, roomnames[i]);			//get the room name comparing 
					if(nameret)                                     // with room names in room name array
					{
						memset(tempname, '\0', sizeof(tempname));	//empty tempname
						memset(arr_rooms[x].roomname,'\0',sizeof(arr_rooms[x].roomname)); //empty struct arry.name
						strcpy(tempname, roomnames[i]);			//copy the machted name into tempname
						strcpy(arr_rooms[x].roomname, tempname);	//copy the tempname into arr.roomname
						break;
					}
					else
						i++;			//if it's not matched then move to next name array


			}
			else if(check=strstr(line,"CONNECTION")) 		//if the line contains room type
			{	
				nameret = strstr(line, roomnames[i]);		//get the roomnames campring to names in name array
				if(nameret)					//if the name machted
				{
					memset(tempname, '\0', sizeof(tempname));	//empty tempname
					//memset(arr_rooms[x].conts[j],'\0',sizeof(arr_rooms[x].conts[j]));
					strcpy(tempname, roomnames[i]);			//copy the name into tempname
					strcpy(arr_rooms[x].conts[j], tempname);	//copy the name into arr.connections
					arr_rooms[x].numRoom++;				//increment number of connections
					j++;
					break;
				}


				else
				{
					i++;				//else move to next name array
				}
			}
			else if(check=strstr(line,"ROOM TYPE"))		//if the line contains room type
			{
				
				typeret = strstr(line, roomtypes[i]);		//get the roomn type comparing to room type char
				if(typeret)					//if the type matched
				{
					memset(arr_rooms[x].rtype,'\0',sizeof(arr_rooms[x].rtype));	//empty arr.roomtype
				
					memset(temptype, '\0', sizeof(temptype));			//empty temptype
					strcpy(temptype, roomtypes[i]);					//copy the type into temptype
					strcpy(arr_rooms[x].rtype, temptype);		//copy the type into arr.roomtype
					break;
				}
				else
				{
					i++;				//else move to next room type
			
				}
				}
			


		}
	
	}
	free(line);

}


void* timeThread()
{

	pthread_mutex_lock(&myMutex);			//lock on a mutex
	//https://stackoverflow.com/questions/25420933/c-create-txt-file-and-append-current-date-and-time-as-a-name
	//use from the website
	char timetext[50];			//buffer to save time
	time_t now1 = time(NULL);
	struct tm *t = localtime(&now1);	//get current time
	strftime(timetext, sizeof(timetext)-1, "%l:%M%P, %A, %B %d, %G", t);	//save time into timetext using strftime
	timetext[49] = 0;
	

	FILE *ftime = fopen("currentTime.txt", "w+");	//make currentTime.text file
	fprintf(ftime, "%s", timetext);		//print timetext to the file
	fclose(ftime);				//close the file
	pthread_mutex_unlock(&myMutex);		//unlock on a mutex
	

}

int main()
{
	//////////////
	pthread_mutex_lock(&myMutex);				//lock mutex
	pthread_t th_id;
	pthread_create(&th_id, NULL, timeThread, NULL);			//create thread
//////////

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

	struct rooms arr_rooms[7];

	//from canvas 
	int newestDirtime = -1;				//modified timestamp of newest subdir examined
	char targetDir[30] = "choha.rooms.";		//prefix that i looking for

	char newestDirName[60];				//buffer for newest dir name at that time
	memset(newestDirName, '\0',sizeof(newestDirName));		//empty the buffer

	DIR* dirCheck;				//hold starting directory
	struct dirent *fileInDir;		//hold the current subdir of the starting dir
	struct stat status;			//hold information we gained about subdir

	dirCheck= opendir(".");			//open up the dir this program was run in 

	if(dirCheck > 0)			//make sure the current dir could be opened
	{
		while((fileInDir = readdir(dirCheck)) != NULL)	//check each entry in dir
		{
			if(strstr(fileInDir->d_name, targetDir) != NULL)	//if the dirname include the prefix
			{
				stat(fileInDir->d_name, &status);		//get attribute of entry

				if((int)status.st_mtime > newestDirtime)		//if this time is bigger
				{
					newestDirtime = (int)status.st_mtime;		//get new newestdirtime
					memset(newestDirName, '\0', sizeof(newestDirName));	//empty newestDirname
					strcpy(newestDirName, fileInDir->d_name);		//copy the name into newestDirname
				}
			}
		}
	}

	closedir(dirCheck);	//close directory


	char pathfile[80];				//buffer to save path of file
	sprintf(pathfile, "%s/room0",newestDirName);	//save newestDirname that i got with room file
	FILE *fd0 = fopen(pathfile, "r");		//make file named with the path. only read

	sprintf(pathfile, "%s/room1",newestDirName);	//same with fd0
	FILE* fd1 = fopen(pathfile, "r");

	sprintf(pathfile, "%s/room2",newestDirName);
	FILE* fd2 = fopen(pathfile, "r");


	sprintf(pathfile, "%s/room3",newestDirName);
	FILE* fd3 = fopen(pathfile, "r");


	sprintf(pathfile, "%s/room4",newestDirName);
	FILE* fd4 = fopen(pathfile, "r");


	sprintf(pathfile, "%s/room5",newestDirName);
	FILE* fd5 = fopen(pathfile, "r");


	sprintf(pathfile, "%s/room6",newestDirName);
	FILE* fd6 = fopen(pathfile, "r");


	
	



	getFileStruct(fd0,arr_rooms, 0);	//save fd0's information into arr_rroms[0]
	getFileStruct(fd1,arr_rooms, 1);	

	getFileStruct(fd2,arr_rooms, 2);
	getFileStruct(fd3,arr_rooms, 3);
	getFileStruct(fd4,arr_rooms, 4);
	getFileStruct(fd5,arr_rooms, 5);
	getFileStruct(fd6,arr_rooms, 6);       //same until here

	
	struct rooms *ptr;			//pointer to struct rooms
	char fromuser[50];
	
	int count=0;
	char patharr[100][9];
	int i;
	int idx;

	ptr = &arr_rooms[0];
	
while(1)
{
	if(strcmp(ptr->rtype, "END ROOM")==0)			//if roomtype is endroom
	{
		printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");		//let user know that user complete the game
		printf("YOU TOOK %d STEPS. YOU PATH TO VICTORY WAS: \n", count);
		for(i=0; i<count; i++)
		{
			printf("%s\n", patharr[i]);			//show the path the user took
		}
		break;
	}
	else
	{
		printf("CURRENT LOCATION: %s\n", ptr->roomname);	//if the room type is not end room
		printf("POSSIBLE CONNECTIONS: ");			//print current room name and connections
		for(i=0; i<(ptr->numRoom)-1; i++)
		{
			printf("%s, ", ptr->conts[i]);			//print with ,
			
		}
		printf("%s.", ptr->conts[(ptr->numRoom)-1]);		//print last connections with .
		printf("\n");
		printf("WHERE TO? >");					//prompt line 
		scanf("%s", &fromuser);					//get user's answer
		printf("\n");
		
		if(strcmp(fromuser, "time")==0)					//if user enter time
		{
			char buffer[50];
			memset(buffer, '\0', sizeof(buffer));			//empty buffer
			pthread_mutex_unlock(&myMutex);				//unlock mutex
			pthread_join(th_id, NULL);				//join
			pthread_mutex_lock(&myMutex);				// lock mutex
			pthread_create(&th_id, NULL, timeThread, NULL);		//create thread because terminated when joined
			FILE *ftime = fopen("currentTime.txt", "r");		//open file named curretTime.txt readable
			fseek(ftime, 0, SEEK_SET);				//set cursor to at the beginning of the file
			fread(buffer, sizeof(buffer), 1, ftime);		//read the file into buffer
			printf("%s\n", buffer);					//print the file
			printf("\n");

		}
		else{								//if user enter other than time
		i=0;
		while(i<ptr->numRoom)
		{
			if(strcmp(fromuser, ptr->conts[i])!=0)			//compare userinput to connetions the room has
			{
				idx = 0;					//not matched then go to next connection
				i++;

			}
			else if(strcmp(fromuser, ptr->conts[i])==0)		//if the user input is matched with connection
			{
				idx =1;						//index =1
				break;
			}
		
		}
		if(idx == 0)				//if user input is not found in the connetioncs
		{
			
			printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n");	//print it
			printf("\n");
		}
		else if(idx ==1)				//if user input matched with one of connections
		{
			for(i=0; i<7; i++)
			{
				if(strcmp(arr_rooms[i].roomname,fromuser)==0)		//found the room 
				{
					strcpy(patharr[count], fromuser);	//save userinput in path array
					count++;				//implement count to trace number of input
					ptr = &arr_rooms[i];			//point to new struct
				}
			}
		}
		}

	}

}


return 0;


}
