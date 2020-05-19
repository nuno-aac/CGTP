#ifndef __XMLPARSE_H__
#define __XMLPARSE_H__

#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "headers/tinyxml2.h"
#include "scene.h"

using namespace tinyxml2;

vector<Group*> parseXML(string nome);


#endif