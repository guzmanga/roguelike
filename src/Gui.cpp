#include <stdio.h>
#include <stdarg.h>
#include "main.hpp"

static const int PANEL_HEIGHT = 7;
static const int BAR_WIDTH = 20;
static const int MSG_X=BAR_WIDTH+2;
static const int MSG_HEIGHT=PANEL_HEIGHT-1;

Gui::Gui(){
	con = new TCODConsole(engine.screenWidth, PANEL_HEIGHT);
}

Gui::~Gui(){
	delete con;
	log.clearAndDelete();
}



void Gui::render(){
	con->setDefaultBackground(TCODColor::black);
	con->clear();

	renderBar(1,1,BAR_WIDTH, "HP", engine.player->destructible->hp,
		engine.player->destructible->maxHp,
		TCODColor::lightRed, TCODColor::darkerRed);

	//draw message log
	int y = 1;
	float colorCoef=0.4f;
	for(Message **it = log.begin(); it != log.end(); it++){
		Message *message = *it;
		con->setDefaultForeground(message->col * colorCoef);
		con->setDefaultForeground(message->col);
		con->print(MSG_X, y, message->text);
		y++;
		if(colorCoef < 1.0f){
			colorCoef+=0.3f;
		}
	}
}

void Gui::renderBar(int x, int y, int width, const char *name,
	float value, float maxValue, const TCODColor &barColor, 
	const TCODColor &backColor){
	//fill the background

	con->setDefaultBackground(backColor);
	con->rect(x,y,width,1,false,TCOD_BKGND_SET);

	//compute how much of the bar should be filled with bar color

	int barWidth = (int)(value / maxValue * width);
	if(barWidth > 0){
		//draw the bar
		con->setDefaultBackground(barColor);
		con->rect(x,y,barWidth, 1, false, TCOD_BKGND_SET);
	}
	//print text on top of bar
	con->setDefaultForeground(TCODColor::white);
	con->printEx(x+width/2, y, TCOD_BKGND_NONE, TCOD_CENTER,
		"%s : %g/%g", name, value, maxValue);
}

//Gui::Message constructors and destructors
Gui::Message::Message(const char *text, const TCODColor &col) : text(strdup(text)), col(col){
}

Gui::Message::~Message(){
	free(text);
}

void Gui::message(const TCODColor &col, const char *text, ...){
	//build the text
	va_list ap;
	char buf [128];
	va_start(ap, text);
	vsprintf(buf, text, ap);
	va_end(ap);

	char *lineBegin=buf;
	char *lineEnd;

	//before writing a new line in the log, check that there is room; else remove oldest mesage:
	do{
		//make room for new message
		if(log.size() == MSG_HEIGHT){
			Message *toRemove=log.get(0);
			log.remove(toRemove);
			delete toRemove;
		}
		//detect end of the line
		lineEnd = strchr(lineBegin, '\n');
		if(lineEnd){
			*lineEnd='\0';
		}
		Message *msg = new Message(lineBegin, col);
		log.push(msg);
		//go to next line
		lineBegin = lineEnd + 1;
	}while(lineEnd); //ends when (or if) no \n is found
}