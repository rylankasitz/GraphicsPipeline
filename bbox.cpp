#include "stdafx.h"

#include "bbox.h"

BBox::BBox(Vector point) {
	Corners[0] = Corners[1] = point;
}

void BBox::AddPoint(Vector point) {

	for (int i = 0; i < 3; i++) {
		if (Corners[0][i] > point[i])
			Corners[0][i] = point[i];
		if (Corners[1][i] < point[i])
			Corners[1][i] = point[i];
	}

	Size = (Corners[0] - Corners[1]).Length();
}

bool BBox::ClipWithFrame(int w, int h) {

	Corners[0][0] = clamp(Corners[0][0], 0, w);
	Corners[1][0] = clamp(Corners[1][0], 0, w);
	Corners[0][1] = clamp(Corners[0][1], 0, h);
	Corners[1][1] = clamp(Corners[1][1], 0, h);

	return (Corners[0] - Corners[1]).Length() != 0;
}

