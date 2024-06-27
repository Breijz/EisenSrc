#ifndef GL_RSURF_H
#define GL_RSURF_H

#include "quakedef.h"

// Dummy Linking file for now
// TODO : Remove
void R_DrawSkyChain(msurface_t *s);

qboolean R_CullBox(vec3_t mins, vec3_t maxs);

void R_MarkLights(dlight_t *light, int bit, mnode_t *node);

void R_RotateForEntity(entity_t *e);

void R_StoreEfrags(efrag_t **ppefrag);

void R_DrawBrushModel(entity_t *e);

void R_AnimateLight();

void V_CalcBlend();

void R_DrawWorld();

void R_RenderDlights();

void R_DrawParticles(void);

void R_DrawWaterSurfaces(void);

void R_RenderBrushPoly(msurface_t *fa);

#endif
