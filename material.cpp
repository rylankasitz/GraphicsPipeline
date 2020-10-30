#include "stdafx.h"

#include "material.h"
#include "framebuffer.h"

#include "math.h"

Material Material::Default = Material(0xFF0000FF, false);

Material::Material(unsigned int _color, bool refl) {

	color = Vector::ZERO;
	color.SetFromColor(_color);
	currColor = color;
	pfong = 32;
	texture = Texture::NONE;
	reflection = refl;
}

Material::Material(unsigned int _color, Texture _texture) {

	color = Vector::ZERO;
	color.SetFromColor(_color);
	currColor = color;
	pfong = 32;
	texture = _texture;
}

void Material::SetPix(float x, float y) {

	Vector _color;
	if (!texture.FindPixColor(x, y, _color))
	{
		currColor = color;
	}
	else
	{
		currColor = _color;
	}
}
		