#include "stdafx.h"
#include "envmap.h"
#include "vector.h"
#include "worldview.h"
#include "ppc.h"

EnvMap::EnvMap(CubeMap _cubemap)
{
	cubemap = _cubemap;
}

void EnvMap::Render(WorldView* wv)
{

	for (int u = 0; u < wv->fb->w; u++)
	{
		for (int v = 0; v < wv->fb->h; v++)
		{
			Vector P = wv->ppc->UnProject(Vector(u, v, 1));
			wv->fb->Set(u, v, cubemap.Lookup(P - wv->ppc->C).GetColor());
		}
	}
}