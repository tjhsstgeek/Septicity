#include <stdio.h>

void load_map(char *name) {
	printf("Loading %s ...\n",name);
}
void unpause_game() {
	printf("Unpaused\n");
}
void show_lost_screen() {
	printf("Lost\n");
}
void show_won_screen() {
	printf("Win\n");
}
void set_number_of_player(char c) {
	printf("Your number is %i\n",c);
}
void set_slot(char num,char *name) {
	printf("%i set to %s\n",num,name);
}
void clear_slot(char c) {
	printf("No one in %i\n",c);
}
void move_slot(char s,char e) {
	printf("Moving person in %i to %i\n",s,e);
}
void show_host_commands() {
	printf("Adding Host commands to normal gui\n");
}
