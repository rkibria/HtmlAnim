/*
HtmlAnim - A C++ header-only library for creating HTML/JavaScript animations

https://github.com/rkibria/HtmlAnim

MIT License

Copyright (c) 2019 Raihan Kibria

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once

#include <htmlanim.hpp>

namespace HtmlAnimShapes {

using namespace HtmlAnim;

class RegularPolygon : public Drawable {
	CoordExpressionValue x, y, r, edges;
	BoolExpressionValue fill;
public:
	explicit RegularPolygon(const CoordExpressionValue& x, const CoordExpressionValue& y,
		const CoordExpressionValue& r, const CoordExpressionValue& edges, const BoolExpressionValue& fill)
		: x{ x }, y{ y }, r{ r }, edges{ edges }, fill{ fill } {}
	virtual void define(DefinitionsStream &ds) const override {
		ds.write_if_undefined(typeid(RegularPolygon).hash_code(), R"(
function regular_polygon(ctx, x, y, r, edges, fill) {
	edges = Math.trunc(edges);
	ctx.beginPath();
	for(let i = 0; i < edges; ++i) {
		const phi = 2 * Math.PI / edges * i;
		px = x + r * Math.cos(phi);
		py = y + r * Math.sin(phi);
		if(i == 0)
			ctx.moveTo(px, py);
		else
			ctx.lineTo(px, py);
	}
	ctx.closePath();
	if(fill)
		ctx.fill();
	else
		ctx.stroke();
}
)");
	}
	virtual void draw(std::ostream& os) const override {
		os << "regular_polygon(ctx, " << x.to_string() << ", " << y.to_string()
			<< ", " << r.to_string() << ", " << edges.to_string() << ", " << fill.to_string() << ");\n";
	}
};

class Grid : public Drawable {
protected:
	CoordType x, y, dx, dy;
	SizeType nx, ny;
public:
	explicit Grid(CoordType x, CoordType y, CoordType dx, CoordType dy, SizeType nx, SizeType ny)
		: x{x}, y{y}, dx{dx}, dy{dy}, nx{nx}, ny{ny} {}
	virtual void define(DefinitionsStream &ds) const override {
		ds.write_if_undefined(typeid(Grid).hash_code(), R"(
function grid(ctx, x, y, dx, dy, nx, ny) {
	const max_y = ny * dy;
	for(let ix = 0; ix < nx + 1; ++ix) {
		ctx.beginPath();
		const lx = x + ix * dx;
		ctx.moveTo(lx, y);
		ctx.lineTo(lx, y + max_y);
		ctx.closePath();
		ctx.stroke();
	}
	const max_x = nx * dx;
	for(let iy = 0; iy < ny + 1; ++iy) {
		ctx.beginPath();
		const ly = y + iy * dy;
		ctx.moveTo(x, ly);
		ctx.lineTo(x + max_x, ly);
		ctx.closePath();
		ctx.stroke();
	}
}
)");
	}
	virtual void draw(std::ostream& os) const override {
		os << "grid(ctx, " << static_cast<int>(x) << ", " << static_cast<int>(y)
			<< ", " << static_cast<int>(dx) << ", " << static_cast<int>(dy)
			<< ", " << nx << ", " << ny
			<< ");\n";
	}
};

class SubdividedGrid : public Grid {
	SizeType sx, sy;
	std::string bgstyle, fgstyle;
public:
	explicit SubdividedGrid(CoordType x, CoordType y, CoordType dx, CoordType dy, SizeType nx, SizeType ny,
		SizeType sx, SizeType sy, const std::string& bgstyle, const std::string& fgstyle)
		: Grid(x, y, dx, dy, nx, ny), sx{sx}, sy{sy}, bgstyle{bgstyle}, fgstyle{fgstyle} {}
	virtual void define(DefinitionsStream &ds) const override {
		Grid::define(ds);
		ds.write_if_undefined(typeid(SubdividedGrid).hash_code(), R"(
function subdivided_grid(ctx, x, y, dx, dy, nx, ny, sx, sy, bgstyle, fgstyle) {
	ctx.save();
	ctx.strokeStyle = bgstyle;
	grid(ctx, x, y, dx / sx, dy / sy, nx * sx, ny * sy);
	ctx.strokeStyle = fgstyle;
	grid(ctx, x, y, dx, dy, nx, ny);
	ctx.restore();
}
)");
	}
	virtual void draw(std::ostream& os) const override {
		os << "subdivided_grid(ctx, " << static_cast<int>(x) << ", " << static_cast<int>(y)
			<< ", " << static_cast<int>(dx) << ", " << static_cast<int>(dy)
			<< ", " << nx << ", " << ny
			<< ", " << sx << ", " << sy
			<< ", \"" << bgstyle << "\", \"" << fgstyle << "\""
			<< ");\n";
	}
};


/// Draw a nx-by-ny grid starting at x,y of dx,dy pixels size
auto grid(CoordType x, CoordType y, CoordType dx, CoordType dy, SizeType nx, SizeType ny) {
	return std::make_unique<Grid>(x, y, dx, dy, nx, ny);};

auto regular_polygon(const CoordExpressionValue& x, const CoordExpressionValue& y,
	const CoordExpressionValue& r, const CoordExpressionValue& edges, const BoolExpressionValue& fill=false) {
	return std::make_unique<RegularPolygon>(x, y, r, edges, fill);};

/// Draw a nx-by-ny grid starting at x,y of dx,dy pixels size with each block subdivided into sx/sy elements
auto subdivided_grid(CoordType x, CoordType y, CoordType dx, CoordType dy, SizeType nx, SizeType ny,
	SizeType sx, SizeType sy, const std::string& bgstyle = "silver", const std::string& fgstyle = "gray") {
	return std::make_unique<SubdividedGrid>(x, y, dx, dy, nx, ny, sx, sy, bgstyle, fgstyle);};

} // namespace HtmlAnimShapes
