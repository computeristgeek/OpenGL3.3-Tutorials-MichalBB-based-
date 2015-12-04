#pragma once

glm::vec4 GetColorByIndex(GLint index);
GLint GetIndexByColor(GLint r, GLint g, GLint b);
GLint GetPickedColorIndexUnderMouse();

GLvoid Get3DRayUnderMouse(glm::vec3* v1, glm::vec3* v2);
GLboolean RaySphereCollision(glm::vec3 vSphereCenter, GLfloat fSphereRadius, glm::vec3 vA, glm::vec3 vB);