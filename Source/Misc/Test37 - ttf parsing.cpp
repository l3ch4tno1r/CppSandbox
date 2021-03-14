#include <iostream>
#include <fstream>
#include <thread>

#include "External/ttf-parser/src/ttfParser.h"

void font_parsed(void* args, void* _font_data, int error)
{
	if (error)
	{
		*(uint8_t*)args = error;
		printf("Unable to parse font\n");
	}
	else
	{
		TTFFontParser::FontData* font_data = (TTFFontParser::FontData*)_font_data;

		printf("Font %s parsed\n", font_data->full_font_name.c_str());
		printf("Number of glyphs: %d\n", font_data->glyphs.size());

		*(uint8_t*)args = 1;
	}
}

int main()
{
	const char* fontPath = "Ressources/rainyhearts.ttf";
	uint8_t condition_variable = 0;

	TTFFontParser::FontData data;

	TTFFontParser::parse_file(fontPath, &data, font_parsed, &condition_variable);

	while (!condition_variable)
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

	uint16_t idx = data.glyph_map['('];
	auto& glyph = data.glyphs[idx];

	std::cout << glyph.num_triangles << std::endl;

	std::ofstream out("output.csv", std::ios::out);

	for (auto& path : glyph.path_list)
		for (auto& curve : path.curves)
			out << curve.p0.x << ';' << curve.p0.y << std::endl;

	std::cin.get();
}