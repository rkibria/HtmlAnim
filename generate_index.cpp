#include "htmlanim.hpp"
#include <ctgmath>

int main() {
	HtmlAnim::HtmlAnim anim("HtmlAnim", 400, 300, 5);

	anim.set_pre_text(R"(
<h1>HtmlAnim</h1>
<h2>A header-only C++ HTML animation library</h2>
)");

	anim.set_post_text(R"(
<p>The animation above can be generated using this code:</p>
<p><pre>
#include "htmlanim.hpp"
int main() {
	HtmlAnim::HtmlAnim anim("HtmlAnim", 400, 300, 5);

	anim.write_file("index.html");
}
</pre></p>
<hr>
<p>
<small>Github project: <a href="https://github.com/rkibria/HtmlAnim">https://github.com/rkibria/HtmlAnim</a></small>
</p>
)");

	auto marker = [&anim](auto x, auto y) {
		anim.rectangle(x, y, 10, 10);
	};

	for(int i = 0; i < 20; ++i) {
		for(int n = 0; n < 30; ++n) {
			const auto x = static_cast<double>(n) + static_cast<double>(i) / 5.0;
			marker(x * 15, 150 + 100 * sin(x / 2.0));
		}
		anim.next_frame();
	}

	anim.write_file("index.html");
}
