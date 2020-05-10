#pragma once
#include <bitset>

// A cluster in a model
class Segment
{
private:
	
	// Material index of assigned mat
	uint32_t MATI = 0;

	// Location of material index of assigned material
	size_t MATI_Position = 0;

	// Position of chunk
	size_t SEGM_Position = 0;

	// Size of SEGM chunk
	size_t SEGM_Size = 0;

	// CLRB location
	size_t CLRB_Position = 0;

	// Vertex Color (single RGBA)
	unsigned char CLRB[4] = { 0, 0, 0, 0 };

	// Vertex Colors list (RGBA)
	std::vector<std::vector<unsigned char>> CLRL;

	// CLRL location
	size_t CLRL_Position = 0;

	// CLRL Size
	uint32_t CLRL_Size = 0;

	// Bool for easy detection
	bool CLRL_Present = false;

	// Bool for easy detection
	bool CLRB_Present = false;

	// Bool for checking
	bool CLRL_OG = false;

	// Bool for checking
	bool CLRB_OG = false;

	// CLRL list count
	uint32_t CLRL_Count = 0;

	friend class MSH;
	friend class View;
};

// A MODL chunk
class Model
{
private:

	// MODL index
	unsigned int MNDX = 1;

	// Name of MODL chunk
	std::string Name;

	// Size of MODL chunk
	size_t MODL_Size = 0;

	// Model size position
	size_t MODL_Position = 0;

	// GEOM chunk size
	size_t GEOM_Size = 0;

	// GEOM chunk position
	size_t GEOM_Position = 0;

	// Type of MODL (0null, 1skin, 3envelope, 4static, 6shadow)
	unsigned int MTYP = 0;

	// Parent MODL name (If applicable)
	std::string PRNT;

	// Material Clusters
	std::vector<Segment> Segments;

	// Whether the model is visible or not (Only present if hidden!)
	bool FLGS = false;

	// Whether the model is a cloth
	bool CLTH = false;

	// Position of CLTH header
	size_t CLTH_Position = 0;

	// Size of CLTH chunk
	size_t CLTH_Size = 0;

	// Cloth texture
	std::string CTEX;

	// Bool as to whether this model has been edited
	bool MODLChanged = false;

	// Position of name chunk
	size_t Name_Position = 0;

	// Size of name chunk
	unsigned int Name_Size = 0;

	// MODL index position
	size_t MNDX_Position = 0;

	// Position of MTYP
	size_t MTYP_Position = 0;

	// Size of the PRNT name
	unsigned int PRNT_Size = 0;

	// Index of parent model
	unsigned int PRNT_Index = 1;

	// Position of PRNT chunk
	size_t PRNT_Position = 0;

	// Position of FLGS chunk
	size_t FLGS_Position = 0;

	// Cloth texture name size
	unsigned int CTEX_Size = 0;

	// Position of CTEX chunk
	size_t CTEX_Position = 0;

	// Array of bools that tell which values need to be overwritten/inserted/erased
	// PRNT, NAME, FLGS, CTEX, MATI, CLRB, CLRL
	std::bitset<8> CHANGED;

	// Byte count of changes in MODL chunk (same indices as CHANGED)
	// (This only accounts for changes from OG file, not changes during editing!)
	unsigned int OG_Value[4] = { 0, 0, 0, 0 };

	// For editing purposes
	friend class MSH;
	friend class View;
};