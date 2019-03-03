#pragma once

#include <fstream>
#include <memory>
#include <vector>

namespace HtmlAnim {

using CoordType = int;
using SizeType = unsigned int;

class Drawable {
public:
	virtual ~Drawable() {}
	virtual void draw(std::ostream&) const = 0;
};

using DrawableVector = std::vector<std::unique_ptr<Drawable>>;

struct Frame {
	DrawableVector dwbl_vec;
};

using FrameVector = std::vector<std::unique_ptr<Frame>>;

class Rectangle : public Drawable {
	CoordType x, y;
	SizeType w, h;
	bool fill;
public:
	explicit Rectangle(CoordType x, CoordType y, SizeType w, SizeType h, bool fill)
		: x{x}, y(y), w(w), h(h), fill(fill) {}
	virtual void draw(std::ostream& os) const override {
		os << "rect(ctx, " << x << ", " << y << ", " << w << ", " << h << ", " << (fill ? "true" : "false") << ");\n";
	}
};

class Text : public Drawable {
	CoordType x, y;
	std::string txt;
	bool fill;
public:
	explicit Text(CoordType x, CoordType y, const char* txt, bool fill)
		: x{x}, y(y), txt(txt), fill(fill) {}
	virtual void draw(std::ostream& os) const override {
		os << "text(ctx, " << x << ", " << y << ", `" << txt << "`, " << (fill ? "true" : "false") << ");\n";
	}
};

class HtmlAnim {
public:
	HtmlAnim() {clear();}
	explicit HtmlAnim(const char* title = "HtmlAnim",
		SizeType width = 1024, SizeType height = 768, SizeType wait_frames = 0)
		: title(title), width(width), height(height), wait_frames(wait_frames) {clear();}

	void clear() {
		frame_vec.clear();
		next_frame();
	}

	void set_pre_text(const char* txt) {pre_text = txt;}
	void set_post_text(const char* txt) {post_text = txt;}

	auto get_num_frames() const {return frame_vec.size();}
	void next_frame() {frame_vec.emplace_back(std::make_unique<Frame>());}

	void rectangle(CoordType x, CoordType y, SizeType w, SizeType h, bool fill=false)
		{add_drawable(std::make_unique<Rectangle>(x, y, w, h, fill));}

	void text(CoordType x, CoordType y, std::string txt, bool fill=true)
		{add_drawable(std::make_unique<Text>(x, y, txt.c_str(), fill));}

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

	void add_drawable(std::unique_ptr<Drawable>&& dwbl) {frame_vec.back()->dwbl_vec.emplace_back(std::move(dwbl));}

	void write_header(std::ostream& os) const;
	void write_canvas(std::ostream& os) const;

	void write_script(std::ostream& os) const;
	void write_definitions(std::ostream& os) const;
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
	write_frames(os);

	os << R"(
window.onload = function() {
	ctx = canvas.getContext('2d'),
	(function drawFrame () {
		window.requestAnimationFrame(drawFrame, canvas);
		ctx.clearRect(0, 0, canvas.width, canvas.height);
		(frames[frame_counter])(ctx);
		wait_counter = (num_wait_frames == 0) ? 0 : ((wait_counter + 1) % num_wait_frames);
		if(wait_counter == 0)
			frame_counter = (frame_counter + 1) % num_frames;
	}());
}

//-->
</script>
<noscript>JavaScript is required to display this content.</noscript>
)";
}

void HtmlAnim::write_frames(std::ostream& os) const {
	os << "frames = [\n";

	for(size_t frame_i = 0; frame_i < frame_vec.size(); ++frame_i) {
		os << "(function(ctx) {\n";

		for(auto& cur_drw : frame_vec[frame_i]->dwbl_vec) {
			cur_drw->draw(os);
		}

		os << "}),\n";
	}

	os << "];\n";
}

void HtmlAnim::write_definitions(std::ostream& os) const {
	os << R"(
function rect(ctx, x, y, w, h, fill) {
	ctx.beginPath();
	ctx.rect(x, y, w, h);
	if(fill)
		ctx.fill();
	else
		ctx.stroke();
}

function text(ctx, x, y, txt, fill) {
	ctx.beginPath();
	if(fill)
		ctx.fillText(txt, x, y);
	else
		ctx.strokeText(txt, x, y);
}

)";
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
