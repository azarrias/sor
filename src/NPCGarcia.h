#ifndef __NPCGarcia_H__
#define __NPCGarcia_H__

#include "NPC.h"

class NPCGarcia : public NPC {
public:
	NPCGarcia();
	~NPCGarcia();
	bool Start();
	bool CleanUp();
};

#endif // __NPCGarcia_H__