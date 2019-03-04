#pragma once

#include <fstream>
#include <memory>
#include <vector>
#include <unordered_set>

namespace HtmlAnim {

using CoordType = double;
using SizeType = unsigned int;

class Drawable {
public:
	virtual ~Drawable() {}
	virtual void define(std::ostream& os) const = 0;
	virtual void draw(std::ostream&) const = 0;
};

class Rect : public Drawable {
	CoordType x, y, w, h;
	bool fill;
public:
	explicit Rect(CoordType x, CoordType y, CoordType w, CoordType h, bool fill)
		: x{x}, y(y), w(w), h(h), fill(fill) {}
	virtual void define(std::ostream& os) const override {
		os << R"(
function rect(ctx, x, y, w, h, fill) {
	ctx.beginPath();
	ctx.rect(x, y, w, h);
	if(fill)
		ctx.fill();
	else
		ctx.stroke();
}
)";
	}
	virtual void draw(std::ostream& os) const override {
		os << "rect(ctx, " << static_cast<int>(x)
			<< ", " << static_cast<int>(y) << ", " << static_cast<int>(w)
			<< ", " << static_cast<int>(h) << ", " << (fill ? "true" : "false") << ");\n";
	}
};

class Text : public Drawable {
	CoordType x, y;
	std::string txt;
	bool fill;
public:
	explicit Text(CoordType x, CoordType y, const char* txt, bool fill)
		: x{x}, y(y), txt(txt), fill(fill) {}
	virtual void define(std::ostream& os) const override {
		os << R"(
function text(ctx, x, y, txt, fill) {
	ctx.beginPath();
	if(fill)
		ctx.fillText(txt, x, y);
	else
		ctx.strokeText(txt, x, y);
}
)";
	}
	virtual void draw(std::ostream& os) const override {
		os << "text(ctx, " << static_cast<int>(x) << ", " << static_cast<int>(y)
			<< ", `" << txt << "`, " << (fill ? "true" : "false") << ");\n";
	}
};

using DrawableVector = std::vector<std::unique_ptr<Drawable>>;
using TypeHashSet = std::unordered_set<size_t>;

class Frame {
	DrawableVector dwbl_vec;
public:
	Frame() {}

	void add_drawable(std::unique_ptr<Drawable>&& dwbl) {dwbl_vec.emplace_back(std::move(dwbl));}

	void clear() {dwbl_vec.clear();}

	void define(std::ostream& os, TypeHashSet& done_defs) const {
		for(auto& cur_drw : dwbl_vec) {
			const auto drw_type = typeid(*cur_drw).hash_code();
			if(done_defs.find(drw_type) == done_defs.end()) {
				done_defs.insert(drw_type);
				cur_drw->define(os);
			}
		}
	}

	void draw(std::ostream& os) const {
		for(auto& cur_drw : dwbl_vec) {
			cur_drw->draw(os);
		}
	}

	void rect(CoordType x, CoordType y, CoordType w, CoordType h, bool fill=false)
		{add_drawable(std::make_unique<Rect>(x, y, w, h, fill));}

	void text(CoordType x, CoordType y, std::string txt, bool fill=true)
		{add_drawable(std::make_unique<Text>(x, y, txt.c_str(), fill));}
};

using FrameVector = std::vector<std::unique_ptr<Frame>>;

class HtmlAnim {
public:
	HtmlAnim() {clear();}
	explicit HtmlAnim(const char* title = "HtmlAnim",
		SizeType width = 1024, SizeType height = 768, SizeType wait_frames = 0)
		: title(title), width(width), height(height), wait_frames(wait_frames) {clear();}

	void clear() {
		bkgnd_frame.clear();
		frgnd_frame.clear();
		frame_vec.clear();
		next_frame();
	}

	void set_pre_text(const char* txt) {pre_text = txt;}
	void set_post_text(const char* txt) {post_text = txt;}

	auto& background() {return bkgnd_frame;}
	auto& foreground() {return frgnd_frame;}
	auto& frame() {return *frame_vec.back();}

	auto get_num_frames() const {return frame_vec.size();}
	void next_frame() {frame_vec.emplace_back(std::make_unique<Frame>());}

	void write_stream(std::ostream&) const;
	void write_file(const char*) const;

private:
	std::string title;
	SizeType width;
	SizeType height;
	SizeType wait_frames;

	std::string pre_text;
	std::string post_text;

	std::string canvas_name = "anim_canvas_1";

	FrameVector frame_vec;

	Frame bkgnd_frame, frgnd_frame;

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

	os << "var wait_counter = 0;\n";
	os << "var num_wait_frames = " << wait_frames << ";\n";

	write_definitions(os);

	write_back_and_foreground(os);
	write_frames(os);

	os << R"(
window.onload = function() {
	ctx = canvas.getContext('2d'),
	(function drawFrame () {
		ctx.clearRect(0, 0, canvas.width, canvas.height);
		draw_bkgnd(ctx);
		(frames[frame_counter])(ctx);
		draw_frgnd(ctx);
		wait_counter = (num_wait_frames == 0) ? 0 : ((wait_counter + 1) % num_wait_frames);
		if(wait_counter == 0)
			frame_counter = (frame_counter + 1) % num_frames;
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
	TypeHashSet done_defs;

	bkgnd_frame.define(os, done_defs);
	frgnd_frame.define(os, done_defs);
	for(const auto& frm : frame_vec) {
		frm->define(os, done_defs);
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
	os << pre_text << "\n";
	write_canvas(os);
	write_script(os);
	os << post_text << "\n";
	write_footer(os);
}

void HtmlAnim::write_file(const char* path) const {
	std::ofstream outfile;
	outfile.open(path);
	write_stream(outfile);
	outfile.close();
}

} // namespace HtmlAnim
