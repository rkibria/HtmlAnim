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

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <unordered_set>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace HtmlAnim {

#ifndef M_PI
constexpr double PI = 3.14159265358979323846;
#else
constexpr double PI = M_PI;
#endif

using CoordType = double;

struct Vec2 {
	CoordType x, y;
	explicit Vec2() : x{0}, y{0} {}
	explicit Vec2(CoordType x, CoordType y) : x{x}, y{y} {}

	Vec2 operator-() const {
		return Vec2{-x, -y};
	}

	Vec2& operator+=(const Vec2& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vec2& operator-=(const Vec2& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Vec2& operator*=(CoordType a) {
		x *= a;
		y *= a;
		return *this;
	}
};

Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {return Vec2{lhs.x + rhs.x, lhs.y + lhs.y};}
Vec2 operator-(const Vec2& lhs, const Vec2& rhs) {return Vec2{lhs.x - rhs.x, lhs.y - lhs.y};}
Vec2 operator*(const CoordType& lhs, const Vec2& rhs) {return Vec2{lhs * rhs.x, lhs * rhs.y};}
Vec2 operator*(const Vec2& lhs, const CoordType& rhs) {return Vec2{lhs.x * rhs, lhs.y * rhs};}

using Vec2Vector = std::vector<Vec2>;

using SizeType = unsigned int;

std::string rgb_color(SizeType r, SizeType g, SizeType b) {
	std::stringstream ss;
	ss << "#" << std::hex << std::setw(2) << std::setfill('0') << (r % 256)
		<< std::setw(2) << (g % 256) << std::setw(2) << (b % 256);
	return ss.str();
}

using HashType = size_t;
using TypeHashSet = std::unordered_set<HashType>;

class DefinitionsStream {
private:
	std::ostream &output_stream;
	TypeHashSet defined_drawables;

public:
	explicit DefinitionsStream(std::ostream &os) : output_stream{os} {}

	bool is_drawable_defined(const HashType &hash) const {
		return (defined_drawables.find(hash) != defined_drawables.end());
	}

	void set_drawable_defined(const HashType &hash) {
		defined_drawables.insert(hash);
	}

	void write_if_undefined(const HashType &hash, const char* def_code) {
		if(is_drawable_defined(hash))
			return;
		set_drawable_defined(hash);
		output_stream << def_code;
	}

	auto& stream() {return output_stream;}
};

class Drawable {
public:
	virtual ~Drawable() {}

	virtual void define(DefinitionsStream&) const {}
	virtual void draw(std::ostream &os) const = 0;
};

class ExpressionValue {
protected:
	std::string str_val;
public:
	virtual ~ExpressionValue() = default;
	ExpressionValue(const std::string& v) : str_val{ v } {}
	virtual const std::string& to_string() const { return str_val; }
};

class CoordExpressionValue : public ExpressionValue {
public:
	CoordExpressionValue(const std::string& v) : ExpressionValue{ v } {}
	CoordExpressionValue(const int& v) : ExpressionValue{ std::to_string(v) } {}
	CoordExpressionValue(const size_t& v) : ExpressionValue{ std::to_string(v) } {}
	CoordExpressionValue(const CoordType& v) : ExpressionValue{ std::to_string(v) } {}
};

class BoolExpressionValue : public ExpressionValue {
public:
	BoolExpressionValue(const std::string& v) : ExpressionValue{ v } {}
	BoolExpressionValue(const bool& b) : ExpressionValue{ (b ? "true" : "false") } {}
};

class PointExpressionValue : public ExpressionValue {
	std::string str_val_2;
public:
	PointExpressionValue(const std::string& v, const std::string& v2) : ExpressionValue{ v }, str_val_2{ v2 } {}
	virtual const std::string& to_string_2() const { return str_val_2; }
};

class Expression {
public:
	virtual ~Expression() {}

	virtual void init(std::ostream& os) const {}
	virtual void exit(std::ostream& os) const {}

	virtual const ExpressionValue& value() const = 0;
};

class LinearRangeExpression : public Expression {
	CoordType start, stop;
	SizeType steps;
	CoordExpressionValue var_name;
	static SizeType count;
public:
	LinearRangeExpression(CoordType start, CoordType stop, SizeType steps)
		: start{ start }, stop{ stop }, steps{ steps },
		var_name{ std::string("expressions.linear_range_") + std::to_string(count++) } {}
	virtual void init(std::ostream& os) const override {
		os << "if(" << var_name.to_string() << " == null) " << var_name.to_string() << " = " << start << ";\n";
	}
	virtual void exit(std::ostream& os) const override {
		if (start < stop) {
			const auto inc = (stop - start) / steps;
			os << "if(" << var_name.to_string() << " < " << stop << ") {\n"
				<< var_name.to_string() << " += " << inc << ";\n"
				<< "repeat_current_frame = true;\n"
				<< "}\n";
		}
		else {
			const auto inc = (start - stop) / steps;
			os << "if(" << var_name.to_string() << " > " << stop << ") {\n"
				<< var_name.to_string() << " -= " << inc << ";\n"
				<< "repeat_current_frame = true;\n"
				<< "}\n";
		}
	}
	virtual const ExpressionValue& value() const override { return var_name; }
};
SizeType LinearRangeExpression::count = 0;

class LinearPointExpression : public Expression {
	LinearRangeExpression range_1, range_2;
	PointExpressionValue point;
public:
	LinearPointExpression(const Vec2& start, const Vec2& stop, SizeType steps)
		: range_1( start.x, stop.x, steps ), range_2( start.y, stop.y, steps ),
		point(range_1.value().to_string(), range_2.value().to_string()) {}
	virtual void init(std::ostream& os) const override {
		range_1.init(os);
		range_2.init(os);
	}
	virtual void exit(std::ostream& os) const override {
		range_1.exit(os);
		range_2.exit(os);
	}
	virtual const ExpressionValue& value() const override { return point; }
};

class Arc : public Drawable {
	CoordExpressionValue x, y, r, sa, ea;
	BoolExpressionValue fill;
public:
	explicit Arc(const CoordExpressionValue& x, const CoordExpressionValue& y, const CoordExpressionValue& r,
		const CoordExpressionValue& sa, const CoordExpressionValue& ea, const BoolExpressionValue& fill)
		: x{ x }, y{ y }, r{ r }, sa{ sa }, ea{ ea }, fill{ fill } {}
	virtual void define(DefinitionsStream& ds) const override {
		ds.write_if_undefined(typeid(Arc).hash_code(), R"(
function arc(ctx, x, y, r, sa, ea, fill) {
	ctx.beginPath();
	ctx.arc(x, y, r, sa, ea);
	if(fill)
		ctx.fill();
	else
		ctx.stroke();
}
)");
	}
	virtual void draw(std::ostream &os) const override {
		os << "arc(ctx, " << x.to_string() << ", "
			<< y.to_string() << ", "
			<< r.to_string() << ", "
			<< sa.to_string() << ", "
			<< ea.to_string() << ", "
			<< fill.to_string() << ");\n";
	}
};

class Rect : public Drawable {
	CoordType x, y, w, h;
	bool fill;
public:
	explicit Rect(CoordType x, CoordType y, CoordType w, CoordType h, bool fill)
		: x{x}, y{y}, w{w}, h{h}, fill{fill} {}
	virtual void define(DefinitionsStream &ds) const override {
		ds.write_if_undefined(typeid(Rect).hash_code(), R"(
function rect(ctx, x, y, w, h, fill) {
	ctx.beginPath();
	ctx.rect(x, y, w, h);
	if(fill)
		ctx.fill();
	else
		ctx.stroke();
}
)");
	}
	virtual void draw(std::ostream& os) const override {
		os << "rect(ctx, " << static_cast<int>(x)
			<< ", " << static_cast<int>(y) << ", " << static_cast<int>(w)
			<< ", " << static_cast<int>(h) << ", " << (fill ? "true" : "false") << ");\n";
	}
};

class Line : public Drawable {
	Vec2Vector points;
	bool fill;
	bool close_path;
public:
	explicit Line(CoordType x1, CoordType y1, CoordType x2, CoordType y2)
		: points{Vec2(x1, y1), Vec2(x2, y2)}, fill{false}, close_path{false} {}
	explicit Line(const Vec2Vector& points, bool fill, bool close_path)
		: points{points}, fill{fill}, close_path{close_path} {
		if(points.size() < 2)
			throw std::runtime_error("Need at least 2 points for line");
	}
	virtual void define(DefinitionsStream &ds) const override {
		ds.write_if_undefined(typeid(Line).hash_code(), R"(
function line(ctx, x1, y1, x2, y2) {
	ctx.beginPath();
	ctx.moveTo(x1, y1);
	ctx.lineTo(x2, y2);
	ctx.stroke();
}
)");
	}
	virtual void draw(std::ostream& os) const override {
		if(points.size() == 2) {
			os << "line(ctx, " << static_cast<int>(points[0].x) << ", " << static_cast<int>(points[0].y)
				<< ", " << static_cast<int>(points[1].x) << ", " << static_cast<int>(points[1].y) << ");\n";
		}
		else {
			os << "ctx.beginPath();\n";
			os << "ctx.moveTo(" << static_cast<int>(points[0].x) << ", " << static_cast<int>(points[0].y) << ");\n";
			for(size_t p_i = 1; p_i < points.size(); ++p_i) {
				os << "ctx.lineTo(" << static_cast<int>(points[p_i].x) << ", " << static_cast<int>(points[p_i].y) << ");\n";
			}
			if(close_path)
				os << "ctx.closePath();\n";
			os << (fill ? "ctx.fill();\n" : "ctx.stroke();\n");
		}
	}
};

class Font : public Drawable {
	std::string font;
public:
	explicit Font(const std::string& font) : font{font} {}
	virtual void draw(std::ostream& os) const override
		{os << "ctx.font = \"" << font << "\";\n";}
};

class FillStyle : public Drawable {
	std::string style;
public:
	explicit FillStyle(const std::string& style) : style{style} {}
	virtual void draw(std::ostream& os) const override
		{os << "ctx.fillStyle = \"" << style << "\";\n";}
};

class StrokeStyle : public Drawable {
	std::string style;
public:
	explicit StrokeStyle(const std::string& style) : style{style} {}
	virtual void draw(std::ostream& os) const override
		{os << "ctx.strokeStyle = \"" << style << "\";\n";}
};

class LineCap : public Drawable {
	std::string style;
public:
	explicit LineCap(const std::string& style) : style{style} {}
	virtual void draw(std::ostream& os) const override
		{os << "ctx.lineCap = \"" << style << "\";\n";}
};

class LineWidth : public Drawable {
	SizeType width;
public:
	explicit LineWidth(SizeType width) : width{width} {}
	virtual void draw(std::ostream& os) const override
		{os << "ctx.lineWidth = " << width << ";\n";}
};

class Text : public Drawable {
	CoordType x, y;
	std::string txt;
	bool fill;
public:
	explicit Text(CoordType x, CoordType y, const char* txt, bool fill)
		: x{x}, y{y}, txt{txt}, fill{fill} {}
	virtual void define(DefinitionsStream &ds) const override {
		ds.write_if_undefined(typeid(Text).hash_code(), R"(
function text(ctx, x, y, txt, fill) {
	ctx.beginPath();
	if(fill)
		ctx.fillText(txt, x, y);
	else
		ctx.strokeText(txt, x, y);
}
)");
	}
	virtual void draw(std::ostream& os) const override {
		os << "text(ctx, " << static_cast<int>(x) << ", " << static_cast<int>(y)
			<< ", `" << txt << "`, " << (fill ? "true" : "false") << ");\n";
	}
};

class Scale : public Drawable {
	CoordType x, y;
public:
	explicit Scale(CoordType x, CoordType y) : x{x}, y{y} {}
	virtual void draw(std::ostream& os) const override {
		os << "ctx.scale(" << x << ", " << y << ");\n";
	}
};

class Rotate : public Drawable {
	CoordType rot;
public:
	explicit Rotate(CoordType rot) : rot{rot} {}
	virtual void draw(std::ostream& os) const override {
		os << "ctx.rotate(" << rot << ");\n";
	}
};

class Translate : public Drawable {
	CoordType x, y;
public:
	explicit Translate(CoordType x, CoordType y) : x{x}, y{y} {}
	virtual void draw(std::ostream& os) const override {
		os << "ctx.translate(" << static_cast<int>(x) << ", " << static_cast<int>(y) << ");\n";
	}
};

class DrawMacro : public Drawable {
	std::string name;
public:
	explicit DrawMacro(const std::string& name) : name{name} {}
	virtual void draw(std::ostream& os) const override {
		os << "macro_" << name << "(ctx);\n";
	}
};

using DrawableVector = std::vector<std::unique_ptr<Drawable>>;
using ExpressionVector = std::vector<std::unique_ptr<Expression>>;

class Frame : public Drawable {
	DrawableVector dwbl_vec;
	ExpressionVector expr_vec;
public:
	Frame() {}

	Frame& add_drawable(std::unique_ptr<Drawable>&& dwbl) {
		dwbl_vec.emplace_back(std::move(dwbl));
		return *this;
	}

	const CoordExpressionValue& add_coord_expression(std::unique_ptr<Expression>&& expr) {
		expr_vec.emplace_back(std::move(expr));
		return dynamic_cast<const CoordExpressionValue&>(expr_vec.back()->value());
	}

	const PointExpressionValue& add_point_expression(std::unique_ptr<Expression>&& expr) {
		expr_vec.emplace_back(std::move(expr));
		return dynamic_cast<const PointExpressionValue&>(expr_vec.back()->value());
	}

	void clear() {dwbl_vec.clear();}

	void define(DefinitionsStream &ds) const override {
		for(auto& dwbl : dwbl_vec) {
			dwbl->define(ds);
		}
	}

	void draw(std::ostream& os) const override {
		for(auto& expr : expr_vec) {
			expr->init(os);
		}
		for(auto& dwbl : dwbl_vec) {
			dwbl->draw(os);
		}
		for (auto& expr : expr_vec) {
			expr->exit(os);
		}
	}

	Frame& wait(SizeType n_frames)
	{
		add_coord_expression(std::make_unique<LinearRangeExpression>(0, n_frames, n_frames));
		return *this;
	}
	Frame& arc(const CoordExpressionValue& x, const CoordExpressionValue& y, const CoordExpressionValue& r,
		const BoolExpressionValue& fill = false, const CoordExpressionValue& sa = 0.0, const CoordExpressionValue& ea = 2 * PI)
	{
		return add_drawable(std::make_unique<Arc>(x, y, r, sa, ea, fill));
	}
	Frame& arc(const PointExpressionValue& p, const CoordExpressionValue& r,
		const BoolExpressionValue& fill = false, const CoordExpressionValue& sa = 0.0, const CoordExpressionValue & ea = 2 * PI)
	{
		return add_drawable(std::make_unique<Arc>(p.to_string(), p.to_string_2(), r, sa, ea, fill));
	}
	Frame& rect(CoordType x, CoordType y, CoordType w, CoordType h, bool fill = false)
	{
		return add_drawable(std::make_unique<Rect>(x, y, w, h, fill));
	}
	Frame& line(CoordType x1, CoordType y1, CoordType x2, CoordType y2)
	{
		return add_drawable(std::make_unique<Line>(x1, y1, x2, y2));
	}
	Frame& line(const Vec2Vector& points, bool fill = false, bool close_path = false)
	{
		return add_drawable(std::make_unique<Line>(points, fill, close_path));
	}
	Frame& line_cap(const std::string& style)
	{
		return add_drawable(std::make_unique<LineCap>(style));
	}
	Frame& line_width(SizeType width)
	{
		return add_drawable(std::make_unique<LineWidth>(width));
	}
	Frame& fill_style(const std::string& style)
	{
		return add_drawable(std::make_unique<FillStyle>(style));
	}
	Frame& stroke_style(const std::string& style)
	{
		return add_drawable(std::make_unique<StrokeStyle>(style));
	}
	Frame& text(CoordType x, CoordType y, std::string txt, bool fill = true)
	{
		return add_drawable(std::make_unique<Text>(x, y, txt.c_str(), fill));
	}
	Frame& rotate(CoordType rot)
	{
		return add_drawable(std::make_unique<Rotate>(rot));
	}
	Frame& translate(CoordType x, CoordType y)
	{
		return add_drawable(std::make_unique<Translate>(x, y));
	}
	Frame& scale(CoordType x, CoordType y)
	{
		return add_drawable(std::make_unique<Scale>(x, y));
	}
	Frame& draw_macro(const std::string& name) {
		return add_drawable(std::make_unique<DrawMacro>(name));
	}
	Frame& font(const std::string& font)
	{
		return add_drawable(std::make_unique<Font>(font));
	}

	const CoordExpressionValue& linear_range(CoordType start, CoordType stop, SizeType inc)
	{
		return add_coord_expression(std::make_unique<LinearRangeExpression>(start, stop, inc));
	}
	const PointExpressionValue& linear_point_range(const Vec2& start, const Vec2& stop, SizeType inc)
	{
		return add_point_expression(std::make_unique<LinearPointExpression>(start, stop, inc));
	}

	Frame& save();
	Frame& define_macro(const std::string& name);
};

class Save : public Frame {
public:
	explicit Save() {}
	virtual void draw(std::ostream& os) const override {
		os << "ctx.save();\n";
		Frame::draw(os);
		os << "ctx.restore();\n";
	}
};

Frame& Frame::save() {
	add_drawable(std::make_unique<Save>());
	return static_cast<Frame&>(*dwbl_vec.back());
}

class DefineMacro : public Frame {
	std::string name;
public:
	explicit DefineMacro(const std::string& name) : name{name} {}
	void define(DefinitionsStream &ds) const override {
		Frame::define(ds);
		ds.stream() << "function macro_" << name << "(ctx) {\n";
		Frame::draw(ds.stream());
		ds.stream() << "}\n";
	}
	virtual void draw(std::ostream& os) const override {}
};

Frame& Frame::define_macro(const std::string& name) {
	add_drawable(std::make_unique<DefineMacro>(name));
	return static_cast<Frame&>(*dwbl_vec.back());
}

using FrameVector = std::vector<std::unique_ptr<Frame>>;

class HtmlAnim {
private:
	std::string title;
	SizeType width;
	SizeType height;

	std::stringstream pre_text_stream;
	std::stringstream post_text_stream;

	bool no_clear = false;

	const std::string canvas_name = "anim_canvas_1";

	FrameVector frame_vec;
	size_t cur_frame;

	Frame bkgnd_frame, frgnd_frame;

public:
	HtmlAnim() {clear();}
	explicit HtmlAnim(const char* title = "HtmlAnim",
		SizeType width = 1024, SizeType height = 768)
		: title{title}, width{width}, height{height} {clear();}

	void clear() {
		bkgnd_frame.clear();
		frgnd_frame.clear();

		frame_vec.clear();
		cur_frame = 0;
		frame_vec.emplace_back(std::make_unique<Frame>());
	}

	auto& pre_text() {return pre_text_stream;}
	auto& post_text() {return post_text_stream;}

	void set_no_clear(bool do_clear) {no_clear = do_clear;}

	auto& background() {return bkgnd_frame;}
	auto& foreground() {return frgnd_frame;}

	auto& frame() {return *frame_vec[cur_frame];}

	auto get_num_frames() const {return frame_vec.size();}

	void rewind() {cur_frame = 0;}

	auto get_frame_index() const {return cur_frame;}

	void next_frame() {
		if(cur_frame == frame_vec.size() - 1) {
			frame_vec.emplace_back(std::make_unique<Frame>());
		}
		++cur_frame;
	}

	void remove_last_frame() {
		if(!frame_vec.empty())
			frame_vec.pop_back();
	}

	void write_stream(std::ostream&) const;
	void write_file(const char*) const;

	auto get_width() const {return width;}
	auto get_height() const {return height;}

private:
	void write_header(std::ostream& os) const;
	void write_canvas(std::ostream& os) const;

	void write_script(std::ostream& os) const;
	void write_definitions(std::ostream& os) const;
	void write_back_and_foreground(std::ostream& os) const;
	void write_frames(std::ostream& os) const;

	void write_footer(std::ostream& os) const;
};

void HtmlAnim::write_header(std::ostream& os) const {
	os << R"(<!doctype html>
<html>
<head>
<meta charset="utf-8">
<meta name="Description" content="Generated with HtmlAnim">
)";

	os << "<title>" << title << "</title>\n";

	os << R"(</head>
<body>
)";
}

void HtmlAnim::write_canvas(std::ostream& os) const {
	os << "<canvas id='" << canvas_name 
		<< "' width='" << width
		<< "' height='" << height
		<< "'></canvas>\n";
}

void HtmlAnim::write_script(std::ostream& os) const {
	os << "<script>\n";
	os << "<!--\n";
	os << "var frame_counter = 0;\n";
	os << "const num_frames = " << get_num_frames() << ";\n";
	os << "var canvas = document.getElementById('" << canvas_name << "');\n";
	os << "var no_clear = " << (no_clear ? "true" : "false") << ";\n";
	os << "var repeat_current_frame = false;\n";
	os << "var expressions = {};\n";

	write_definitions(os);

	write_back_and_foreground(os);
	write_frames(os);

	os << R"(
window.onload = function() {
	ctx = canvas.getContext('2d'),
	(function drawFrame () {
		if(frame_counter == 0 || !no_clear)
			ctx.clearRect(0, 0, canvas.width, canvas.height);
		repeat_current_frame = false;
		draw_bkgnd(ctx);
		(frames[frame_counter])(ctx);
		draw_frgnd(ctx);
		if(!repeat_current_frame) {
			frame_counter = (frame_counter + 1) % num_frames;
			expressions = {};
		}
		window.requestAnimationFrame(drawFrame, canvas);
	}());
}

//-->
</script>
<noscript>JavaScript is required to display this content.</noscript>
)";
}

void HtmlAnim::write_back_and_foreground(std::ostream& os) const {
	os << "function draw_bkgnd(ctx) {\n";
	bkgnd_frame.draw(os);
	os << "}\n\n";

	os << "function draw_frgnd(ctx) {\n";
	frgnd_frame.draw(os);
	os << "}\n\n";
}

void HtmlAnim::write_frames(std::ostream& os) const {
	os << "frames = [\n";
	for(size_t frame_i = 0; frame_i < frame_vec.size(); ++frame_i) {
		os << "(function(ctx) {\n";
		frame_vec[frame_i]->draw(os);
		os << "}),\n";
	}
	os << "];\n";
}

void HtmlAnim::write_definitions(std::ostream& os) const {
	DefinitionsStream ds(os);
	bkgnd_frame.define(ds);
	frgnd_frame.define(ds);
	for(const auto& frm : frame_vec) {
		frm->define(ds);
	}
}

void HtmlAnim::write_footer(std::ostream& os) const {
	os << R"(
</body>
</html>
)";
}

void HtmlAnim::write_stream(std::ostream& os) const {
	write_header(os);
	os << pre_text_stream.str() << "\n";
	write_canvas(os);
	write_script(os);
	os << post_text_stream.str() << "\n";
	write_footer(os);
}

void HtmlAnim::write_file(const char* path) const {
	std::ofstream outfile;
	outfile.open(path);
	write_stream(outfile);
	outfile.close();
}

} // namespace HtmlAnim
