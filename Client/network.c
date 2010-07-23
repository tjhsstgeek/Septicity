#include "rts.h"
#include <SDL/SDL_net.h>

TCPsocket serversocket;

void tcp_has_connected() {
	//Required to have client wait until server has successfully approved client
}
char *last_map = 0;
void check_for_map() {
	unsigned char len;
	SDLNet_TCP_Recv(serversocket,&len,1);
	char *file = malloc(len+1);
	file[len] = 0;
	SDLNet_TCP_Recv(serversocket,file,len);
	int magic;
	SDLNet_TCP_Recv(serversocket,&magic,4);
	FILE *test = fopen(file,"r");
	last_map = file;
	if (!test) {
		SDLNet_TCP_Send(serversocket,"\1",1);//File does not exist
		return;
	}
	if (fgetc(test) || fgetc(test) != 13 || fgetc(test) != 1 || fgetc(test) != 16) {//Incorrect header
		fclose(test);
		SDLNet_TCP_Send(serversocket,"\2",1);//Incorrect Header
		return;
	}
	int fmagic;
	fread(&fmagic,4,1,test);
	fclose(test);
	if (fmagic != magic) {
		SDLNet_TCP_Send(serversocket,"\3",1);//Incorrect Magic
		return;
	}
	last_map = 0;
	SDLNet_TCP_Send(serversocket,"\0",1);//File exists and is ok
}
char *last_unit = 0;
void check_for_unit() {
	unsigned char len;
	SDLNet_TCP_Recv(serversocket,&len,1);
	char *file = malloc(len+1);
	file[len] = 0;
	SDLNet_TCP_Recv(serversocket,file,len);
	//Units don't have a magic
	FILE *test = fopen(file,"r");
	last_unit = file;
	if (!test) {
		SDLNet_TCP_Send(serversocket,"\1",1);//Unit does not exist
		return;
	}
	int header;
	fread(&header,4,1,test);
	fclose(test);
	if (header != 0x33504449) {//Incorrect header
		SDLNet_TCP_Send(serversocket,"\2",1);//Incorrect Header
		return;
	}
	last_unit = 0;
	SDLNet_TCP_Send(serversocket,"\0",1);//File exists and is ok
}
void save_map_file() {
	int len;
	SDLNet_TCP_Recv(serversocket,&len,4);
	char *file = malloc(len);//Hopefully it isn't too big
	SDLNet_TCP_Recv(serversocket,file,len);
	FILE *map = fopen(last_map,"w");
	fwrite(file,len,1,map);
	fclose(map);
	last_map = 0;
}
void save_unit_file() {
	int len;
	SDLNet_TCP_Recv(serversocket,&len,4);
	char *file = malloc(len);//Hopefully it isn't too big
	SDLNet_TCP_Recv(serversocket,file,len);
	FILE *unit = fopen(last_unit,"w");
	fwrite(file,len,1,unit);
	fclose(unit);
	last_unit = 0;
}
void tcp_failed_connection() {
	unsigned char len;
	SDLNet_TCP_Recv(serversocket,&len,1);
	char *reason = malloc(len+1);
	reason[len] = 0;
	SDLNet_TCP_Recv(serversocket,reason,len);
	fprintf(stderr,"%s\n",reason);
}
extern void load_map(char *);//put in rts.h
char *cur_map = 0;
void switch_mode() {
	load_map(cur_map);
	SDLNet_TCP_Send(serversocket,"\1",1);//Done
}
extern void unpause_game();//put in rts.h
void start_game() {
	unpause_game();
}
extern void show_lost_screen();//put in rts.h
void lost_the_game() {
	show_lost_screen();
}
extern void show_won_screen();//put in rts.h
void won_the_game() {
	show_won_screen();
}
extern void set_number_of_player(char);
void set_player_number() {
	char num;
	SDLNet_TCP_Recv(serversocket,&num,1);
	set_number_of_player(num);
}
extern void set_slot(char,char *);
void person_joined() {
	char num;
	SDLNet_TCP_Recv(serversocket,&num,1);
	unsigned char len;
	SDLNet_TCP_Recv(serversocket,&len,1);
	char *name = malloc(len+1);
	name[len] = 0;
	SDLNet_TCP_Recv(serversocket,name,len);
	set_slot(num,name);
}
extern void clear_slot(char);
void person_left() {
	char num;
	SDLNet_TCP_Recv(serversocket,&num,1);
	clear_slot(num);
}
extern void move_slot(char,char);
void person_changed_slot() {
	char start;
	SDLNet_TCP_Recv(serversocket,&start,1);
	char end;
	SDLNet_TCP_Recv(serversocket,&end,1);
	move_slot(start,end);
}

void set_current_map() {
	if (cur_map) {
		free(cur_map);
		cur_map = 0;
	}
	unsigned char len;
	SDLNet_TCP_Recv(serversocket,&len,1);
	char *name = malloc(len+1);
	name[len] = 0;
	SDLNet_TCP_Recv(serversocket,name,len);
	cur_map = name;
}
extern void show_host_commands();
void set_as_host() {
	show_host_commands();
}
static void (*response [])() = {
	tcp_has_connected,check_for_map,check_for_unit,save_map_file,save_unit_file,tcp_failed_connection,switch_mode,start_game,
	lost_the_game,won_the_game,set_player_number,set_as_host
};

int connect_to_server(char *ipaddress) {
	IPaddress ip;
	if (SDLNet_ResolveHost(&ip,ipaddress,4937)) {
		fprintf(stderr,"Cannot resolve ip address. %s\n",SDLNet_GetError());
		return 1;
	}
	serversocket = SDLNet_TCP_Open(&ip);
	if (!serversocket) {
		fprintf(stderr,"Cannot open socket. %s\n",SDLNet_GetError());
		return 1;
	}
	return 0;
}
void listen_for_data() {
	while (1) {
		char command;
		int how = SDLNet_TCP_Recv(serversocket,&command,1);
		if (how <= 0) {//Should be -1,0,1
			break;
		}
		response[(unsigned char)command]();
	}
}

