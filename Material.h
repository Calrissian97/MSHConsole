#pragma once

// Bool because I hate macros
static bool DEBUG = false;
static bool ADVANCEDMODELS = false;

// Object containing all required data on a material
class Material
{
private:

	// Name of the material
	std::string MatName;

	// Textures (Diffuse, then extra according to rendertype)
	std::string TX0D;
	std::string TX1D;
	std::string TX2D;
	std::string TX3D;

	// ATRB RenderType
	unsigned char RenderType = 0;

	// ATRB Data 0 (U or horizontal scrolling)
	unsigned char Data0 = 0;

	// ATRB Data1 (V or Vertical scrolling)
	unsigned char Data1 = 0;

	// ATRB flags
	std::tuple<std::string, bool, unsigned char > MatFlags[8] =
	{
		std::make_tuple("specular", false, 0x80),
		std::make_tuple("additive", false, 0x40),
		std::make_tuple("perpixel", false, 0x20),
		std::make_tuple("hard", false, 0x10),
		std::make_tuple("double", false, 0x8),
		std::make_tuple("single", false, 0x4),
		std::make_tuple("glow", false, 0x2),
		std::make_tuple("emissive", false, 0x1),
	};

	bool MATDChanged = false;

	// Specular Decay
	float S_Decay = 50.0;

	// Diffuse RGBA (actually stored as BGRA) from 0.0 to 1.0
	float D_RGBA[4] = { 0.7, 0.7, 0.7, 1.0 };
	// Specular RGBA
	float S_RGBA[4] = { 0.7, 0.7, 0.7, 1.0 };
	// Ambient RGBA
	float A_RGBA[4] = { 0.7, 0.7, 0.7, 1.0 };

	// File positions of chunk size and the size itself
	size_t MATD_Position = 0;
	uint32_t MATD_Size = 0;
	size_t MatName_Position = 0;
	uint32_t MatName_Size = 0;
	size_t ATRB_Position = 0;
	size_t TX0D_Position = 0;
	uint32_t TX0D_Size = 0;
	size_t TX1D_Position = 0;
	uint32_t TX1D_Size = 0;
	size_t TX2D_Position = 0;
	uint32_t TX2D_Size = 0;
	size_t TX3D_Position = 0;
	uint32_t TX3D_Size = 0;
	uint32_t MATI = 0;
	size_t DATA_Position = 0;

	// Calculates the ATRB value based on material flags
	inline unsigned char CalculateATRB()
	{
		// Needs to be signed unsigned char for safe arithmetic
		unsigned char ATRB = 0;

		// Sums up the flags
		for (short F = 0; F < 8; F++)
			if (std::get<1>(MatFlags[F]))
				ATRB += std::get<2>(MatFlags[F]);

		// Verbose output
		if (DEBUG)
		{
			for (short C = 0; C < 8; C++)
				if (std::get<1>(MatFlags[C]))
					std::cout << " CalculateATRB:Flag: " << std::get<0>(MatFlags[C]) << "\n";

			std::cout << "\n";
		}

		return ATRB;
	}

	// Calculates material flags based on ATRB value
	inline void CalculateFlags(unsigned char Sum = 0)
	{
		// Cast to signed unsigned char for safe arithmetic
		unsigned char ATRB = static_cast<unsigned char>(Sum);

		// Iterate through and set flags accordingly
		for (short i = 0; i < 8; i++)
		{
			ATRB -= std::get<2>(MatFlags[i]);
			if (ATRB < 0)
			{
				std::get<1>(MatFlags[i]) = false;
				ATRB += std::get<2>(MatFlags[i]);
			}
			else
				std::get<1>(MatFlags[i]) = true;
		}

		// Verbose output
		if (DEBUG)
		{
			for (short C = 0; C < 8; C++)
				if (std::get<1>(MatFlags[C]))
					std::cout << " CalculateFlags:Flag: " << std::get<0>(MatFlags[C]) << "\n";
		}
	}

	// Parallel array holding rendertype names for easy printing
	static std::string RenderTypeName[32];

	// So that MSH can manipulate materials
	friend class MSH;

	// So that View can manipulate materials
	friend class View;
};

// Parallel array of RenderType names for easy printing
std::string Material::RenderTypeName[32] = {
	"Normal", "Glow", "Lightmap", "Scrolling", "Specular",
	"Gloss Map", "Chrome", "Animated", "Ice", "Sky",
	"Water", "Detail", "2 Scroll (V)", "Rotate",
	"Glow Rotate", "Planar Reflection", "Glow Scroll",
	"Glow 2 Scroll", "Curved Reflection", "NormalMap Fade",
	"NormalMap InvFade", "Ice Reflection","Ice Refraction",
	"Emboss", "Wireframe", "Energy", "Afterburner",
	"Bumpmap", "Bumpmap+Glossmap", "Teleportal",
};