#include "stdafx.h"
#include "cubemap.h"
#include "framebuffer.h"
#include "ppc.h"
#include "vector.h"
#include "worldview.h"
#include "resourcemanager.h"

CubeMap::CubeMap() 
{
	lastlookup = 0;
}

void CubeMap::Load(Texture cubemap)
{

	int w = cubemap.GetWidth() / 4;
	int h = cubemap.GetHeight() / 3;
	int m = 0;

	setPPC(w, h);

	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if ((i == 0 && j == 0) || (i == 2 && j == 0) || (i == 3 && j == 0) ||
				(i == 0 && j == 2) || (i == 2 && j == 2) || (i == 3 && j == 2))
				continue;

			textures[m] = new Texture(w, h);

			for (int u = 0; u < w; u++)
			{
				for (int v = 0; v < h; v++)
				{
					unsigned int color;
					if (cubemap.fb->Get(u + i * w, v + j * h, color))
					{
						textures[m]->fb->Set(u, v, color);
					}
				}
			}

			m++;
		}
	}
}

Vector CubeMap::Lookup(Vector ruv)
{

	bool first = false;
	for (int i = lastlookup; (i != lastlookup) || !first; i = (i + 1) % 6)
	{
		first = true;

		Vector pp;
		if (!ppcs[i]->Project(ruv, pp))
			continue;

		Vector c;
		if (!textures[i]->FindPixColorByPixel(pp[0], pp[1], c))
			continue;

		lastlookup = i;
		return c;
	}

	Vector c;
	c.SetFromColor(CR_RED);
	return c;
}

void CubeMap::setPPC(int w, int h)
{
	ppcs[0] = new PPC(90.0f, w, h);
	ppcs[0]->SetPose(Vector::ZERO, Vector::YAXIS * 10.0f, Vector::ZAXIS);

	ppcs[1] = new PPC(90.0f, w, h);
	ppcs[1]->SetPose(Vector::ZERO, Vector::XAXIS * -10.0f, Vector::YAXIS);

	ppcs[2] = new PPC(90.0f, w, h);
	ppcs[2]->SetPose(Vector::ZERO, Vector::ZAXIS * -10.0f, Vector::YAXIS);

	ppcs[3] = new PPC(90.0f, w, h);
	ppcs[3]->SetPose(Vector::ZERO, Vector::XAXIS * 10.0f, Vector::YAXIS);

	ppcs[4] = new PPC(90.0f, w, h);
	ppcs[4]->SetPose(Vector::ZERO, Vector::ZAXIS * 10.0f, Vector::YAXIS);

	ppcs[5] = new PPC(90.0f, w, h);
	ppcs[5]->SetPose(Vector::ZERO, Vector::YAXIS * -10.0f, Vector::ZAXIS * -1.0f);
}