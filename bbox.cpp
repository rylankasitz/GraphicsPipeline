#include "stdafx.h"

#include "bbox.h"

BBox::BBox() {
	SetBBox(Vector::ZERO, Vector::ZERO);
}

void BBox::SetBBox(Vector _min, Vector _max) {
	Min = _min;
	Max = _max;

	Size = (Max - Min).Length();
}

