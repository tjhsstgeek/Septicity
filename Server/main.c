#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

TCPsocket serversocket;

TCPsocket mastersocket;//The master server which will display all client connected to it

char run = 1;

int connect_to_master() {
	IPaddress ip;
	if (SDLNet_ResolveHost(&ip,"FIXME",4937)) {
		fprintf(stderr,"Cannot resolve Master Server's IP Address. Starting in Local Mode. %s\n",SDLNet_GetError());
		return 1;
	}
	mastersocket = SDLNet_TCP_Open(&ip);
	if (!serversocket) {
		fprintf(stderr,"Master server down or cannot connect to master server. Starting in Local Mode. %s\n",SDLNet_GetError());
		return 1;
	}
	return 0;
}

int init(char public,char *name) {//Port will be 4937
	if (SDL_Init(SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_VIDEO)) {
		fprintf(stderr,"Error initiating SDL. %s\n",SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);
	if (SDLNet_Init()) {
		fprintf(stderr,"Error initiating SDL network library. %s\n",SDLNet_GetError());
		return 1;
	}
	atexit(SDLNet_Quit);
	IPaddress ip;
	if (SDLNet_ResolveHost(&ip,0,4937)) {
		fprintf(stderr,"Cannot resolve ip address. %s\n",SDLNet_GetError());
		return 1;
	}
	serversocket = SDLNet_TCP_Open(&ip);
	if (!serversocket) {
		fprintf(stderr,"Cannot open socket. %s\n",SDLNet_GetError());
		return 1;
	}
	if (public && !connect_to_master()) {//If we manage to connect to the master server
		//Write to server and give name
	}//This goes last because it wouldn't make sense connected if local servers don't work
	return 0;
}
int connection(void *socket) {
	TCPsocket s = socket;
	return 0;
}
int listen_for_connections(void *v) {
	TCPsocket socket;
	while (run) {
		socket = SDLNet_TCP_Accept(serversocket);
		if (socket) {
			SDL_CreateThread(connection,socket);
		}
		SDL_Delay(1000);
	}
	return 0;
}
int main() {
	if (init(1,"locally started server")) 
		return 1;
	SDL_CreateThread(listen_for_connections,0);
	fgets(malloc(4),4,stdin);
	return 0;
}
