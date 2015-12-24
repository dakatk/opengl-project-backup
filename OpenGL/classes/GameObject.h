#ifndef GAME_OBJECT_H__
#define GAME_OBJECT_H__

#include "../headers/Types.h"

extern GameObject createObject(const char* texFile, Vertex* vertices, int* indices, int vertAmt, int indAmt, int posAmt, unsigned int shadeProg);
extern void moveObject(GameObject* o, int index, float dx, float dy, float dz);
extern void moveObjectAll(GameObject* o, float dx, float dy, float dz);
extern void deleteObjects(GameObject* o, int numObjects);

#endif
