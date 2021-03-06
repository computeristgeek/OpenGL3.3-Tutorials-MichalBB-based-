// Header files of frequent usage
#include <cstdio>
#include <iostream>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <sstream>
#include <queue>
#include <map>
#include <set>

using namespace std;

#include <GL/glew.h>
#include <glm/glm.hpp>

// Some useful defines

#define FOR(q,n) for(GLint q=0;q<n;q++)
#define SFOR(q,s,e) for(GLint q=s;q<=e;q++)
#define RFOR(q,n) for(GLint q=n;q>=0;q--)
#define RSFOR(q,s,e) for(GLint q=s;q>=e;q--)

#define ESZ(elem) (int)elem.size()

#define MessageBox(hWnd,msg,msgTitle,icon) cerr<<msgTitle<<":"<<msg<<endl;
