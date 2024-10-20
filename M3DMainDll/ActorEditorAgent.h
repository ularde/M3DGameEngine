#pragma once
#include "Class.h"
#include "Global.h"
#include <glm/glm.hpp>
#include <math.h>

class MUnscriptableObject;
class MBasicPlatform;
class MActor;

#define M_PI 3.1415926535

static void fghDrawGeometrySolid10(GLfloat* varr, GLfloat* narr, GLfloat* tarr,
    GLsizei nverts, GLushort* iarr, GLsizei nparts, GLsizei npartverts)
{
    int i, j;
    GLfloat* vptr = 0, * nptr = 0, * tptr = 0;
    GLushort* iptr = 0;

    if (!iarr) {
        vptr = varr;
        nptr = narr;
        tptr = tarr;
        glBegin(GL_TRIANGLES);
        for (i = 0; i < nverts; i++) {
            if (tarr) {
                glTexCoord2fv(tptr); tptr += 2;
            }
            glNormal3fv(nptr); nptr += 3;
            glVertex3fv(vptr); vptr += 3;
        }
        glEnd();
        return;
    }

    iptr = iarr;
    if (nparts > 1) {
        for (i = 0; i < nparts; i++) {
            glBegin(GL_TRIANGLE_STRIP);
            for (j = 0; j < npartverts; j++) {
                int idx = *iptr++;
                if (tarr) {
                    glTexCoord2fv(tarr + idx * 2);
                }
                idx = idx * 2 + idx;
                glNormal3fv(narr + idx);
                glVertex3fv(varr + idx);
            }
            glEnd();
        }
    }
    else {
        glBegin(GL_TRIANGLES);
        for (i = 0; i < npartverts; i++) {
            int idx = *iptr++;
            if (tarr) {
                glTexCoord2fv(tarr + idx * 2);
            }
            idx = idx * 2 + idx;
            glNormal3fv(narr + idx);
            glVertex3fv(varr + idx);
        }
        glEnd();
    }
}

static void fghCircleTable(GLfloat** sint, GLfloat** cost, const int n, const GLboolean halfCircle)
{
    int i;

    /* Table size, the sign of n flips the circle direction */
    const int size = abs(n);

    /* Determine the angle between samples */
    const GLfloat angle = (halfCircle ? 1 : 2) * (GLfloat)M_PI / (GLfloat)((n == 0) ? 1 : n);

    /* Allocate memory for n samples, plus duplicate of first entry at the end */
    *sint = (GLfloat*)malloc(sizeof(GLfloat) * (size + 1));
    *cost = (GLfloat*)malloc(sizeof(GLfloat) * (size + 1));

    /* Bail out if memory allocation fails, fgError never returns */
    if (!(*sint) || !(*cost))
    {
        free(*sint);
        free(*cost);
    }

    /* Compute cos and sin around the circle */
    (*sint)[0] = 0.0;
    (*cost)[0] = 1.0;

    for (i = 1; i < size; i++)
    {
        (*sint)[i] = (GLfloat)sin(angle * i);
        (*cost)[i] = (GLfloat)cos(angle * i);
    }


    if (halfCircle)
    {
        (*sint)[size] = 0.0f;  /* sin PI */
        (*cost)[size] = -1.0f;  /* cos PI */
    }
    else
    {
        /* Last sample is duplicate of the first (sin or cos of 2 PI) */
        (*sint)[size] = (*sint)[0];
        (*cost)[size] = (*cost)[0];
    }
}

static void fghGenerateCone(
    GLfloat base, GLfloat height, GLint slices, GLint stacks,   /*  input */
    GLfloat** vertices, GLfloat** normals, int* nVert           /* output */
)
{
    int i, j;
    int idx = 0;    /* idx into vertex/normal buffer */

    /* Pre-computed circle */
    GLfloat* sint = 0, * cost = 0;

    /* Step in z and radius as stacks are drawn. */
    GLfloat z = 0;
    GLfloat r = (GLfloat)base;

    const GLfloat zStep = (GLfloat)height / ((stacks > 0) ? stacks : 1);
    const GLfloat rStep = (GLfloat)base / ((stacks > 0) ? stacks : 1);

    /* Scaling factors for vertex normals */
    const GLfloat cosn = (GLfloat)(height / sqrt(height * height + base * base));
    const GLfloat sinn = (GLfloat)(base / sqrt(height * height + base * base));



    /* number of unique vertices */
    if (slices == 0 || stacks < 1)
    {
        /* nothing to generate */
        *nVert = 0;
        return;
    }
    *nVert = slices * (stacks + 2) + 1;   /* need an extra stack for closing off bottom with correct normals */

    fghCircleTable(&sint, &cost, -slices, GL_FALSE);

    /* Allocate vertex and normal buffers, bail out if memory allocation fails */
    *vertices = (GLfloat*)malloc((*nVert) * 3 * sizeof(GLfloat));
    *normals = (GLfloat*)malloc((*nVert) * 3 * sizeof(GLfloat));
    if (!(*vertices) || !(*normals))
    {
        free(*vertices);
        free(*normals);
    }

    /* bottom */
    (*vertices)[0] = 0.f;
    (*vertices)[1] = 0.f;
    (*vertices)[2] = z;
    (*normals)[0] = 0.f;
    (*normals)[1] = 0.f;
    (*normals)[2] = -1.f;
    idx = 3;
    /* other on bottom (get normals right) */
    for (j = 0; j < slices; j++, idx += 3)
    {
        (*vertices)[idx] = cost[j] * r;
        (*vertices)[idx + 1] = sint[j] * r;
        (*vertices)[idx + 2] = z;
        (*normals)[idx] = 0.f;
        (*normals)[idx + 1] = 0.f;
        (*normals)[idx + 2] = -1.f;
    }

    /* each stack */
    for (i = 0; i < stacks + 1; i++)
    {
        for (j = 0; j < slices; j++, idx += 3)
        {
            (*vertices)[idx] = cost[j] * r;
            (*vertices)[idx + 1] = sint[j] * r;
            (*vertices)[idx + 2] = z;
            (*normals)[idx] = cost[j] * cosn;
            (*normals)[idx + 1] = sint[j] * cosn;
            (*normals)[idx + 2] = sinn;
        }

        z += zStep;
        r -= rStep;
    }

    /* Release sin and cos tables */
    free(sint);
    free(cost);
}

static void fghCone(GLfloat base, GLfloat height, GLint slices, GLint stacks)
{
    int i, j, idx, nVert;
    GLfloat* vertices = 0, * normals = 0;

    /* Generate vertices and normals */
    /* Note, (stacks+1)*slices vertices for side of object, slices+1 for top and bottom closures */
    fghGenerateCone(base, height, slices, stacks, &vertices, &normals, &nVert);

    GLushort* stripIdx;
    /* Create index vector */
    GLushort offset;

    /* Allocate buffers for indices, bail out if memory allocation fails */
    stripIdx = (GLushort*)malloc((slices + 1) * 2 * (stacks + 1) * sizeof(GLushort));    /*stacks +1 because of closing off bottom */
    if (!(stripIdx))
    {
        free(stripIdx);
    }

    /* top stack */
    for (j = 0, idx = 0; j < slices; j++, idx += 2)
    {
        stripIdx[idx] = 0;
        stripIdx[idx + 1] = j + 1;              /* 0 is top vertex, 1 is first for first stack */
    }
    stripIdx[idx] = 0;                    /* repeat first slice's idx for closing off shape */
    stripIdx[idx + 1] = 1;
    idx += 2;

    /* middle stacks: */
    /* Strip indices are relative to first index belonging to strip, NOT relative to first vertex/normal pair in array */
    for (i = 0; i < stacks; i++, idx += 2)
    {
        offset = 1 + (i + 1) * slices;                /* triangle_strip indices start at 1 (0 is top vertex), and we advance one stack down as we go along */
        for (j = 0; j < slices; j++, idx += 2)
        {
            stripIdx[idx] = offset + j;
            stripIdx[idx + 1] = offset + j + slices;
        }
        stripIdx[idx] = offset;               /* repeat first slice's idx for closing off shape */
        stripIdx[idx + 1] = offset + slices;
    }

    /* draw */
    fghDrawGeometrySolid10(vertices, normals, NULL, nVert, stripIdx, stacks + 1, (slices + 1) * 2);

    /* cleanup allocated memory */
    free(stripIdx);

    /* cleanup allocated memory */
    free(vertices);
    free(normals);
}

static void fghGenerateSphere(GLfloat radius, GLint slices, GLint stacks, GLfloat** vertices, GLfloat** normals, int* nVert)
{
    int i, j;
    int idx = 0;    /* idx into vertex/normal buffer */
    GLfloat x, y, z;

    /* Pre-computed circle */
    GLfloat* sint1 = 0, * cost1 = 0;
    GLfloat* sint2 = 0, * cost2 = 0;

    /* number of unique vertices */
    if (slices == 0 || stacks < 2)
    {
        /* nothing to generate */
        *nVert = 0;
        return;
    }
    *nVert = slices * (stacks - 1) + 2;

    /* precompute values on unit circle */
    fghCircleTable(&sint1, &cost1, -slices, GL_FALSE);
    fghCircleTable(&sint2, &cost2, stacks, GL_TRUE);

    /* Allocate vertex and normal buffers, bail out if memory allocation fails */
    *vertices = (GLfloat*)malloc((*nVert) * 3 * sizeof(GLfloat));
    *normals = (GLfloat*)malloc((*nVert) * 3 * sizeof(GLfloat));
    if (!(*vertices) || !(*normals))
    {
        free(*vertices);
        free(*normals);
    }

    /* top */
    (*vertices)[0] = 0.f;
    (*vertices)[1] = 0.f;
    (*vertices)[2] = radius;
    (*normals)[0] = 0.f;
    (*normals)[1] = 0.f;
    (*normals)[2] = 1.f;
    idx = 3;

    /* each stack */
    for (i = 1; i < stacks; i++)
    {
        for (j = 0; j < slices; j++, idx += 3)
        {
            x = cost1[j] * sint2[i];
            y = sint1[j] * sint2[i];
            z = cost2[i];

            (*vertices)[idx] = x * radius;
            (*vertices)[idx + 1] = y * radius;
            (*vertices)[idx + 2] = z * radius;
            (*normals)[idx] = x;
            (*normals)[idx + 1] = y;
            (*normals)[idx + 2] = z;
        }
    }

    /* bottom */
    (*vertices)[idx] = 0.f;
    (*vertices)[idx + 1] = 0.f;
    (*vertices)[idx + 2] = -radius;
    (*normals)[idx] = 0.f;
    (*normals)[idx + 1] = 0.f;
    (*normals)[idx + 2] = -1.f;

    /* Done creating vertices, release sin and cos tables */
    free(sint1);
    free(cost1);
    free(sint2);
    free(cost2);
}

static void fghSphere(GLfloat radius, GLint slices, GLint stacks)
{
    int i = 0, j = 0, idx = 0, nVert = 0;
    GLfloat* vertices, * normals;

    /* Generate vertices and normals */
    fghGenerateSphere(radius, slices, stacks, &vertices, &normals, &nVert);

    if (nVert == 0)
        /* nothing to draw */
        return;

    /* First, generate vertex index arrays for drawing with glDrawElements
     * All stacks, including top and bottom are covered with a triangle
     * strip.
     */
    GLushort* stripIdx;
    /* Create index vector */
    GLushort offset;

    /* Allocate buffers for indices, bail out if memory allocation fails */
    stripIdx = (GLushort*)malloc((slices + 1) * 2 * (stacks) * sizeof(GLushort));
    if (!(stripIdx))
    {
        free(stripIdx);
    }

    /* top stack */
    for (j = 0, idx = 0; j < slices; j++, idx += 2)
    {
        stripIdx[idx] = j + 1;              /* 0 is top vertex, 1 is first for first stack */
        stripIdx[idx + 1] = 0;
    }
    stripIdx[idx] = 1;                    /* repeat first slice's idx for closing off shape */
    stripIdx[idx + 1] = 0;
    idx += 2;

    /* middle stacks: */
    /* Strip indices are relative to first index belonging to strip, NOT relative to first vertex/normal pair in array */
    for (i = 0; i < stacks - 2; i++, idx += 2)
    {
        offset = 1 + i * slices;                    /* triangle_strip indices start at 1 (0 is top vertex), and we advance one stack down as we go along */
        for (j = 0; j < slices; j++, idx += 2)
        {
            stripIdx[idx] = offset + j + slices;
            stripIdx[idx + 1] = offset + j;
        }
        stripIdx[idx] = offset + slices;        /* repeat first slice's idx for closing off shape */
        stripIdx[idx + 1] = offset;
    }

    /* bottom stack */
    offset = 1 + (stacks - 2) * slices;               /* triangle_strip indices start at 1 (0 is top vertex), and we advance one stack down as we go along */
    for (j = 0; j < slices; j++, idx += 2)
    {
        stripIdx[idx] = nVert - 1;              /* zero based index, last element in array (bottom vertex)... */
        stripIdx[idx + 1] = offset + j;
    }
    stripIdx[idx] = nVert - 1;                  /* repeat first slice's idx for closing off shape */
    stripIdx[idx + 1] = offset;


    /* draw */
    fghDrawGeometrySolid10(vertices, normals, NULL, nVert, stripIdx, stacks, (slices + 1) * 2);

    /* cleanup allocated memory */
    free(stripIdx);

    /* cleanup allocated memory */
    free(vertices);
    free(normals);
}

/* -- Cube -- */
#define CUBE_NUM_VERT           8
#define CUBE_NUM_FACES          6
#define CUBE_NUM_EDGE_PER_FACE  4
#define CUBE_VERT_PER_OBJ       (CUBE_NUM_FACES*CUBE_NUM_EDGE_PER_FACE)
#define CUBE_VERT_ELEM_PER_OBJ  (CUBE_VERT_PER_OBJ*3)
#define CUBE_VERT_PER_OBJ_TRI   (CUBE_VERT_PER_OBJ+CUBE_NUM_FACES*2)    /* 2 extra edges per face when drawing quads as triangles */
/* Vertex Coordinates */
static GLfloat cube_v[CUBE_NUM_VERT * 3] =
{
     .5f, .5f, .5f,
    -.5f, .5f, .5f,
    -.5f,-.5f, .5f,
     .5f,-.5f, .5f,
     .5f,-.5f,-.5f,
     .5f, .5f,-.5f,
    -.5f, .5f,-.5f,
    -.5f,-.5f,-.5f
};
/* Normal Vectors */
static GLfloat cube_n[CUBE_NUM_FACES * 3] =
{
     0.0f, 0.0f, 1.0f,
     1.0f, 0.0f, 0.0f,
     0.0f, 1.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
     0.0f,-1.0f, 0.0f,
     0.0f, 0.0f,-1.0f
};

/* Vertex indices, as quads, before triangulation */
static GLubyte cube_vi[CUBE_VERT_PER_OBJ] =
{
    0,1,2,3,
    0,3,4,5,
    0,5,6,1,
    1,6,7,2,
    7,4,3,2,
    4,7,6,5
};

static void fghCube(GLfloat dSize, GLboolean useWireMode)
{
    
}

inline long double my_log(long double n) {
    if (n <= 0) {
        return -log(-n);
    }
    else {
        return log(n);
    }
}

class MActorEditorAgent :public MUnscriptableObject {
public:
	MActorEditorAgent(MBasicPlatform* platform, MActor* parent);
	MActorEditorAgent(MBasicPlatform* platform, MActor* parent, const glm::vec3& pos, float x, float y, float z);
	~MActorEditorAgent();
	glm::vec2 GetPositionWinCoordSys() { return mPositionWinCoordSys; }
	glm::vec2 GetAxisTopPosWinCoordSys(MAxis axis);
	glm::vec3 GetPositionWorldCoordSys() { return mPosition; }
	void DrawSignalElement(MAxis axis, const glm::mat4& dmat);
	void Render();
	bool IsSpecificAxisFocused(MAxis axis);
	bool IsAnyAxisFocused() {
		return IsSpecificAxisFocused(MAxis::X_AXIS) || IsSpecificAxisFocused(MAxis::Y_AXIS) || IsSpecificAxisFocused(MAxis::Z_AXIS);
	}
    void SetPosition(const glm::vec3& pos);
	void FocusAxis(MAxis axis, bool flag);
	void Select(bool flag);
	void Focuse(bool flag);
    void Translate(MAxis axis, double len);
    void Rotate(MAxis axis, double angle);
    void Scale(MAxis axis, double level);
    float GetAxisLength();
    float GetConeHeight();
    float GetConeBase();
    float GetSphereRadius();
	bool IsFocused() { return mFocused; }
	bool IsSelected() { return mSelected; }
protected:
	bool mSelected = false;
	bool mFocused = false;
	bool mXAxisFocused = false, mYAxisFocused = false, mZAxisFocused = false;
	glm::vec2 mPositionWinCoordSys;
	glm::vec2 mXAxisTopPosWinCoordSys, mYAxisTopPosWinCoordSys, mZAxisTopPosWinCoordSys;
	glm::vec3 mPosition;
	float mx = 0.0f, my = 0.0f, mz = 0.0f;
	float ox = 0.0f, oy = 0.0f, oz = 0.0f;
	MActor* mParent = NULL;
	void Initialize();
	void RenderAABB();
	void RenderAxes();
};